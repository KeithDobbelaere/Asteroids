#pragma once

#include "GameState.h"

#include <memory>
#include <stack>


using StatePtr = std::unique_ptr<GameState>;

class StateMachine
{
public:
	StateMachine() = default;
	~StateMachine() = default;


	void addState(StatePtr newState, bool isReplacing = true);
	void removeState();

	void processStateChanges();

	StatePtr& getActiveState();

private:
	std::stack<StatePtr> m_states;
	StatePtr m_newState;

	bool m_isRemoving = false;
	bool m_isAdding = false;
	bool m_isReplacing = false;
};
