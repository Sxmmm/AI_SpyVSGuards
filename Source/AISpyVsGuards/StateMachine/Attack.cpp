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
	UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
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

				if (FVector::Dist(GetOwner()->GetActorLocation(), m_pTargetActor->GetActorLocation()) >= 1000.0f)
				{
					//IF SPY IS OUT OF SENSORY RANGE
					SetLastKnownPosition(m_pTargetActor->GetActorLocation());
					m_eCurrentAttackState = GET_SPY_LAST_POSITION;
					UE_LOG(LogTemp, Warning, TEXT("Lost Spy!!!"));
				}
				else
				{
					m_eCurrentAttackState = GET_SPY_POSITION;
				}
			}
			else
			{
				m_eCurrentAttackState = GET_SPY_POSITION;
			}
		}
		break;
	}
	case GET_SPY_LAST_POSITION:
	{
		AAIController* pAIController = Cast<AAIController>(GetOwner()->GetController());
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(pAIController, GetLastKnownPosition());

		if (FVector::Dist(GetOwner()->GetActorLocation(), m_pTargetActor->GetActorLocation()) <= 500.0f)
		{
			m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
		}
		if (FVector::Dist(GetOwner()->GetActorLocation(), GetLastKnownPosition()) <= 100.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("At Location"));
			if (FVector::Dist(GetOwner()->GetActorLocation(), m_pTargetActor->GetActorLocation()) <= 500.0f)
			{
				m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Look For Spy!!!"));
				m_eCurrentAttackState = LOOK_AROUND_FOR_SPY;
				UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetOwner()->GetWorld());
				SetLastKnownPosition(NavSystem->GetRandomPointInNavigableRadius(GetOwner()->GetWorld(), (GetOwner()->GetActorLocation() + m_pTargetActor->GetActorLocation()) / 2, 500.0f));
			}
		}
		break;
	}
	case LOOK_AROUND_FOR_SPY:
	{
		//IF SPY IS FOUND
		AAIController* pAIController = Cast<AAIController>(GetOwner()->GetController());

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetOwner()->GetWorld());

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(pAIController, GetLastKnownPosition());
		if (FVector::Dist(GetOwner()->GetActorLocation(), m_pTargetActor->GetActorLocation()) <= 750.0f)
		{
			//Reset the looking for spy loop
			LookLoop = 0;
			m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
		}
		else if (FVector::Dist(GetOwner()->GetActorLocation(), GetLastKnownPosition()) <= 250.0f)
		{
			SetLastKnownPosition(NavSystem->GetRandomPointInNavigableRadius(GetOwner()->GetWorld(), (GetOwner()->GetActorLocation() + m_pTargetActor->GetActorLocation()) / 2, 500.0f));
			UE_LOG(LogTemp, Warning, TEXT("RAND"));
			LookLoop += 1;
		}
		if(LookLoop >= 2)
		{
			LookLoop = 0;
			GetOwner()->SetHasSpottedStatus(false);
		}
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

FVector Attack::GetLastKnownPosition()
{
	return LastKnowSpyPos;
}

void Attack::SetLastKnownPosition(FVector a_vpos)
{
	LastKnowSpyPos = a_vpos;
}
