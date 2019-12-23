#include "GOAP/GOAP_Move.h"
#include "GOAP/GOAP_PerformAction.h"
#include "GOAP/GOAP_Idle.h"
#include "GOAP/Action.h"

GOAP_Move::GOAP_Move(ACPP_SpyAgent* pOwner, Action* a_ActionToGoTo) : GOAP_Behaviour(pOwner)
{
	m_pCurrentAction = a_ActionToGoTo;
}

void GOAP_Move::Update()
{
}

GOAP_Behaviour* GOAP_Move::CheckConditions()
{
	//Moving to an Actions FVector location rather than its target.
	//Used in the Wander action.
	if (!m_pCurrentAction->m_paTarget && m_pCurrentAction->NeedsToBeInRange())//Actor needs to go somewhere but there is no target
	{
		if (m_pCurrentAction->m_vTargetLocation != FVector::ZeroVector)//If the Target location variable is not empty.
		{
			ACPP_GOAP* pAIAgent = Cast<ACPP_GOAP>(GetOwner());
			if (pAIAgent)
			{
				if (pAIAgent->MoveAgentToAction(m_pCurrentAction, true) == true)//If the actor has reached its destination
				{
					return new GOAP_PerformAction(GetOwner(), m_pCurrentAction);//We can now perform the action.
				}
			}
		}
		else//Catch case for unsetup Action, needs to replan.
		{
			return new GOAP_Idle(GetOwner());
		}
	}

	//Moving to an actions target actor.
	else if (m_pCurrentAction->m_paTarget && m_pCurrentAction->NeedsToBeInRange() == true)//Actor needs to go to target
	{
		ACPP_GOAP* pAIAgent = Cast<ACPP_GOAP>(GetOwner());
		if (pAIAgent)
		{
			if (pAIAgent->MoveAgentToAction(m_pCurrentAction))//If actor has reached its destination.
			{
				return new GOAP_PerformAction(GetOwner(), m_pCurrentAction);//We can now perform the action.
			}
		}
	}
	else if (m_pCurrentAction->NeedsToBeInRange() == false)//If we do not need to be in range of the target to perform the action.
	{
		return new GOAP_PerformAction(GetOwner(), m_pCurrentAction);//Perform it
	}

	return nullptr;//Replan

	//To do - Perform action anyway with error catcher, otherwise actors get stuck.
}