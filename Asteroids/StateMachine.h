#pragma once

#include "GameState.h"

#include <memory>
#include <stack>


using StateRef = std::unique_ptr<GameState>;

class StateMachine
{
public:
	StateMachine() = default;
	~StateMachine() = default;


	void addState(StateRef newState, bool isReplacing = true);
	void removeState();

	void processStateChanges();

	StateRef& getActiveState();

private:
	std::stack<StateRef> m_states;
	StateRef m_newState;

	bool m_isRemoving = false;
	bool m_isAdding = false;
	bool m_isReplacing = false;
};
