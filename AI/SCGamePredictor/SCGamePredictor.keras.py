#modified by Agoston Walter for compatibility with EAISI Supply Chain Game on March 20, 2021

import random
from tensorflow import keras
import numpy as np
import sys, getopt #https://www.tutorialspoint.com/python/python_command_line_arguments.htm

# Implements the Supply Chain Game recommender agent.
# Given a state of the game board, the recommender agent
# recommends he actions to play in that state.
#
# The state and the recommendation are given in terms of 
# numbers of tokens on places of the game board. Each place
# on the game board has a number 0...16. The numbers of these
# inventory_places are marked on the game_board.pdf file.
# 
# Each state has 18 elements. For a state:
# - the list inventory_places = state[1:] are the inventory places on the game board
# - state[0] is the number of remaining demands
#   (a game typically has 10 rounds, so initially the state has 10 remaining demands)
#
# The get_recommendation function is the main entry into this class.
# This function returns the recommended actions for a given state
# If returns the tokens on the inventory places after playing these actions.
#
# As an example, you can also exectute the play function. This 
# prints an example of playing a full game.
#
# MORE DETAILED:
# There are 4 types of actions: order_actions, production_actions, transshipment_actions and evolve.
# An order_action specifies the number of components or finished products to order on inventory places 0...4:
# order_action[0] = inventory_places[0] = number of phone components to order, 
# order_action[1] = inventory_places[1] = number of electronics components to order, 
# order_action[2] = inventory_places[2] = number of computer components to order, 
# order_action[3] = inventory_places[3] = number of finished phones to order,
# order_action[4] = inventory_places[4] = number of finished computers to order.
# A production_action specifies the number of products to produce on inventory places 5...6:
# production_action[0] = inventory_places[5] = number of phones to produce,
# production_action[1] = inventory_places[6] = number of computers to produce.
# A transshipment_action specifies the number of phones to move from The Netherlands to Germany
# by putting them on inventory_places[7]
# An evolve action, evolves the board, when a recommendation is followed.
#
# A recommendation is a concatenation of actions of the first three types, i.e.:
# let order_action, production_action, transshipment_action be actions of the corresponding type.
# order_action + production_action + transshipment_action = recommendation
# Note that the game board place p in recommendation[p] corresponds to inventory_places[p].

