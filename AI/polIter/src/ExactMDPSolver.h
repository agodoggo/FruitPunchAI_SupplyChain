#pragma once
#include<unordered_map>
#include <assert.h>
#include <limits>	
#include "StateWrapper.h"
#include "GreedyAgent.h"
#include "MDPAdapter.h"
#include "AgentAdapter.h"
#include "RNG.h"
/// <summary>
/// This is code that helps to evaluate the MDP exactly, i.e. without approximative methods.
/// This is the tabular approach. Uses hybrid between value iteration and  policy iteration.  
/// </summary>
template <class MDP, bool IncurTestOverhead = false>
class ExactMDPSolver
{
	MDPAdapter<MDP> adapter;
	bool silent{ false };
	using State = typename MDP::State;
	//When evaluating non-discounted infinite horizon models, 
	//we add transitions from state to themselves to ensure 
	//our algorithm converges. 
	const double probTransitionsToSelf = 0.02;
public:


	ExactMDPSolver(const MDP* mdp_ptr, bool silent=false) :
		adapter{ mdp_ptr }, silent{ silent },statesMap{}, statesVec{}
	{
		WasInitiated = false;
		if (!silent)
		{
			if (adapter.GetAlpha() == 1.0)
			{
				if (!MDPAdapter<MDP>::HasFinalState())
				{
					std::string Message = "Warning: You are calling exact-solver with alpha==1.0 and without implementing IsFinal. This is not guaranteed to work. The MDP should be at least weakly communicating for this to work, perhaps stronger conditions are needed.";
					std::cout << Message << std::endl;
				}
				else
				{
					std::string Message = "Warning: discount factor is 1.0 and you implement IsFinal. Assumption is made that eventually a final state is always reached. ";
					std::cout << Message << std::endl;
				}
			}
		}
	}



	/// <summary>
/// Finds the statewrapper in the vector of all statewrappers, that contains a given state, and returns a pointer to it.
/// </summary>
/// <param name="State">ref to state for which we will return the enclosing statewrapper</param>
/// <returns>Pointer to statewrapper that encloses the state</returns>
	StateWrapper<MDP>* find(const State& State) const
	{
		if (!WasInitiated)
		{
			throw std::logic_error("Please first compute a solution before trying to find statewrappers");
		}
		//Find returns iterator. Return the element that the iterator points to. 
		auto iterator = statesMap.find(&State);

		//Check whether the element is in the map. z
		if (iterator == statesMap.end())
		{//If not, print the state

			std::cout << "Cannot find state in dictionary" << std::endl;
			if constexpr (MDPAdapter<MDP>::StateCanSerialize())
			{
				std::cout << (State) << std::endl;
			}
			throw std::logic_error("Cannot find state in dictionary");
		}
		//Return the value (Wrapper) belonging to this key.
		return iterator->second;
	}


	/// <summary>
	/// Computes the optimal value function for the MDP using a tabular method and a hybrid between policy iteration and
	/// value iteration
	/// </summary>
	/// <param name="Tolerance">The precision to which the return value is calculated</param>
	/// <returns>Under the optimal policy: the average incurred costs per period (if average cost MDP, i.e. alpha=1.0), or
	/// the costs associated with starting in the mdp.InitialState() (if discounted MDP, i.e. alpha<1.0)</returns>
	template <class Agent>
	double ComputeOptimalCosts(Agent& agent, double tolerance, std::initializer_list<size_t> fixTasksToAgent)
	{		
		return DetermineCosts(true, agent, tolerance,fixTasksToAgent);
	}

	/// <summary>
/// Computes the optimal value function for the MDP using a tabular method and a hybrid between policy iteration and
/// value iteration
/// </summary>
/// <param name="Tolerance">The precision to which the return value is calculated</param>
/// <returns>Under the optimal policy: the average incurred costs per period (if average cost MDP, i.e. alpha=1.0), or
/// the costs associated with starting in the mdp.InitialState() (if discounted MDP, i.e. alpha<1.0)</returns>
	template <class Agent>
	double ComputeOptimalCosts(Agent& agent, double tolerance)
	{
		return DetermineCosts(true, agent, tolerance, std::vector<size_t>{});
	}



