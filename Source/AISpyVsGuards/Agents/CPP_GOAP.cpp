#include "Agents/CPP_GOAP.h"
#include "GOAP/Action.h"
#include "Agents/CPP_SpyAgent.h"
#include "AIController.h"
#include "GOAP/GOAP_Idle.h"

// Actions 
#include "GOAP/Action_GetKey.h"
#include "GOAP/Action_Exit.h"
#include "GOAP/Action_Flee.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

ACPP_GOAP::ACPP_GOAP()
{
	LoadAvailableActions();
}

void ACPP_GOAP::LoadAvailableActions()
{
	//Add all of the actions that the actor can use to a list.
	m_sAvailableActions.Add(new Action_GetKey);
	m_sAvailableActions.Add(new Action_Exit);
	m_sAvailableActions.Add(new Action_Flee);
}

TSet<TPair<FString, bool>> ACPP_GOAP::GetPlayersCurrentState()
{
	//Create a list of Player states that tell the GOAP planner what actions it can perform
	//Used to compare against an actions pre conditions.
	TSet<TPair<FString, bool>> PlayerStateData = TSet<TPair<FString, bool>>();
	PlayerStateData.Add(TPair<FString, bool>("Spotted", GetHasSpotted()));
	PlayerStateData.Add(TPair<FString, bool>("HasKey", GetHaveKey()));
	return PlayerStateData;
}

TSet<TPair<FString, bool>> ACPP_GOAP::CreateGoalState()
{
	//Create a goal state
	//You can have multiple goals.
	TSet<TPair<FString, bool>> GoalData = TSet<TPair<FString, bool>>();

	GoalData.Add(TPair<FString, bool>("Exit", true));
	GoalData.Add(TPair<FString, bool>("GetKey", true));
	GoalData.Add(TPair<FString, bool>("Spotted", false));

	return GoalData;
}

void ACPP_GOAP::PlanFailed(TSet<TPair<FString, bool>> a_kvpFailedGoal)
{
}

void ACPP_GOAP::PlanFound(TSet<TPair<FString, bool>> a_kvpGoal, TQueue<Action*> a_qActionQueue)
{
}

void ACPP_GOAP::AllActionsFinished()
{
}

void ACPP_GOAP::AbortPlan(Action* a_FailedAction)
{
}

bool ACPP_GOAP::MoveAgentToAction(Action* a_NextAction)
{
	if (a_NextAction != nullptr)//Null check
	{
		AAIController* pAIController = Cast<AAIController>(GetController());
		if (pAIController)
		{
			float fDistance = FVector::Dist(GetActorLocation(), a_NextAction->m_paTarget->GetActorLocation());//Get distance between Actor and target

			UAIBlueprintHelperLibrary::SimpleMoveToActor(pAIController, a_NextAction->m_paTarget);//Move to target actor

			if (fDistance <= m_fArrivalDistance)//If we are close enough
			{
				return true;//We have arrived
			}
		}
	}
	return false;
}

bool ACPP_GOAP::MoveAgentToAction(Action* a_NextAction, bool a_bVector)
{
	if (a_NextAction != nullptr)
	{
		AAIController* pAIController = Cast<AAIController>(GetController());
		if (pAIController)
		{
			float fDistance = FVector::Dist(GetActorLocation(), a_NextAction->m_vTargetLocation);//Get distance between actor and target.

			UAIBlueprintHelperLibrary::SimpleMoveToLocation(pAIController, a_NextAction->m_vTargetLocation);//Move there
			if (fDistance <= m_fArrivalDistance)//Have we arrived?
			{
				return true;
			}
		}
	}
	return false;
}

TSet<Action*> ACPP_GOAP::GetActionList()
{
	return m_sAvailableActions;
}

bool ACPP_GOAP::GetHasSpotted()
{
	return m_bSpotted;
}

void ACPP_GOAP::SetHasSpotted(bool a_bSpotted)
{
	m_bSpotted = a_bSpotted;
}

bool ACPP_GOAP::GetHaveKey()
{
	return m_bHasKey;
}

void ACPP_GOAP::SetHaveKey(bool a_bHasKey)
{
	m_bHasKey = a_bHasKey;
}
