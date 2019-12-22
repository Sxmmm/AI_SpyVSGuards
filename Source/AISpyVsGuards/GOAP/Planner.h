#pragma once
#include "Queue.h"
#include "Map.h"
#include "Set.h"
#include "List.h"
#include "CoreMinimal.h"

class Node;
class AActor;
class Action;

class Planner
{
public:
	Planner();
	~Planner();
	TQueue<Action*>* m_qActionQueue;
	TQueue<Action*> m_qQueue;

	void CreateActionPlan(AActor* a_pAIAgent, TSet<Action*> a_sAvailableActions, TSet<TPair<FString, bool>>& a_smWorldState, TSet<TPair<FString, bool>>& a_smGoalState, TQueue<Action*>* a_qActionPlan);

private:

	bool BuildGOAPPlan(Node* a_nParent, TArray<Node*>& a_nNodeList, TSet<Action*> a_sUsableActions, TSet<TPair<FString, bool>> a_smGoalState);

	bool GoalMatchesState(TSet<TPair<FString, bool>> a_smGoal, TSet<TPair<FString, bool>> a_smState);

	bool MatchesPlayersCurrentState(TSet<TPair<FString, bool>> a_smPreConditions, TSet<TPair<FString, bool>> a_smState);

	TSet<Action*> GatherNewActions(TSet<Action*> a_sActions, Action* a_ExcludedAction);

	TSet<TPair<FString, bool>> RefreshPlayersState(TSet<TPair<FString, bool>> a_smCurrentState, TSet<TPair<FString, bool>> a_smStateChange);
};