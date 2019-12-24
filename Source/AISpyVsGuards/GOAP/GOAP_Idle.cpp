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
	m_ActionPlanner = new Planner();//Create a new planner

	TSet<TPair<FString, bool>> WorldState = Cast<ACPP_GOAP>(pOwner)->GetPlayersCurrentState();//Get the players current state
	TSet<TPair<FString, bool>> GoalState = Cast<ACPP_GOAP>(pOwner)->CreateGoalState();//Get the players current goals.

	TSet<Action*> sActionList = Cast<ACPP_GOAP>(pOwner)->GetActionList();//Get the players available actions

	m_ActionPlanner->CreateActionPlan(pOwner, sActionList, WorldState, GoalState, &ActionPlan);//Create an actionplan
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
	if (!ActionPlan.IsEmpty())//If there actually is a plan.
	{
		Action* pOutAction;
		ActionPlan.Peek(pOutAction);//Get the first action from the plan
		return new GOAP_Move(GetOwner(), pOutAction);//Start moving to the action
	}
	return new GOAP_Idle(GetOwner());//There is no plan so we need to replan.

	//TO DO - Set replan to a timer so its not running every single frame.
}