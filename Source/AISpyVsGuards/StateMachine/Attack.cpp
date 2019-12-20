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

//Init attack behaviour
Attack::Attack(ACPP_GuardAgent* pOwner) : Behaviour(pOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
	//Setting start state
	m_eCurrentAttackState = GET_SPY_POSITION;
}

void Attack::Update()
{
	switch (m_eCurrentAttackState)
	{
	case GET_SPY_POSITION:
	{
		//Getting the spy and storing for later use
		TArray<AActor*> aFoundSpy;
		UGameplayStatics::GetAllActorsOfClass(GetOwner()->GetWorld(), ACPP_SpyAgent::StaticClass(), aFoundSpy);
		m_pTargetActor = nullptr;
		//Loop though all spies (could be multiple spies)
		for (AActor* a_Actor : aFoundSpy)
		{
			if (a_Actor)
			{
				m_pTargetActor = a_Actor;//Storing the first spy found
			}
		}
		//Switch state to move to spies position
		m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
		break;
	}
	case MOVE_TO_SPY_POSITION:
	{
		//Getting the AI controller of the actor
		AAIController* pAIController = Cast<AAIController>(GetOwner()->GetController());
		//Moving towards the spy
		UAIBlueprintHelperLibrary::SimpleMoveToActor(pAIController, m_pTargetActor);
		if (pAIController)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToActor(pAIController, m_pTargetActor);

			if (m_pTargetActor)
			{
				//Checking to see if the AI has lost sight of the spy
				if (FVector::Dist(GetOwner()->GetActorLocation(), m_pTargetActor->GetActorLocation()) >= 1000.0f)
				{
					//If the spy is out of range we will switch state to get the last known position of the spy
					SetLastKnownPosition(m_pTargetActor->GetActorLocation());
					m_eCurrentAttackState = GET_SPY_LAST_POSITION;
					UE_LOG(LogTemp, Warning, TEXT("Lost Spy!!!"));
				}
				else
				{
					//If the spy is in sight we want to keep the position of the spy updated
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
		//Getting the AI controller of the actor
		AAIController* pAIController = Cast<AAIController>(GetOwner()->GetController());
		//Moving towards the spies last know position
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(pAIController, GetLastKnownPosition());
		//If the spy is back in sight range return to move towards spy
		if (FVector::Dist(GetOwner()->GetActorLocation(), m_pTargetActor->GetActorLocation()) <= 500.0f)
		{
			m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
		}
		//If we get to the last know position for the spy and the spy is out of sight
		if (FVector::Dist(GetOwner()->GetActorLocation(), GetLastKnownPosition()) <= 100.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("At Location"));
			//If the spy is back in sight once we reach the position
			if (FVector::Dist(GetOwner()->GetActorLocation(), m_pTargetActor->GetActorLocation()) <= 500.0f)
			{
				m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
			}
			//If the spy is out of sight the AI will continue to search around for the spy
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
		//Getting the AI controller of the actor
		AAIController* pAIController = Cast<AAIController>(GetOwner()->GetController());
		//Getting the navigation mesh
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetOwner()->GetWorld());
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(pAIController, GetLastKnownPosition());
		//if the spy is in sight while looking around it will switch state moving back towards the spy
		if (FVector::Dist(GetOwner()->GetActorLocation(), m_pTargetActor->GetActorLocation()) <= 750.0f)
		{
			//Reset the looking for spy loop
			LookLoop = 0;
			m_eCurrentAttackState = MOVE_TO_SPY_POSITION;
		}
		//If the AI is at the last known position, it will find a new area to look at. This is done by using RandPointInNav and getting the mid point of the AI and the spy
		//		We let the AI know the general direction of the spy to enhance realsim. The guard would have seen the direction the spy was going so it would be silly for it
		//		to turn around and look somewhere unrelated
		else if (FVector::Dist(GetOwner()->GetActorLocation(), GetLastKnownPosition()) <= 250.0f)
		{
			SetLastKnownPosition(NavSystem->GetRandomPointInNavigableRadius(GetOwner()->GetWorld(), (GetOwner()->GetActorLocation() + m_pTargetActor->GetActorLocation()) / 2, 500.0f));
			LookLoop += 1;
		}
		//We use an int to determine if the guard has done enough seraching and should carry on back to their patrol
		if(LookLoop >= 2)
		{
			LookLoop = 0;
			GetOwner()->SetHasSpottedStatus(false);
		}
		break;
	}
	}
}

//If the spy can not be found, the has spotted staus will be changed and patrol will be its new behaviour
Behaviour* Attack::CheckConditions()
{
	if (!GetOwner()->GetHasSpottedStatus())
	{
		return new Patrol(GetOwner());
	}
	return nullptr;
}

//Used for setting and getting the last known position of the spy
FVector Attack::GetLastKnownPosition()
{
	return LastKnowSpyPos;
}
void Attack::SetLastKnownPosition(FVector a_vpos)
{
	LastKnowSpyPos = a_vpos;
}
