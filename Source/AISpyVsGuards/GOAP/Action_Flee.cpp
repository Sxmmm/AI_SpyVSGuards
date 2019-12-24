#include "GOAP/Action_Flee.h"
#include "Agents/CPP_SpyAgent.h"
#include "Agents/CPP_GuardAgent.h"
#include "Agents/CPP_GOAP.h"
#include "AIController.h"

#include "NavigationSystem.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

Action_Flee::Action_Flee()
{
	////Players State preconditions for this action
	AddPreCondition("Spotted", true);

	//How this action will affect the Players state
	AddEffect("Spotted", false);

	m_sActionName = "Flee From Guards";
	m_fCost = 0.1f;//Figurative cost of performing this action.
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
	UE_LOG(LogTemp, Warning, TEXT("Checking FLEE"));
	TArray<AActor*> aFoundAgents;
	UGameplayStatics::GetAllActorsOfClass(a_paAIAgent->GetWorld(), ACPP_GuardAgent::StaticClass(), aFoundAgents);
	float fClosestDistance = 100000.0f;
	ACPP_GOAP* GOAPSpotted = Cast<ACPP_GOAP>(a_paAIAgent);
	for (AActor* a_Actor : aFoundAgents)
	{
		if (a_Actor)
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
			}
		}
		//FVector Target = (a_paAIAgent->GetActorLocation() - a_Actor->GetActorLocation());
		ACPP_GOAP* pAIAgent = Cast<ACPP_GOAP>(a_paAIAgent);
		AAIController* pAIController = Cast<AAIController>(a_paAIAgent);
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(pAIAgent->GetWorld());
		FVector FleePosition = a_Actor->GetActorLocation() - a_paAIAgent->GetActorLocation() - a_paAIAgent->GetActorLocation();
		FleePosition.Normalize();
		m_vTargetLocation = NavSys->GetRandomReachablePointInRadius(pAIAgent->GetWorld(), FleePosition, FVector::Dist(a_paAIAgent->GetActorLocation(), a_Actor->GetActorLocation()));
		//FVector::Dist(a_paAIAgent->GetActorLocation(), a_Actor->GetActorLocation())
		
		//m_vTargetLocation = FVector(a_paAIAgent->GetActorLocation() - a_Actor->GetActorLocation()).Normalize;
		//m_vTargetLocation = FVector(-500.0f, 1700.0f, 253.0f);
	}
	return true;
}

bool Action_Flee::PerformAction(AActor * a_paAIAgent)
{
	UE_LOG(LogTemp, Warning, TEXT("DONE FLEE!!!"));
	Cast<ACPP_GOAP>(a_paAIAgent)->SetHasSpotted(false);
	Cast<ACPP_GOAP>(a_paAIAgent)->InterruptBehaviour();
	m_bPerformingAction = true;
	return true;
}

bool Action_Flee::NeedsToBeInRange()
{
	return m_bRequiresInRange;
}