#pragma once
#include<vector>
#include<cmath>
#include<cstddef>
#include<iostream>
#include "NetworkState.h"


class NetworkMDP
{
	std::string name = "NetworkMDP";//May be overwritten
public:

	std::string Identifier() const;


	using State = NetworkState;


	using Event = size_t;
	
	
	using RandomSource = std::vector<size_t>;
	using RandomSourceStatus = size_t;

	static constexpr size_t NumTaskTypes = 2;
	


	/// <summary>
	/// So the heuristic agent has access to private members of the MDP
	/// </summary>
	friend class NetworkAgent;

	


	/// <summary>
	/// Indicates whether the state is awaiting an event. States that await an event do not allow for actions: the next
	/// step must be modifying the state with a (random) event. 
	/// </summary>
	/// <param name="state">The state</param>
	/// <returns>A bool indicating whether the state awaits an event</returns>
	bool AwaitsAction(const State& state) const;
	


	/// <summary>
	/// Indicates the tasktype	of a state. Tasktype only exists for states that do not await an event and that are not Final. 
	/// </summary>
	/// <param name="state">The state</param>
	/// <returns>A size_t that acts as a label for the task type</returns>
	size_t TaskType(const State& state) const;



	/// <summary>
	/// Number of actions that could potentially be valid for a state with a certain TaskType. 
	/// </summary>
	/// <returns>Integer. Note that return value I may only depend on the task-type of the state. Potential valid actions are then 0,1,...,I-1. To test validity of each action for a state, call</returns>
	size_t NumValidActions(size_t taskType) const;


	bool IsAllowedAction(const State& state, size_t action) const;
	double ModifyStateWithAction(State& state, size_t action) const;





	size_t NumFeatures() const;
	/// <summary>
	/// Extracts the features, used for deciding which action to take in a state, from the state. 
	/// </summary>
	/// <param name="state">The state from which to extract the features</param>
	/// <param name="feat_array">An array in which to put the features. Caller should pass array of length NumFeatures(TaskType(state)).</param>
	void ExtractFeatures(const State& state,float* out_feat_array) const;

	
	void ToTikzFigure(std::ostream & os, const State& State, size_t action, bool first);


	//These static functions return specific instances
	//of the network MDP for testing. 
	static NetworkMDP GetComplexInstance(size_t size);
	static NetworkMDP GetSimpleInstance();
	static NetworkMDP GetSemiconInstance(bool SmallerInstance);


	State InitialState() const;


	//applicable discount factor
	double alpha{ 0.0 };


	const std::vector<double>& EventProbs() const;
	double ModifyStateWithEvent(State& state, size_t event) const;


    //This function is only needed if we want to solve the MDP exactly
	//using the existing code. 
	std::vector<State> AllStates() const;



private:
	//For conversion to Tikz Figure
	std::string ToNumber(short Inventory) const
	{
		if (Inventory == 0)
		{
			return "zero";
		}
		if (Inventory == 1)
		{
			return "one";
		}
		if (Inventory == 2)
		{
			return "two";
		}
		if (Inventory == 3)
		{
			return "three";
		}
		throw;
	}

	struct Location {
		double x{ 0.0 };
		double y{ 0.0 };
		short region{ 0 };
		
		double RegionPenaltyTo(Location other)
		{
			if (region == other.region)
			{
				return 0.0;//hours
			}
			if (region == 0 && other.region == 2)
			{//from EMEA to us
				return 2.0;//hours
			}

			//Any other intercontinental
			return 6.0;
		}

		double DistTo(Location other)
		{
			auto dist = metricDistTo(other);
			if (region == 0)
			{
				//Earth is round, incorporate this for origin europe, i.e. they can cross atlantic.
				//Other options not incorporated because not important.
				Location loc{ *this };//Make copy
				loc.x += 18;
				auto altdist = loc.metricDistTo(other);
				if (altdist < dist)
				{
					dist = altdist;
				}
			}
			return dist;
		}
		Location(double x, double y, short region) :x{ x }, y{ y }, region{region}
		{

		}
		Location()
		{}
	private:
		double metricDistTo(Location other)
		{
			auto delta = this->Subtract(other);
			return std::sqrt(delta.x*delta.x + delta.y*delta.y);
		}

		Location Subtract(Location other)
		{
			return Location{ x - other.x,y - other.y ,region};

		}
	};
	struct Warehouse
	{
		Location loc;
		/// <summary>
		/// Maximum inventory that the warehouse can hold. 
		/// </summary>
		NetworkMDP::State::InvCount MaxInv;
		/// <summary>
		/// Number of warehouse. Warehouses are numbered consequtively from 0 to warehouses.size(). 
		/// </summary>
		size_t number;
		std::string LabelPlacement;
		/// <summary>
		///   indicates whether the warehouse is regional hub
		/// </summary>
		bool regionalHub;
		Warehouse(Location loc, NetworkMDP::State::InvCount MaxInv, size_t count) : Warehouse(loc, MaxInv, count, "above right=1.3mm",true)
		{
		}

		Warehouse(Location loc, NetworkMDP::State::InvCount MaxInv, size_t count, std::string LabelPlacement, bool regionalHub) : loc{ loc }, MaxInv{ MaxInv }, number{ count }, LabelPlacement{ LabelPlacement }, regionalHub{regionalHub}
		{

		}
	};
	struct Customer
	{
		Location loc;
		/// <summary>
		/// Multiplicative modifier of the costs of slower reaction times when part is needed. 
		/// </summary>
		double importance;
		double relativeDemandFrequency;
		/// <summary>
		/// Each customer has a unique number; numbers range from 0 to customers.size()-1. 
		/// </summary>
		size_t number;
		//Indicates the possibles levels (that moderate penalties) that the customer can have.
		State::CustStatusOption MaxOption = 1;
	
