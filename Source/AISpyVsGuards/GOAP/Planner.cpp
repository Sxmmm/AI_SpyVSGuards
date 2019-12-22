#include "GOAP/Planner.h"
#include "GOAP/Action.h"
#include "GOAP/Node.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

Planner::Planner()
{
}

Planner::~Planner()
{
}

void Planner::CreateActionPlan(AActor * a_pAIAgent, TSet<Action*> a_sAvailableActions, TSet<TPair<FString, bool>>& a_smWorldState, TSet<TPair<FString, bool>>& a_smGoalState, TQueue<Action*>* a_qActionPlan)
{
	for (Action* pAction : a_sAvailableActions)
	{
		pAction->ResetAction();
	}

	TSet<Action*> sUsableActions;
	for (Action* pAction : a_sAvailableActions)
	{
		if (pAction->CheckPreCondition(a_pAIAgent))
		{
			sUsableActions.Add(pAction);
		}
	}

	TArray<Node*> nLeaves;
	Node* nStartNode = new Node(nullptr, 0, a_smWorldState, nullptr);
	bool bSuccessfulTree = BuildGOAPPlan(nStartNode, nLeaves, sUsableActions, a_smGoalState);

	if (!bSuccessfulTree)
	{
		return;
	}

	Node* nCheapestPlan = nullptr;
	for (Node* nNode : nLeaves)
	{
		if (nCheapestPlan == nullptr)
		{
			nCheapestPlan = nNode;
		}
		else
		{
			if (nNode->m_fCost < nCheapestPlan->m_fCost)
			{
				nCheapestPlan = nNode;
			}
		}
	}

	TSet<Action*> lFinishedPlan = TSet<Action*>();
	while (nCheapestPlan)
	{
		if (nCheapestPlan->m_Action)
		{
			lFinishedPlan.Add(nCheapestPlan->m_Action);
		}
		if (nCheapestPlan->m_nParentNode == nullptr)
		{
			nCheapestPlan = nullptr;
		}
		else
		{
			nCheapestPlan = nCheapestPlan->m_nParentNode;
		}
	}
	TArray<Action*> lFinishedPlanB = lFinishedPlan.Array();

	Algo::Reverse(lFinishedPlanB);

	a_qActionPlan->Empty();

	for (Action* pAction : lFinishedPlanB)
	{
		a_qActionPlan->Enqueue(pAction);//Queue the new action.
	}

	return;
}

bool Planner::BuildGOAPPlan(Node* a_nParent, TArray<Node*>& a_nNodeList, TSet<Action*> a_sUsableActions, TSet<TPair<FString, bool>> a_smGoalState)
{
	bool bFoundSuccessfulPath = false;
	for (Action* pAction : a_sUsableActions)//Loop through all of the actions that we can actually currently perform
	{
		if (MatchesPlayersCurrentState(pAction->GetPreConditions(), a_nParent->m_smState))//Check that the current action can actually be performed by the actor, by comparing the preconditions of the action to the players current state.
		{
			TSet<TPair<FString, bool>> smCurrentState = RefreshPlayersState(a_nParent->m_smState, pAction->GetEffects());//Create a new state, formed from the players state, but updated with the Effects of the action we just checked.
			Node* nNewNode = new Node(a_nParent, (a_nParent->m_fCost + pAction->m_fCost), smCurrentState, pAction);//Create a new node with the newly calculated values.

			if (GoalMatchesState(a_smGoalState, smCurrentState))//Check if any of the goals can be satisfied by the new updated state.
			{
				a_nNodeList.Add(nNewNode);
				bFoundSuccessfulPath = true;

			}
			else
			{
				TSet<Action*> sChildBranches = GatherNewActions(a_sUsableActions, pAction);
				bool bFoundSecondPath = BuildGOAPPlan(nNewNode, a_nNodeList, sChildBranches, a_smGoalState);
				if (bFoundSecondPath)
				{
					bFoundSuccessfulPath = true;
				}
			}
		}
	}
	return bFoundSuccessfulPath;
}

bool Planner::GoalMatchesState(TSet<TPair<FString, bool>> a_smGoal, TSet<TPair<FString, bool>> a_smState)
{
	bool bIsInState = false;
	for (TPair<FString, bool> mGoal : a_smGoal)
	{
		for (TPair<FString, bool> mState : a_smState)
		{
			if (mGoal.Key == mState.Key)
			{
				if (mGoal.Value == mState.Value)
				{
					bIsInState = true;
					break;
				}
			}
		}
	}
	return bIsInState;
}

bool Planner::MatchesPlayersCurrentState(TSet<TPair<FString, bool>> a_smPreConditions, TSet<TPair<FString, bool>> a_smState)
{
	bool bAllPreconditionsMet = true;
	bool bPreConditionMet = false;
	for (TPair<FString, bool> mPreCons : a_smPreConditions)
	{
		for (TPair<FString, bool> mState : a_smState)
		{
			if (mPreCons.Key == mState.Key)
			{
				if (mPreCons.Value == mState.Value)
				{
					bPreConditionMet = true;
				}
			}
		}
		if (bPreConditionMet == false)
		{
			bAllPreconditionsMet = false;
		}
	}
	return bAllPreconditionsMet;
}

TSet<Action*> Planner::GatherNewActions(TSet<Action*> a_sActions, Action* a_ExcludedAction)
{
	TSet<Action*> sBranches = TSet<Action*>();
	for (Action* pAction : a_sActions)
	{
		if (pAction != a_ExcludedAction)
		{
			sBranches.Add(pAction);
		}
	}
	return sBranches;
}

TSet<TPair<FString, bool>> Planner::RefreshPlayersState(TSet<TPair<FString, bool>> a_smCurrentState, TSet<TPair<FString, bool>> a_smStateChange)
{
	TSet<TPair<FString, bool>> smNewState = TSet<TPair<FString, bool>>();

	TPair<FString, bool> mPairToRemove;

	for (TPair<FString, bool > mState : a_smCurrentState)
	{
		smNewState.Add(TPair<FString, bool>(mState.Key, mState.Value));
	}

	for (TPair<FString, bool> mChange : a_smStateChange)
	{
		bool bKeyExists = false;

		for (TPair<FString, bool> mState : smNewState)
		{
			if (mChange.Key == mState.Key)
			{
				bKeyExists = true;
				mPairToRemove = mState;
				break;
			}
		}

		if (bKeyExists)
		{
			smNewState.Remove(mPairToRemove);
			TPair<FString, bool> mUpdatedPair = TPair<FString, bool>(mChange.Key, mChange.Value);
			smNewState.Add(mUpdatedPair);
		}
		else
		{
			smNewState.Add(TPair<FString, bool>(mChange.Key, mChange.Value));
		}
	}
	return smNewState;
}