	/// <summary>
	/// Computes the optimal value function for the MDP using a tabular method and a hybrid between policy iteration and
	/// value iteration
	/// </summary>
	/// <param name="tolerance">The precision to which the return value is calculated</param>
	/// <returns>Under the optimal policy: the average incurred costs per period (if average cost MDP, i.e. alpha=1.0), or
	/// the costs associated with starting in the mdp.InitialState() (if discounted MDP, i.e. alpha<1.0)</returns>
	double ComputeOptimalCosts(double tolerance)
	{
		auto greedyAgent = GreedyAgent<MDP>{(adapter.mdp_ptr)};
		return DetermineCosts(true, greedyAgent, tolerance, std::vector<size_t>{});
	}



	/// <summary>
	/// Computes the value function associated with a given policy using value iteration.
	/// </summary>
	/// <param name="Policy">A function that assigns an action to each combination of State and Event</param>
	/// <param name="Tolerance">The precision to which the return value is calculated</param>
	/// <returns>Under the given policy: the average incurred costs per period (if average cost MDP, i.e. alpha=1.0), or
	/// the costs associated with starting in the mdp.InitialState() (if discounted MDP, i.e. alpha<1.0)</returns>
	template <class Agent>
	double ComputePolicyCosts(Agent& agent, double tolerance)
	{
		return DetermineCosts(false, agent, tolerance, std::vector<size_t>{});
	}



	size_t CurrentAction(const State &  state) const
	{
		if (!WasInitiated)
		{
			throw std::logic_error("Please first compute a solution before trying to determine actions");
		}
		State stateCopy{ state };//Need to have an address;
		auto found = find(stateCopy);
		return found->Action;
	}

private:
	void InitiateMap()
	{
		{
			auto AllStates = adapter.mdp_ptr->AllStates();
			if (!silent)
			{
				size_t EventStates{ 0 }, ActionStates{ 0 }, FinalStates{ 0 };
				for (auto& state : AllStates)
				{
					if (adapter.AwaitsAction(state))
					{
						ActionStates++;
					}
					else
					{
						if (adapter.IsFinalState(state))
						{
							FinalStates++;
						}
						else
						{
							EventStates++;
						}
					}
				}
				std::cout << "Number of states:" << AllStates.size() << " (a: " << ActionStates << " e: " << EventStates << " f: " << FinalStates << ")" << std::endl;
			}
			statesVec.reserve(AllStates.size());
			for (auto& st : AllStates)
			{
				size_t placeholders;
				if (adapter.AwaitsAction(st))
				{
					//only one transit possible.
					placeholders = 1;
				}
				else
				{
					if (adapter.IsFinalState(st))
					{
						placeholders = 0;
					}
					else
					{
						//one transit possible per event.
						placeholders = adapter.EventProbs(st).size();
					}
				}
				statesVec.push_back(StateWrapper<MDP>(st, placeholders));
			}
		}
		PopulateMap();
	}

	/// <summary>
	/// Does one value iteration over the vector of statewrappers. Makes all newVals equal to oldval, and 
	/// subsequently uses those oldvals to compute a set of newVals, all given the actions implemented for each state
	/// wrapper. 
	/// </summary>
	void IterateValues()
	{		
		for (auto& sw : statesVec)
		{
			sw.oldVal = sw.newVal;
		}


		for (auto& sw : statesVec)
		{
			if (adapter.AwaitsAction( sw.state))
			{//State with actions
				sw.newVal = sw.CostsInState + sw.TransitsToStates[0]->oldVal;
			}
			else
			{//State that awaits event
				if (!adapter.IsFinalState(sw.state))
				{
					auto& probs = adapter.EventProbs(sw.state);
					auto NumEvents = probs.size();

					sw.newVal = sw.CostsInState;
					for (size_t event = 0; event < NumEvents; event++)
					{
						sw.newVal += sw.TransitsToStates[event]->oldVal * probs[event];
					}
					sw.newVal *= adapter.GetAlpha();
				}
				else
				{
					sw.newVal = 0.0; 
				}
			}
		}

		
		if constexpr (!MDPAdapter<MDP>::HasFinalState())
		{			
			if (adapter.GetAlpha() == 1.0)
			{
				double probNoTransitionsToSelf = 1.0 - probTransitionsToSelf;
				for (auto& sw : statesVec)
				{					
					sw.newVal *= probNoTransitionsToSelf;
					sw.newVal += probTransitionsToSelf * sw.oldVal;
				}
			}
		}


	}
	



