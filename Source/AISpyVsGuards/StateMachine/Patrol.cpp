#include "Patrol.h"
#include "Agents/CPP_GuardAgent.h"
#include "AIController.h"
#include "StateMachine/Attack.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/AI/NavigationSystemBase.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/NavigationSystem/Public/AbstractNavData.h"



Patrol::Patrol(ACPP_GuardAgent* pOwner) : Behaviour(pOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("PATROL"));
	m_eCurrentPatrolState = FIND_NEXT_POSITION;
}

void Patrol::Update()
{
	switch (m_eCurrentPatrolState)
	{
	case FIND_NEXT_POSITION:
	{
		//Find New Position
		AAIController* pAIController = Cast<AAIController>(GetOwner()->GetController());

		if (pAIController)
		{
			//Assign the navigation mesh, pick a random position and move to it
			UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetOwner()->GetWorld());
			FVector vRandPos = NavSystem->GetRandomPointInNavigableRadius(GetOwner()->GetWorld(), pAIController->GetNavAgentLocation(), 20000.0f);
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(pAIController, vRandPos);

			m_eCurrentPatrolState = MOVE_TO_POSITION;
		}
		break;
	}
	case MOVE_TO_POSITION:
	{
		//Move To Position
		AAIController* pAIController = Cast<AAIController>(GetOwner()->GetController());
		if (pAIController)
		{
			//If at the random position
			if (pAIController->GetMoveStatus() == EPathFollowingStatus::Idle)
			{
				m_eCurrentPatrolState = FIND_NEXT_POSITION;
			}

			if (pAIController->GetMoveStatus() == EPathFollowingStatus::Waiting)
			{
				m_eCurrentPatrolState = FIND_NEXT_POSITION;
			}
		}
		break;
	}
	}
}

Behaviour* Patrol::CheckConditions()
{
	//Has the guard spotted the spy
	if (GetOwner()->GetHasSpottedStatus())
	{
		return new Attack(GetOwner());
	}
	return nullptr;
}