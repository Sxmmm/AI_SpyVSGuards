#include "GOAP/GOAP_Idle.h"
#include "GOAP/GOAP_Move.h"
#include "Agents/CPP_GOAP.h"
#include "GOAP/Planner.h"
#include "Agents/CPP_SpyAgent.h"
#include "AIController.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

GOAP_Idle::GOAP_Idle(ACPP_SpyAgent* pOwner) : GOAP_Behaviour(pOwner)
{
	//Creating a new planner
	m_ActionPlanner = new Planner();

	//Get the ai state and current goals
	TSet<TPair<FString, bool>> WorldState = Cast<ACPP_GOAP>(pOwner)->GetPlayersCurrentState();
	TSet<TPair<FString, bool>> GoalState = Cast<ACPP_GOAP>(pOwner)->CreateGoalState();

	//Get available actions
	TSet<Action*> sActionList = Cast<ACPP_GOAP>(pOwner)->GetActionList();

	//Create a plan
	m_ActionPlanner->CreateActionPlan(pOwner, sActionList, WorldState, GoalState, &ActionPlan);
}

GOAP_Idle::~GOAP_Idle()
{
	if (m_ActionPlanner)
	{
		delete m_ActionPlanner;
	}
}

void GOAP_Idle::Update()
{
}

GOAP_Behaviour* GOAP_Idle::CheckConditions()
{
	//If there is a plan go through the actions
	if (!ActionPlan.IsEmpty())
	{
		Action* pOutAction;
		ActionPlan.Peek(pOutAction);
		return new GOAP_Move(GetOwner(), pOutAction);
	}
	return new GOAP_Idle(GetOwner());
}