	/// <summary>
/// Sets the actions for all states as in the policy
/// </summary>
/// <param name="HeuristicAction">the heuristic to use, represented as a function pointer assigning the action to 
/// a particular state and event</param>
	template <class Agent>
	void SetPolicy(Agent& agent)
	{
		RNG rng{ 26071983 };
		AgentAdapter<MDP, Agent> agent_adapter{ adapter.mdp_ptr,&agent };
		if constexpr (AgentAdapter<MDP, Agent>::TakesRandomActions())
		{
			std::string Message = "Random agent not supported in exactMDPSolver!";
			std::cout << Message;
			throw Message;
		}

		for (auto& sw : statesVec)
		{
			if (adapter.AwaitsAction(sw.state))
			{
				auto tracker = adapter.GetTracker(sw.state);
				sw.Action =  agent_adapter.GetAction(sw.state,rng);
				adapter.IncorporateAction( tracker, sw.Action);
				sw.CostsInState = tracker.accumulatedCosts;
				sw.TransitsToStates[0] = find(tracker.state);
			}
		}
	}

	void UpdateActionsBasedOnCurrentValues(std::vector<bool>& taskIncludedInOpt)
	{
		for (auto& sw : statesVec)
		{
			if (adapter.AwaitsAction( sw.state))
			{
				if (!taskIncludedInOpt[adapter.TaskType(sw.state)])
				{
					continue;
				}

				double greedyActionCosts{ std::numeric_limits<double>::infinity() };

				auto numActions = adapter.NumValidActions(sw.state);

				if constexpr (IncurTestOverhead)
				{
					sw.CostsPerAction.reserve(numActions);
				}

				for (size_t action = 0;action < numActions;action++)
				{
					if (adapter.IsAllowedAction( sw.state, action))
					{
						auto tracker = adapter.GetTracker(sw.state);
						adapter.IncorporateAction( tracker, action);
						//Now the tracker corresponds to the next state, with associated costs;

						//find the state in the state structure
						auto asWrapper = find(tracker.state);
						auto CostForAction = tracker.accumulatedCosts;
						//Cost for action are direct costs + value of future action.
						double TotalCosts = CostForAction + asWrapper->newVal;

						if constexpr (IncurTestOverhead)
						{
							sw.CostsPerAction.push_back(TotalCosts);
						}

						if (TotalCosts < greedyActionCosts)
						{
							greedyActionCosts = TotalCosts;
							sw.Action = action;
							sw.TransitsToStates[0] = asWrapper;
							sw.CostsInState = CostForAction;
						}
					}
				}
			}
		}
	}

	void DetermineEventTransitions()
	{
		for (auto& sw : statesVec)
		{
			if (!adapter.AwaitsAction(sw.state))
			{
				if (adapter.IsFinalState(sw.state))
				{
					sw.CostsInState = 0.0;

				}
				else
				{
					auto& probs = adapter.EventProbs(sw.state);
					auto eventNum = probs.size();
					double cost = 0.0;
					for (typename MDPAdapter<MDP>::Event event = 0; event < eventNum; event++)
					{
						State next{ sw.state };//make copy
						//modify to take into account event, store costs:
						double costForEvent{ adapter.mdp_ptr->ModifyStateWithEvent(next, event) };
						cost += costForEvent * probs[event];
						sw.TransitsToStates[event] = find(next);
					}
					sw.CostsInState = cost;
				}
			}
		}
	}

	void ResetValues()
	{
		for (auto &sw : statesVec)
		{
			sw.oldVal = 0.0;
			sw.newVal = 0.0;
		}
	}

	void PopulateMap()
	{
		statesMap.reserve(statesVec.size());
		for (auto &sw : statesVec)
		{
			State* ptState = &(sw.state);
			if (statesMap.find(ptState) != statesMap.end())
			{
				std::cout << "double entry in statesMap:" << std::endl;
				std::cout << sw.state << std::endl;
				throw;
			}

			statesMap[ptState] = &sw;
		}
	}

