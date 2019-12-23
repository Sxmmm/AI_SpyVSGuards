#include "GOAP/Action_Flee.h"
#include "Agents/CPP_SpyAgent.h"
#include "Agents/CPP_GuardAgent.h"
#include "Agents/CPP_GOAP.h"
#include "AIController.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

Action_Flee::Action_Flee()
{
	////Players State preconditions for this action
	AddPreCondition("Spotted", true);

	//How this action will affect the Players state
	AddEffect("Spotted", false);

	m_sActionName = "Flee From Guards";
	m_fCost = 1.0f;//Figurative cost of performing this action.
	m_bRequiresInRange = true;
}

Action_Flee::~Action_Flee()
{
}

void Action_Flee::ResetGA()
{
	m_bPerformingAction = false;
	m_paTarget = nullptr;
}

bool Action_Flee::IsActionFinished()
{
	return m_bPerformingAction;
}

bool Action_Flee::CheckPreCondition(AActor * a_paAIAgent)
{
	TArray<AActor*> aFoundAgents;//To store list of all agents
	UGameplayStatics::GetAllActorsOfClass(a_paAIAgent->GetWorld(), ACPP_GuardAgent::StaticClass(), aFoundAgents);
	float fClosestDistance = 100000.0f;//Max distance to compare to

	for (AActor* a_Actor : aFoundAgents)//Loop through actor list
	{
		if (a_Actor)//Null check
		{
			float fThisDistance = FVector::Dist(a_paAIAgent->GetActorLocation(), a_Actor->GetActorLocation());//Get distance between two actors
			if (fThisDistance <= 0.0f)//If the target is me, temp fix for broken check above.
			{
				continue;
			}
			if (fThisDistance <= fClosestDistance)//If closer than last actor
			{
				//m_paTarget = a_Actor;//Set new target
				fClosestDistance = fThisDistance;//New closest distance
				m_vTargetLocation = FVector(1.0f, 1.0f, 1.0f);
			}
		}
	}
	return true;
}

bool Action_Flee::PerformAction(AActor * a_paAIAgent)
{
	Cast<ACPP_GOAP>(a_paAIAgent)->SetHasSpotted(false);
	m_bPerformingAction = true;
	return true;
}

bool Action_Flee::NeedsToBeInRange()
{
	return m_bRequiresInRange;
}