#pragma once

#include <memory>
#include <stack>


class GameState;
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

	bool m_isRemoving;
	bool m_isAdding;
	bool m_isReplacing;
};
