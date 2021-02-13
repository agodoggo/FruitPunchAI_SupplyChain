#pragma once
#include <iostream>
#include <array>
#include <type_traits>
#include "EventStream.h"
#include "RNG.h"



template <typename MDP>
class MDPAdapter final
{


public:
	const MDP* mdp_ptr;



private:

	using State = typename MDP::State;


	template<class>
	struct sfinae_true : std::true_type {};

	template<class T, class A0>
	static auto test_IsFinal(int)
		->sfinae_true<decltype(std::declval<T>().IsFinal(std::declval<A0>()))>;
	template<class, class A0>
	static auto test_IsFinal(long)->std::false_type;

	template<class T, class Arg>
	struct has_IsFinal : decltype(test_IsFinal<T, Arg>(0)){};

	
	template<class T, class A0>
	static auto test_NumEventsRemaining(int)
		->sfinae_true<decltype(std::declval<T>().NumEventsRemaining(std::declval<A0>()))>;
	template<class, class A0>
	static auto test_NumEventsRemaining(long)->std::false_type;

	template<class T, class Arg>
	struct has_NumEventsRemaining : decltype(test_NumEventsRemaining<T, Arg>(0)){};


	template<class T, class A0>
	static auto test_EventProbsStateDep(int)
		->sfinae_true<decltype(std::declval<T>().EventProbs(std::declval<A0>()))>;
	template<class, class A0>
	static auto test_EventProbsStateDep(long)->std::false_type;

	template<class T, class Arg>
	struct has_EventProbsStateDependent : decltype(test_EventProbsStateDep<T, Arg>(0)){};


	template<class T, class A0>
	static auto test_EventProbsIndependent(int)
		->sfinae_true<decltype(std::declval<T>().EventProbs())>;
	template<class, class A0>
	static auto test_EventProbsIndependent(long)->std::false_type;

	template<class T, class Arg>
	struct has_EventProbsIndependent : decltype(test_EventProbsIndependent<T, Arg>(0)){};


	template<class T, class A0>
	static auto test_RandomInitialState(int)
		->sfinae_true<decltype(std::declval<T>().InitialState(*std::declval<A0*>()))>;
	template<class, class A0>
	static auto test_RandomInitialState(long)->std::false_type;

	template<class T, class Arg>
	struct has_RandomInitialState : decltype(test_RandomInitialState<T, Arg>(0)){};



	template<class T, class A0, class A1>
	static auto test_IsAllowedAction(int)
		->sfinae_true<decltype(std::declval<T>().IsAllowedAction(*std::declval<A0*>(), std::declval<A1>()))>;
	template<class, class A0, class A1>
	static auto test_IsAllowedAction(long)->std::false_type;

	template<class T, class Arg0, class Arg1>
	struct has_IsAllowedAction : decltype(test_IsAllowedAction<T, Arg0, Arg1>(0)){};


	
	struct DefaultEventProvider {
		using Event = size_t;
	};

	template <typename T, typename = void>
	struct hasEventDeclaration : std::false_type {};

	template <typename T>
	struct hasEventDeclaration<T, std::void_t<typename T::Event>> : std::true_type {};



	template <typename T, typename = void>
	struct stateHasCanSerializeFlag : std::false_type {};
	template <typename T>
	struct stateHasCanSerializeFlag<T, decltype((void)T::CanSerialize, void())> : std::true_type {};

	


