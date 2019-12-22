// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//KEEP THIS INCLUDE LAST
#include "CPP_SpyAgent.generated.h"

UCLASS()
class AISPYVSGUARDS_API ACPP_SpyAgent : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_SpyAgent();

	void LoadAvailableActions();

	TSet<TPair<FString, bool>> GetPlayersCurrentState();

	TSet<TPair<FString, bool>> CreateGoalState();

	void PlanFailed(TSet<TPair<FString, bool>> a_kvpFailedGoal);
	//Need GOAP_Action first
	//void PlanFound(TSet<TPair<FString, bool>> a_kvpGoal, TQueue<GOAP_Action*> a_qActionQueue);

	void AllActionsFinished();

	//Need GOAP_Action first
	//void AbortPlan(GOAP_Action* a_FailedAction);

	//bool MoveAgentToAction(GOAP_Action* a_NextAction);

	//bool MoveAgentToAction(GOAP_Action* a_NextAction, bool a_bVector);

	//TSet<GOAP_Action*> GetActionList();

	bool HaveIBeenSpotted();
	void SetHaveIBeenSpotted(bool a_bSpotted);
	bool AreGuardsNear();
	void SetAreGuardsNear(bool a_bGuardsNear);
	bool DoIHaveTheKey();
	void SetDoIHaveTheKey(bool a_bHaveKey);
	bool IsGuardAboutToCatchMe();
	void SetIsGuardAboutToCatchMe(bool a_bAboutToCatch);

private:	
	bool m_bHaveBeenSpotted = false;
	bool m_bAreGuardsNear = false;
	bool m_bDoIHaveTheKey = false;
	bool m_bIsGuardAboutToCatchMe = false;

	//TSet<GOAP_Action*> m_sAvailableActions;
};