		/// <summary>
		/// Indicates placement of the label in TIKZ
		/// </summary>
		std::string LabelPlacementTikz;
	

		Customer(Location loc, double importance, double relativeDemandFrequency, size_t number)
			:Customer(loc, importance, relativeDemandFrequency, number, "above right=1.3 mm")
		{

		}
		Customer(Location loc, double importance, double relativeDemandFrequency, size_t number, std::string LabelPlacement)
			:loc{ loc }, importance{ importance }, relativeDemandFrequency{ relativeDemandFrequency }, number{ number }, LabelPlacementTikz{ LabelPlacement }
		{
		}
	};

	std::vector<Customer> Customers;
	std::vector<Warehouse> Warehouses;

	//Temp source for checking distances
	size_t whsourceTikz{ 0 };
	void ToTikzFigure(std::ostream & os, const State& State, size_t action,bool explicitAction, bool first);

	
	enum class StatusType :unsigned int { DemandArrival, EvalMoves, AlertRaised, AlertsExpired, ReplenishmentEvent , AwaitEvent };
	
	
	struct Status
	{
		//Initializations purely to avoid uninitialized variables:
		StatusType statusType{StatusType::DemandArrival};
		size_t eventModifier{ 0 };
		size_t numLegalActions{ 0 };
		/// <summary>
		/// A state can only take on stateType statuses. 
		/// </summary>
		bool canBeTakenOnByState{ false };
		/// <summary>
		/// An event may only take on values corresponding to an eventType Status. 
		/// </summary>
		bool isOfEventType{ false };
	};

	/// <summary>
	/// List of all possible statuses, either of event type, or of state type:
	/// Event type status are statuses that correspond to an event that may happen. stateType statuses 
	/// may occur for a particular state in the MDP. Note that at least one applies, sometimes both. 
	/// </summary>
	std::vector<Status> StatusList;


	size_t AwaitEventStatus{ 0 };
	size_t EvaluateMoveStatus{ 0 };

	/// <summary>
	/// The probabilities of the various events. THese are independent of the state in the current setting. 
	/// </summary>
	std::vector<double> EventProbsStatic;

	/// <summary>
	/// Represents a possible transfer action from warehouse to warehouse with costs. 
	/// </summary>
	struct TransferAction
	{
		size_t NumberFrom;
		size_t NumberTo;
		double Cost;

		TransferAction(size_t from, size_t to, double cost)
			:NumberFrom{ from }, NumberTo{ to }, Cost{ cost }
		{

		}
	};

	/// <summary>
	/// List of possible transfers with costs. 
	/// </summary>
	std::vector<TransferAction> TransferActions;

	/// <summary>
	/// Warehouse where replenishments originate from. 
	/// </summary>
	size_t SourceWarehouse{ 0 };
	/// <summary>
	/// Costs of moves immediately after a replenishment. 
	/// </summary>
	std::vector<double> MoveAfterReplenishmentCosts;
	
	

	struct WHCustData
	{
		double Cost;
		double HoursOfflight;
		double HoursOfTruck;
		std::string type;
	};

	/// <summary>
	/// Costs of moving parts from warehouse to customer upon demand. 
	/// </summary>
	std::vector<std::vector<WHCustData>> WHCustCostMatrix;
	
	


	/// /// <summary>
	/// Costs of emergency procedure to obtain part. 
	/// </summary>
	double emergencyProcecureCosts{ 0.0 };


	/// <summary>
	/// Order in which warehouses are replenished for heuristic. 
	/// </summary>
	std::vector<size_t> WHPriorities;

	/// <summary>
	/// Returns a vector containing all possible configurations of statusoption per customer, taking into account 
	/// various constraints. 
	/// </summary>
	std::vector<std::vector<State::CustStatusOption>>  LegalCustStatusOptions() const;

	/// <summary>
	/// ratio between total demand rate and total supply rate. 
	/// </summary>
	double rho{ 0.0 };
	   

	bool ForbidShipmentsFromLocal = false;
	/// <summary>
	/// Maximum number of simultaneous alerts. 
	/// </summary>
	unsigned int maxAlertedCustomers{ 0 };

	size_t MaxSystemInventory{ 0 };
	/// <summary>
	/// Average total number of alerts generated per total number of customer demands. This says
	/// something about the stability of alerts, i.e. how long they remain. 
	/// </summary>
	double alertsPerDemand{ 0.0 };
	double resetsPerAlert{ 0.0 };
	double MultiplicativeCostEffectOfAlertedCustomer{ 0.0 };

	NetworkMDP();

	void Initiate()
	{
		InitiateWHCustCostMatrix();	
		InitiateTransferActions();
		InitiateStatusInfo();
		NormalizeEventProbs();
	}
	void InitiateWHCustCostMatrix();
	void InitiateTransferActions();
	void InitiateStatusInfo();
	void NormalizeEventProbs();

	std::string StatusTypeToString(StatusType type)
	{
		switch (type)
		{
		case StatusType::AlertRaised:
			return "alert raised";
		case StatusType::AlertsExpired:
			return "alert expired";
		case StatusType::DemandArrival:
			return "demand arrival";
		case StatusType::EvalMoves:
			return "evaluate moves";
		case StatusType::ReplenishmentEvent:
			return "replenishment event";
		case StatusType::AwaitEvent:
			return "await event";
		default:
			throw;
		}
	}

	double distCosts(double dist)
	{//cost for lateral transshipments without hurry
		//return 0.0;
		return 0.2 + 0.05*dist;
	}
};
