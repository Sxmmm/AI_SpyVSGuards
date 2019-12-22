// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_SpyAgent.h"

#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"

ACPP_SpyAgent::ACPP_SpyAgent()
{
	LoadAvailableActions();
}

//void ACPP_SpyAgent::LoadAvailableActions()
//{
//	//Add all of the actions that the actor can use to a list.
//	m_sAvailableActions.Add(new Action_KillSelf);
//	m_sAvailableActions.Add(new Action_BiteActor);
//	m_sAvailableActions.Add(new Action_Mutate);
//	m_sAvailableActions.Add(new Action_Wander);
//}

TSet<TPair<FString, bool>> ACPP_SpyAgent::GetPlayersCurrentState()
{
	//Create a list of Player states that tell the GOAP planner what actions it can perform
	//Used to compare against an actions pre conditions.
	TSet<TPair<FString, bool>> PlayerStateData = TSet<TPair<FString, bool>>();
	PlayerStateData.Add(TPair<FString, bool>("Spotted", HaveIBeenSpotted()));
	PlayerStateData.Add(TPair<FString, bool>("GuardsNear", AreGuardsNear()));
	PlayerStateData.Add(TPair<FString, bool>("HaveKey", DoIHaveTheKey()));
	PlayerStateData.Add(TPair<FString, bool>("AboutToCatch", IsGuardAboutToCatchMe()));
	return PlayerStateData;
}

bool ACPP_SpyAgent::MoveAgentToAction(GOAP_Action * a_NextAction)
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

bool ACPP_SpyAgent::MoveAgentToAction(GOAP_Action * a_NextAction, bool a_bVector)
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