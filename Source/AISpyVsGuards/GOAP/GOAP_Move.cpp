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
	//Moving to an actions fvector rather than an actor 
	if (!m_pCurrentAction->m_paTarget && m_pCurrentAction->NeedsToBeInRange())
	{
		//If the target does not eqaul 0,0,0
		if (m_pCurrentAction->m_vTargetLocation != FVector::ZeroVector)
		{
			ACPP_GOAP* pAIAgent = Cast<ACPP_GOAP>(GetOwner());
			if (pAIAgent)
			{
				//Reached destination
				if (pAIAgent->MoveAgentToAction(m_pCurrentAction, true) == true)
				{
					m_pCurrentAction->m_vTargetLocation = FVector::ZeroVector;
					//Perform action
					return new GOAP_PerformAction(GetOwner(), m_pCurrentAction);
				}
			}
		}
		else
		{
			return new GOAP_Idle(GetOwner());
		}
	}

	//Moving to an actions target actor.
	else if (m_pCurrentAction->m_paTarget && m_pCurrentAction->NeedsToBeInRange() == true)
	{
		ACPP_GOAP* pAIAgent = Cast<ACPP_GOAP>(GetOwner());
		if (pAIAgent)
		{
			//Reached destination
			if (pAIAgent->MoveAgentToAction(m_pCurrentAction))
			{
				//Perform action
				return new GOAP_PerformAction(GetOwner(), m_pCurrentAction);
			}
		}
	}
	//Do not need to be in range
	else if (m_pCurrentAction->NeedsToBeInRange() == false)
	{
		//Perform action
		return new GOAP_PerformAction(GetOwner(), m_pCurrentAction);
	}

	return nullptr;
}