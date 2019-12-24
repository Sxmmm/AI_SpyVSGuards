#pragma once

#include "CoreMinimal.h"
#include "Agents/CPP_SpyAgent.h"
#include "GOAP/Action.h"
#include "CPP_GOAP.generated.h"

UCLASS()
class AISPYVSGUARDS_API ACPP_GOAP : public ACPP_SpyAgent
{
	GENERATED_BODY()
public:
	ACPP_GOAP();

	void LoadAvailableActions();

	TSet<TPair<FString, bool>> GetPlayersCurrentState();

	TSet<TPair<FString, bool>> CreateGoalState();

	void PlanFailed(TSet<TPair<FString, bool>> a_kvpFailedGoal);

	void PlanFound(TSet<TPair<FString, bool>> a_kvpGoal, TQueue<Action*> a_qActionQueue);

	void AllActionsFinished();

	void AbortPlan(Action* a_FailedAction);

	bool MoveAgentToAction(Action* a_NextAction);

	bool MoveAgentToAction(Action* a_NextAction, bool a_bVector);

	TSet<Action*> GetActionList();

	bool GetHasSpotted();
	void SetHasSpotted(bool a_bSpotted);
	bool GetHaveKey();
	void SetHaveKey(bool a_bTrue);

	float m_fArrivalDistance = 100.0f;

private:
	UPROPERTY(EditAnywhere)
		bool m_bSpotted = false;

	bool m_bHasKey = false;

	TSet<Action*> m_sAvailableActions;
};
