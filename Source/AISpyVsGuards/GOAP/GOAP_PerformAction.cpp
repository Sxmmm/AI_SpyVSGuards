#include "GOAP/GOAP_PerformAction.h"

#include "GOAP/GOAP_Idle.h"
#include "Agents/CPP_GOAP.h"
#include "GOAP/Action.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

GOAP_PerformAction::GOAP_PerformAction(ACPP_SpyAgent* pOwner, Action* a_pAction) : GOAP_Behaviour(pOwner)
{
	m_pAction = a_pAction;
	m_pAction->PerformAction(pOwner);//On initialisation, perform the action.
}

void GOAP_PerformAction::Update()
{
}

GOAP_Behaviour* GOAP_PerformAction::CheckConditions()
{
	if (m_pAction->IsActionFinished())//Check if the action has been performed
	{
		return new GOAP_Idle(GetOwner());//Go back to planner for next action
		//To Do - Make Idle state permanent and so we don't have to keep replanning.
	}
	return nullptr;
}