class SCGameRecommenderKeras:
    
    # Initializes the Supply Chain Game recommender agent from the specified neural network_file.
    def __init__(self, network_file):
        self.k_model = keras.models.load_model(network_file)
        
        self.action_types = ["order_actions", "production_actions", "transshipment_actions"]
        self.actions = {}
        self.actions["order_actions"] = [(i1,i2,i3,i4,i5) for i5 in range(3) for i4 in range(3) for i3 in range(7) for i2 in range(7) for i1 in range(7) if i1+i2+i3+3*(i4+i5) == 6]
        self.actions["production_actions"] = [(i1,i2) for i2 in range(7) for i1 in range(7)]
        self.actions["transshipment_actions"] = [i for i in range(7)]
        self.action_type_offset = {"order_actions": 0, "production_actions": len(self.actions["order_actions"]), "transshipment_actions": len(self.actions["order_actions"]) + len(self.actions["production_actions"])}
        
    # Returns True if the action of the specified action_type
    # is allowed in the state. False, otherwise.
    def is_allowed_action(self, state, action, action_type):
        invs = state[1:] # The first element of the state is the number of remaining demands, skip that
        if action_type == "order_actions":
            return True
        elif action_type == "production_actions":
            prodQuantities = action
            return (prodQuantities[0] <= invs[10]) and (prodQuantities[1] <= invs[11]) and (prodQuantities[0] + prodQuantities[1] <= invs[9])
        elif action_type == "transshipment_actions":
            transQuantity = action
            return transQuantity <= invs[15]      
        else:
            raise Exception("Invalid action_type")
    
    # Returns the best action of the specified action_type
    # in the specified state, according to the neural network.
    def get_best_action(self, state, action_type):
        action_scores = self.k_model.predict(np.array([state])).tolist()[0]
        offset = self.action_type_offset[action_type]
        nr_actions = len(self.actions[action_type])
        sorted_scores_with_action_nrs = [(action_scores[i], i) for i in range(offset, offset+nr_actions)]
        sorted_scores_with_action_nrs.sort(reverse=True)
        
        for (score, action_nr) in sorted_scores_with_action_nrs:
            action = self.actions[action_type][action_nr - self.action_type_offset[action_type]]
            if self.is_allowed_action(state, action, action_type):
                return action
            
        raise Exception("No allowed action found. This should not be possible. It probably represents a programming error.")

    # Updates the specified state with the specified action of the specified action_type.
    # Returns the updated state.
    def modify_state_with_action(self, state, action, action_type):
        invs = state[1:].copy() # The first element of the state is the number of remaining demands, skip that
        
        if action_type == "order_actions":
            orderQuantities = action
            for i in range(5):
                invs[i] = orderQuantities[i]    
        elif action_type == "production_actions":
            prodQuantities = action
            invs[5] += prodQuantities[0]
            invs[6] += prodQuantities[1]
            invs[9] -= prodQuantities[0] + prodQuantities[1]
            invs[10] -= prodQuantities[0]
            invs[11] -= prodQuantities[1]
        elif action_type == "transshipment_actions":
            transQuantity = action
            invs[15] -= transQuantity
            invs[7] += transQuantity
        elif action_type == "evolve":
            invs[14] += invs[7];
            invs[7] = 0;
            invs[15] += invs[3] + invs[12];
            invs[3] = 0;
            invs[12] = invs[5];
            invs[5] = 0;
            invs[10] += invs[0];
            invs[0] = 0;
            invs[9] += invs[8];
            invs[8] = invs[1];
            invs[1] = 0;
            invs[16] += invs[4] + invs[13];
            invs[4] = 0;
            invs[13] = invs[6];
            invs[6] = 0;
            invs[11] += invs[2];
            invs[2] = 0;
        else:
            raise Exception("Invalid action_type")
        
        return [state[0]] + invs
    
    # Returns a recommendation of the best combination of an
    # order action, a production action and a transshipment action to take
    # in the specified state, according to the neural network.    
    # Returns the recommendation in the form of a concatenation
    # order_action + production_action + transshipment_action as well as the 
    # effect that executing the recommendation has on the state.
    def get_recommendation(self, state):
        new_state = state
        for action_type in self.action_types:
            best_action = self.get_best_action(new_state, action_type)
            new_state = self.modify_state_with_action(new_state, best_action, action_type)
        return new_state
    
    # Returns the initial state of the game.
    def initial_state(self):
        state = [0]*18
        state[0] = 10
        state[8] = 2
        state[9] = 2
        state[10] = 2
        state[11] = 2
        state[12] = 3
        state[13] = 3
        state[15] = 3        
        
        return state
    
    # Returns a random demand in the form (location, quantity), where:
    # location in [0,1,2], sucht that 0 means phones in GE, 1 means phones in NL, 2 means computers in NL
    # demand quantity in [1,2,3,4,5,6]
    def demand(self):
        return (random.randint(0,2), random.randint(1,6))
    
    # Updates the specified state with the specified demand.
    # Returns (the updated state, the reward for fulfilling the demand)
    # The reward for fulfilling the demand is equal to the number of products
    # that could be fulfilled.
    def modify_state_with_event(self, state, demand):
        invs = state[1:].copy()
        demand_location = 14 + demand[0]
        demand_quantity = demand[1]
        
        reward = min(invs[demand_location], demand_quantity)
        invs[demand_location] -= reward
        
        return [state[0]-1] + invs, reward  
    
    # Given a recommendation, returns a string that 
    # verbalizes the recommendation.
    def pretty_print_recommendation(self, recommendation):
        a = recommendation[1:9]
        result = ""
        result += "Order phone components:      " + str(a[0]) + "\n"
        result += "Order electronic components: " + str(a[1]) + "\n"
        result += "Order computer components:   " + str(a[2]) + "\n"
        result += "Order phone:                 " + str(a[3]) + "\n"
        result += "Order computer:              " + str(a[4]) + "\n"
        result += "Produce phone:               " + str(a[5]) + "\n"
        result += "Produce computer:            " + str(a[6]) + "\n"
        result += "Transship:                   " + str(a[7]) + "\n"
        return result
    
    # Plays a full game.
    # Prints the results to the standard output.
    def play(self):
        state = self.initial_state()
        total_reward = 0
        print(state)        
        while state[0] > 0:
            recommendation = self.get_recommendation(state)
            state = self.modify_state_with_action(recommendation, None, "evolve")
            demand = self.demand()
            state, reward = self.modify_state_with_event(state, demand)
            print(self.pretty_print_recommendation(recommendation))
            location = " phones in Germany" if demand[0] == 0 else " phones in The Netherlands" if demand[0] == 1 else " computers in The Netherlands"
            print("Demand: " + str(demand[1]) + location)
            print("Reward: " + str(reward))
            print(state)
            print()
            total_reward += reward
        print("TOTAL REWARD: " + str(total_reward))
        
        return total_reward
     
    def argParse(self):
        state = [0]*18
        if(len(sys.argv)!=19):
            print(str(len(sys.argv)) + "inputs, not 18")
        else:
            i=1
            while(i<len(sys.argv)):
                state[i-1] = sys.argv[i]
                i=i+1
        return state
    
keras_model = SCGameRecommenderKeras("model.keras")
state = keras_model.argParse()
print(state)
# recommendation = keras_model.get_recommendation(state)
# print(recommendation)
# keras_model.play()

# state = model.initial_state()
# print(model.pretty_print_recommendation(model.get_recommendation(state)))