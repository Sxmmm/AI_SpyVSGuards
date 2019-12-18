#include "Attack.h"
#include "Agents/CPP_GuardAgent.h"
#include "Agents/CPP_SpyAgent.h"
#include "AIController.h"
#include "StateMachine/Patrol.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/AI/NavigationSystemBase.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/NavigationSystem/Public/AbstractNavData.h"

Attack::Attack(ACPP_GuardAgent* pOwner) : Behaviour(pOwner)
{
	m_eCurrentAttackState = GET_SPY_POSITION;
}

void Attack::Update()
{
	switch (m_eCurrentAttackState)
	{
	case GET_SPY_POSITION:
	{
		//GET SPY POSITION
		TArray<AActor*> aFoundSpy;
		UGameplayStatics::GetAllActorsOfClass(GetOwner()->GetWorld(), ACPP_SpyAgent::StaticClass(), aFoundSpy);
		m_pTargetActor = nullptr;

		for (AActor* a_Actor : aFoundSpy)//Loop through actor list
		{
			if (a_Actor)//Null check
			{
				m_pTargetActor = a_Actor;//Set new target
			}
		}
		//MOVE TO SPY POSITION
		//IF SPY IS OUT OF SENSORY RANGE
		m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
		break;
	}
	case MOVE_TO_SPY_POSITION:
	{
		//MOVE TO SPY POSITION
		AAIController* pAIController = Cast<AAIController>(GetOwner()->GetController());
		UAIBlueprintHelperLibrary::SimpleMoveToActor(pAIController, m_pTargetActor);//Move to target actor
		if (pAIController)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToActor(pAIController, m_pTargetActor);//Move to target actor

			if (m_pTargetActor)//Null check
			{
				if (FVector::Dist(GetOwner()->GetActorLocation(), m_pTargetActor->GetActorLocation()) <= 100.0f)
				{
					//If the spy is in reach
					m_eCurrentAttackState = CAPTURE_SPY;
				}

				//if (FVector::Dist(GetOwner()->GetActorLocation(), m_pTargetActor->GetActorLocation()) <= 5000.0f)
				//{
				//	//IF SPY IS OUT OF SENSORY RANGE
				//	m_eCurrentAttackState = LOOK_AROUND_FOR_SPY;
				//}
			}
			else
			{
				m_eCurrentAttackState = GET_SPY_POSITION; //Find new target
			}
		}
		break;
	}
	case LOOK_AROUND_FOR_SPY:
	{
		//IF SPY IS FOUND
		AAIController* pAIController = Cast<AAIController>(GetOwner()->GetController());

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetOwner()->GetWorld());
		FVector vRandPos = NavSystem->GetRandomPointInNavigableRadius(GetOwner()->GetWorld(), pAIController->GetNavAgentLocation(), 20000.0f);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(pAIController, vRandPos);


		m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
		//IF SPY IS IN REACH
		m_eCurrentAttackState = CAPTURE_SPY;
		break;
	}
	case CAPTURE_SPY:
	{
		UGameplayStatics::OpenLevel(GetOwner()->GetWorld(), FName(*GetOwner()->GetWorld()->GetName()), false);
		break;
	}
	}
}

Behaviour* Attack::CheckConditions()
{
	//Check if spy cannot be found, resume patrol
	if (!GetOwner()->GetHasSpottedStatus())
	{
		return new Patrol(GetOwner());
	}
	return nullptr;
}