	template <typename T>
	static constexpr bool HasEventOverride()
	{
		if constexpr (hasEventDeclaration<T>::value)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<class T>
	using EventTemplate = typename std::conditional_t<HasEventOverride<T>(), T, DefaultEventProvider>::Event;

public:
	using Event = EventTemplate<MDP>;

	static constexpr bool StateCanSerialize()
	{
		if constexpr (stateHasCanSerializeFlag<State>::value)
		{
			return State::CanSerialize;
		}
		else
		{
			return false;
		}
	}

private:

	//Detects whether T has a member Alpha. 
	
	template <typename T, typename = void>
	struct HasAlpha : std::false_type {};
	template <typename T>
	struct HasAlpha<T, decltype((void)T::alpha, void())> : std::true_type {};

	template <typename T, typename = void>
	struct hasNumTaskTypes : std::false_type {};
	template <typename T>
	struct hasNumTaskTypes<T, decltype((void)T::NumTaskTypes, void())> : std::true_type {};

	struct ExogenousEventTypeProvider
	{
		using RandomSource = std::vector<Event>;
		using RandomSourceStatus = size_t;
	};

	public:
	static constexpr bool ExogenousEventDriven()
	{
		return has_EventProbsIndependent<MDP, State>::value;
	}

	static constexpr bool HasRandomInitialState()
	{
		return has_RandomInitialState<MDP, RNG>::value;
	}

	static constexpr bool HasIsAllowedAction()
	{
		return has_IsAllowedAction < MDP, State, size_t>::value;
	}

	static constexpr bool HasFinalState()
	{
		return has_IsFinal<MDP, State>();
	}

	
	static constexpr bool HasNumEventsRemaining()
	{
		return has_NumEventsRemaining<MDP, State>();
	}
	private:

	template<class T>
	using RandomSourceTemplate = typename std::conditional_t<ExogenousEventDriven(), ExogenousEventTypeProvider, T>::RandomSource;
	template<class T>
	using RandomSourceStatusTemplate = typename std::conditional_t<ExogenousEventDriven(), ExogenousEventTypeProvider, T>::RandomSourceStatus;




public:




	static constexpr size_t NumTaskTypes()
	{
		if constexpr (hasNumTaskTypes<MDP>::value)
		{
			return MDP::NumTaskTypes;
		}
		else
		{
			return 1;
		}
	}



	using RandomSource = RandomSourceTemplate<MDP>;

	using RandomSourceStatus = RandomSourceStatusTemplate<MDP>;

	struct TrajectoryTracker
	{
		State state;
		double accumulatedCosts;
		explicit TrajectoryTracker(State state) : state{ state }, accumulatedCosts{ 0.0 } {}
	};

	TrajectoryTracker GetNewTracker() const
	{
		return TrajectoryTracker(mdp_ptr->InitialState());
	}


	TrajectoryTracker GetRandomTracker(RNG& rng) const
	{
		if constexpr (HasRandomInitialState())
		{
			return TrajectoryTracker(mdp_ptr->InitialState(rng));
		}
		else
		{
			return TrajectoryTracker(mdp_ptr->InitialState());
		}
	}

	TrajectoryTracker GetTracker(const State& state) const
	{
		return TrajectoryTracker(state);
	}

	RandomSourceStatus GetRandomSourceStatus(const TrajectoryTracker& tracker, const RandomSource& randomSource) const
	{
		RandomSourceStatus status{};
		return status;
	}

	double GetAlpha() const
	{
		if constexpr (HasAlpha<MDP>::value)
		{
			return mdp_ptr->alpha;
		}
		else
		{
			return 1.0;
		}
	}

	bool IsFinalState(const State& state) const
	{
		if constexpr (HasFinalState())
		{
			return mdp_ptr->IsFinal(state);
		}
		else
		{
			return false;
		}
	}

	
	bool HasAnotherEvent(const RandomSource& randomSource, const RandomSourceStatus& randomSourceStatus) const
	{
		if constexpr (ExogenousEventDriven())
		{
			return randomSourceStatus < randomSource.size();
		}
		else
		{
			return mdp_ptr->HasAnotherEvent(randomSource, randomSourceStatus);
		}
	}

	bool TrajectoryHasEnded(const TrajectoryTracker& trajectory, const RandomSource& randomSource, const RandomSourceStatus& randomSourceStatus) const
	{
		return IsFinalState(trajectory.state)||((!AwaitsAction(trajectory)) && (!HasAnotherEvent(randomSource, randomSourceStatus)));
	}

	RandomSource GetRandomSource(const TrajectoryTracker& tracker, RNG& rng) const
	{
		if constexpr (ExogenousEventDriven())
		{
			double alpha = GetAlpha();
			if constexpr (!HasNumEventsRemaining())
			{
				if (alpha >= 1.0)
				{
					std::string message = "In MDP, either set alpha to <1.0 or implement NumEventsRemaining(const State& state)!";
					std::cout << message << std::endl;
					throw message;
				}
			}

			auto NumEvents = MaxEventsRemaining(tracker.state);
			if (alpha < 1.0)
			{
				rng.GeomParam(alpha);
				size_t length = rng.NextGeom();
				NumEvents = std::min(NumEvents, length);
			}
			return EventStream<Event>::GetFixedLengthStream(NumEvents, rng, mdp_ptr->EventProbs());
		}
		else
		{
			return mdp_ptr->GetRandomSource(tracker.state, rng);
		}
	}

	const std::vector<double>& EventProbs(const State& state) const
	{
		if constexpr (ExogenousEventDriven())
		{
			return mdp_ptr->EventProbs();
		}
		else
		{
			return mdp_ptr->EventProbs(state);
		}
	}


	void IncorporateEvent(TrajectoryTracker& tracker, const RandomSource& randomSource, RandomSourceStatus& randomSourceStatus) const
	{
		if constexpr (ExogenousEventDriven())
		{
			tracker.accumulatedCosts += mdp_ptr->ModifyStateWithEvent(tracker.state, randomSource[randomSourceStatus++]);
		}
		else
		{
			auto event = mdp_ptr->GetEvent(tracker.state, randomSource, randomSourceStatus);
			tracker.accumulatedCosts += mdp_ptr->ModifyStateWithEvent(tracker.state, event);
		}
	}
	bool ReturnActionIfTrivial(const State& state,size_t& out_action) const
	{
		auto NumValid = NumValidActions(state);
		size_t NumAllowed = 0;
		for (size_t action = 0; action < NumValid; action++)
		{
			if (IsAllowedAction(state, action))
			{
				out_action = action;
				NumAllowed++;
			}
			if (NumAllowed > 1)
			{//More than once action is allowed: do not take action and return false
				return false;
			}
		}
		return true;
	}


	bool TakeActionIfTrivial(TrajectoryTracker& tracker) const
	{
		auto NumValid = NumValidActions(tracker.state);
		size_t NumAllowed{ 0 };
		size_t allowedAction{ 0 };
		for (size_t action = 0; action < NumValid; action++)
		{
			if (IsAllowedAction(tracker.state,action))
			{
				allowedAction = action;
				NumAllowed++;
			}
			if (NumAllowed > 1)
			{//More than once action is allowed: do not take action and return false
				return false;
			}
		}
		IncorporateAction(tracker, allowedAction);
		return true;
	}


	inline bool IncorporateEventsUntilNextNonTrivialAction(const RandomSource& randomSource, RandomSourceStatus& randomSourceStatus, TrajectoryTracker& tracker) const
	{
		while (!TrajectoryHasEnded(tracker, randomSource, randomSourceStatus))
		{
			if (AwaitsAction(tracker))
			{
				if (!TakeActionIfTrivial(tracker))
				{//We didn't take a trivial action, so there is a non-trivial action coming up.. 
					return true;
				}
			}
			else
			{
				IncorporateEvent(tracker, randomSource, randomSourceStatus);
				
			}
		}
		return false;
	}




	std::string Identifier() const
	{
		return mdp_ptr->Identifier();
	}





	void Test(State& state) const
	{
		std::cout << "---------------" << std::endl;
		std::cout << "Testing adapter for " << Identifier() << std::endl;
		std::cout << "number of task types " << NumTaskTypes() << std::endl;
		std::cout << "alpha: " << GetAlpha() << std::endl;
		std::cout << "Has num events remaining: " << HasNumEventsRemaining() << std::endl;
		std::cout << "Num events remaining: " << MaxEventsRemaining(state) << std::endl;
		std::cout << "HasFinalState: " << HasFinalState() << std::endl;
		std::cout << "IsFinalState: " << IsFinalState(state) << std::endl;
		std::cout << "HasRandomInitialState: " << HasRandomInitialState() << std::endl;
		if (AwaitsAction(state))
		{
			size_t task = TaskType(state);
			std::cout << "task: " << task << std::endl;
			std::cout << "Numvalid Actions: " << NumValidActions(state) << std::endl;

		}
		else
		{
			std::cout << "State awaits event" << std::endl;

		}

		std::cout << "Has event override: " << HasEventOverride<MDP>() << std::endl;
		std::cout << "Independent events " << has_EventProbsIndependent<MDP, State>::value << std::endl;
		std::cout << "Dependent events " << has_EventProbsStateDependent<MDP, State>::value << std::endl;
		std::cout << "---------------" << std::endl;

	}
	//MDP should either define state-dependent event probs, or state independent eventprobs:
	static_assert(has_EventProbsIndependent<MDP, State>::value != has_EventProbsStateDependent<MDP, State>::value,"MDP should either define EventProbs() or EventProbs(const State& state). ");

	size_t TaskType(const State& state) const
	{
		if constexpr (NumTaskTypes() > 1)
		{
			return mdp_ptr->TaskType(state);
		}
		else
		{
			return 0;
		}
	}

private:

	std::array<size_t, NumTaskTypes()> offsets;
	size_t totalValidActions{ 0 };

public:

	explicit MDPAdapter(const MDP* mdp_ptr) :mdp_ptr{ mdp_ptr }
	{
		size_t cumulative = 0;
		for (size_t i = 0; i < NumTaskTypes(); i++)
		{
			offsets[i] = cumulative;
			cumulative += NumValidActionsForTask(i);
		}
		totalValidActions = cumulative;
	}

	size_t ActionOffset(const State& state)
	{
		if constexpr (NumTaskTypes() > 1)
		{
			return offsets[TaskType(state)];
		}
		else
		{
			return 0;
		}
	}

	size_t TotalValidActions() const
	{
		return totalValidActions;
	}

	size_t MaxEventsRemaining(const State& state) const
	{
		if constexpr (HasNumEventsRemaining())
		{
			return mdp_ptr->NumEventsRemaining(state);
		}
		else
		{
			return std::numeric_limits<size_t>::max();
		}

	}

	size_t NumValidActions([[maybe_unused]]const State& state) const
	{
		if constexpr (NumTaskTypes() > 1)
		{
			return mdp_ptr->NumValidActions(TaskType(state));
		}
		else
		{
			return mdp_ptr->NumValidActions();
		}
	}


	bool IsAllowedAction(const State& state, size_t action) const
	{//Can add default true if function is not provided later.
		if constexpr (HasIsAllowedAction())
		{
			return mdp_ptr->IsAllowedAction(state, action);
		}
		else
		{
			return true;
		}
	}

	bool AwaitsAction(const State& state) const
	{
		return mdp_ptr->AwaitsAction(state);

	}

	bool AwaitsAction(const TrajectoryTracker& tracker) const
	{
		return mdp_ptr->AwaitsAction(tracker.state);
	}

	
	//Obtains a transformed action.
	void IncorporateAction(TrajectoryTracker& tracker, size_t action) const
	{
		//This should transform the action based on the state.. 
		tracker.accumulatedCosts += mdp_ptr->ModifyStateWithAction(tracker.state, action);
	}

	size_t NumFeatures(size_t taskType = 0) const
	{
		//No support for heterogenous feature set yet. Would need multiple neural nets to deal
		//with that. 
		//Could implement this in the future by allowing syntax like this:
		//mdp_ptr->NumFeatures(taskType);, possibly together with a flag indicating that the MDP has the feature sets. 
		return mdp_ptr->NumFeatures();
	}

	std::vector<float> GetFeatures(const State& state) const
	{
		auto NumFeats = NumFeatures(TaskType(state));
		std::vector<float> Features(NumFeats);
		ExtractFeatures(state, Features.data());
		return Features;
	}

	std::vector<size_t> GetAllowedActions(const TrajectoryTracker& tracker) const
	{
		return GetAllowedActions(tracker.state);
	}

	std::vector<size_t> GetAllowedActions(const State& state) const
	{
		auto NumValid = NumValidActions(state);
		std::vector<size_t> AllowedActions{};
		AllowedActions.reserve(NumValid);
		for (size_t action = 0; action < NumValid; action++)
		{
			if (IsAllowedAction(state, action))
			{
				AllowedActions.push_back(action);
			}
		}
		return AllowedActions;
	}


	void ExtractFeatures(const State& state, float* feats_out) const
	{
		mdp_ptr->ExtractFeatures(state, feats_out);
	}
private:

	size_t NumValidActionsForTask(size_t TaskType) const
	{
		if constexpr (NumTaskTypes() > 1)
		{
			return mdp_ptr->NumValidActions(TaskType);
		}
		else
		{
			return mdp_ptr->NumValidActions();
		}
	}
};