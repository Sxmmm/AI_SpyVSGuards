#include "GOAP/GOAP_PerformAction.h"

#include "GOAP/GOAP_Idle.h"
#include "Agents/CPP_GOAP.h"
#include "GOAP/Action.h"

#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

//On init perform action
GOAP_PerformAction::GOAP_PerformAction(ACPP_SpyAgent* pOwner, Action* a_pAction) : GOAP_Behaviour(pOwner)
{
	m_pAction = a_pAction;
	m_pAction->PerformAction(pOwner);
}

void GOAP_PerformAction::Update()
{
}

GOAP_Behaviour* GOAP_PerformAction::CheckConditions()
{
	//Check to see if action has been performed
	if (m_pAction->IsActionFinished())
	{
		//Return to planner for next action
		return new GOAP_Idle(GetOwner());
	}
	return nullptr;
}