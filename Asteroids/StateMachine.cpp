#include "StateMachine.h"

#include "Defines.h"
#include "GameState.h"


void StateMachine::cleanup()
{
	while (!m_states.empty()) {
		auto& state = m_states.top();
		state->cleanup();
		m_states.pop();
	}
}

void StateMachine::addState(StatePtr newState)
{
	m_isAdding = true;
	m_isReplacing = false;

	m_newState = std::move(newState);
}

void StateMachine::replaceState(StatePtr newState)
{
	m_isAdding = true;
	m_isReplacing = true;

	m_newState = std::move(newState);
}

void StateMachine::removeState()
{
	m_isRemoving = true;
}

void StateMachine::processStateChanges()
{
	if (m_isRemoving && !m_states.empty())
	{
		m_states.top()->cleanup();
		m_states.pop();

		if (!m_states.empty())
		{
			this->m_states.top()->resume();
		}
		m_isRemoving = false;
	}
	if (m_isAdding)
	{
		if (!m_states.empty())
		{
			if (m_isReplacing)
			{
				m_states.top()->cleanup();
				m_states.pop();
			}
			else
			{
				m_states.top()->pause();
			}
		}
		m_states.push(std::move(m_newState));
		m_states.top()->init();
		m_isAdding = false;
	}
}

StatePtr& StateMachine::getActiveState()
{
	return m_states.top();
}
