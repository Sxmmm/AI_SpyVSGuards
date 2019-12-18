#include "Attack.h"
#include "Agents/CPP_GuardAgent.h"
#include "AIController.h"
#include "StateMachine/Patrol.h"

Attack::Attack(ACPP_GuardAgent* pOwner) : Behaviour(pOwner)
{
	m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
}

void Attack::Update()
{
	switch (m_eCurrentAttackState)
	{
	case MOVE_TO_SPY_POSITION:
	{
		//GET SPY POSITION
		//MOVE TO SPY POSITION
		//IF SPY IS OUT OF SENSORY RANGE
		m_eCurrentAttackState = MOVE_TO_SPY_LAST_KNOWN_POSITION;
		//IF SPY IS IN REACH
		m_eCurrentAttackState = CAPTURE_SPY;
		break;
	}
	case MOVE_TO_SPY_LAST_KNOWN_POSITION:
	{
		//IF SPY IS NOT SEEN
		m_eCurrentAttackState = LOOK_AROUND_FOR_SPY;
		//IF SPY IS IN REACH
		m_eCurrentAttackState = CAPTURE_SPY;
		break;
	}
	case LOOK_AROUND_FOR_SPY:
	{
		//IF SPY IS FOUND
		m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
		//IF SPY IS IN REACH
		m_eCurrentAttackState = CAPTURE_SPY;
		break;
	}
	case CAPTURE_SPY:
	{
		//IF SPY HAS BEEN CAPTURED RESTART THE GAME
		break;
	}
	}
}

Behaviour* Attack::CheckConditions()
{
	//Check if spy cannot be found, resume patrol
	if (!GetOwner()->GetHasSpottedStatus())
	{
		return new Patrol(GetOwner());
	}
	return nullptr;
}