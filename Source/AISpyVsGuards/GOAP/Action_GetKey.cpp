#include "GOAP/Action_GetKey.h"
#include "Agents/CPP_SpyAgent.h"
#include "Agents/CPP_GOAP.h"
#include "AIController.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

Action_GetKey::Action_GetKey()
{
	//The AI must not be spotted before it can go to the key
	AddPreCondition("Spotted", false);

	//This is the outcome
	AddEffect("HasKey", true);

	m_sActionName = "Get The Key";
	m_fCost = 1.0f;
	m_bRequiresInRange = true;
}

Action_GetKey::~Action_GetKey()
{
}

void Action_GetKey::ResetGA()
{
	m_bPerformingAction = false;
	m_paTarget = nullptr;
	//m_vTargetLocation = FVector::ZeroVector;
}
//Used to determine if the current action has finished
bool Action_GetKey::IsActionFinished()
{
	return m_bPerformingAction;
}
//Used for the planner to check if it can perform this action
bool Action_GetKey::CheckPreCondition(AActor * a_paAIAgent)
{
	UE_LOG(LogTemp, Warning, TEXT("Checking Get Key"));

	if (Cast<ACPP_GOAP>(a_paAIAgent)->GetHaveKey() == true)
	{
		return false;
	}
	else
	{
		/*TArray<AActor*> aFoundAgents;
		UGameplayStatics::GetAllActorsOfClass(a_paAIAgent->GetWorld(), AActor::StaticClass(), aFoundAgents);
		for (AActor* a_Actor : aFoundAgents)
		{
			if (a_Actor)
			{
				if (a_Actor->ActorHasTag("Key"))
				{
					UE_LOG(LogTemp, Warning, TEXT("GOTTTT THE KEY LOCATION!!!!!!!!"));
					m_vTargetLocation = a_Actor->GetActorLocation();
				}
			}
		}*/
		m_vTargetLocation = FVector(500.0f, 1700.0f, 253.0f);
		return true;
	}
}
//If it can perform the action it will perform this
bool Action_GetKey::PerformAction(AActor * a_paAIAgent)
{
	UE_LOG(LogTemp, Warning, TEXT("GET KEY DONE!!!"));
	Cast<ACPP_GOAP>(a_paAIAgent)->SetHaveKey(true);
	m_bPerformingAction = true;
	return true;
}

bool Action_GetKey::NeedsToBeInRange()
{
	return m_bRequiresInRange;
}