	void PrintResults()
	{
		for (auto& wrapper : statesVec)
		{
			wrapper.PrintResults();
		}
	}






	/// <summary>
	/// Computes the costs for an MDP
	/// </summary>
	/// <param name="optimize">Indication whether we should optimize the policy (true) or evaluate a given policy (false)</param>
	/// <param name="policy">Optimize=true: Policy to initiate the actions with at the start of policy iteration
	/// Optimize=false: Policy to find the costs for</param>
	/// <param name="Tolerance">Tolerance for optimization</param>
	/// <returns></returns>
	template <class Agent>
	double DetermineCosts(bool optimize, Agent& agent, double tolerance, std::vector<size_t> FixTasksToAgent)
	{		
		if (!WasInitiated)
		{
			InitiateMap();
			WasInitiated = true;
		}

		std::vector<bool> TaskIncludedInOptimization(MDPAdapter<MDP>::NumTaskTypes(), true);
		for (auto task : FixTasksToAgent)
		{
			TaskIncludedInOptimization[task] = false;
		}


		ResetValues();
		double deltaMax,deltaMin;
		//Determine event transitions for event states:
		DetermineEventTransitions();
		//Set initial actions. 

		SetPolicy<Agent>(agent);
		double MaxChange ;
		double CurrentCost;
		do
		{
			for (size_t i = 0; i < 100; i++)
			{//For fixed actions, iterate the values a number of times. 
				IterateValues();
				
			}
			
			if (optimize)
			{//In this case, we are optimizing the actions. 
				UpdateActionsBasedOnCurrentValues(TaskIncludedInOptimization);
			}
			//Iterate the values one more time so that the convergence of the solution can be checked. Note that this
			//iteration is guaranteed to work based on the optimal actions if we are optimizing actions
			IterateValues();
			deltaMax = -std::numeric_limits<double>::infinity();

			deltaMin = std::numeric_limits<double>::infinity();;
			for (auto& wrapper : statesVec)
			{
				double delta{ wrapper.newVal - wrapper.oldVal };
				deltaMax = std::max(deltaMax, delta);
				deltaMin = std::min(deltaMin, delta);
			}
			MaxChange = std::max(deltaMax, -deltaMin);
			{
				auto newTraj = adapter.GetNewTracker();
				auto state = find(newTraj.state);
				CurrentCost = state->newVal;
			}
			if constexpr (!MDPAdapter<MDP>::HasFinalState())
			{
				if (adapter.GetAlpha() == 1.0)
				{
					double lowestVal = std::numeric_limits<double>::infinity();
					//This is purely for numerical stability: We subtract the minimum from every state. 
					//Otherwise, statevalues might grow to large to be represented reliably in floating point numbers
					for (auto& wrapper : statesVec)
					{
						lowestVal = std::min(wrapper.newVal, lowestVal);
					}
					for (auto& wrapper : statesVec)
					{
						wrapper.newVal -= lowestVal;
						wrapper.oldVal -= lowestVal;
					}
					MaxChange = (deltaMax - deltaMin) / 2.0 /(1.0-probTransitionsToSelf);
					CurrentCost = (deltaMax + deltaMin) / 2.0 /(1.0-probTransitionsToSelf);
				}
			}
			

			if (!silent)
			{
				std::cout << "Costs: " << CurrentCost << "    Convergence: "<< MaxChange << std::endl;

			}


		} while (MaxChange > tolerance);

		if constexpr (IncurTestOverhead)
		{
			UpdateActionsBasedOnCurrentValues(TaskIncludedInOptimization);
		}

		return CurrentCost;
	}


	struct PointedStateEq
	{
		bool operator() (State const * const lhs, State const * const rhs) const {
			return (*lhs) == (*rhs);
		}
	};

	struct PointedStateHash {
		std::size_t operator() (State const* const& st) const {
			return st->Hash();
		}
	};
	
	std::unordered_map<const State*, StateWrapper<MDP>*, PointedStateHash, PointedStateEq> statesMap;
	std::vector<StateWrapper<MDP>> statesVec;
	bool WasInitiated;
};

