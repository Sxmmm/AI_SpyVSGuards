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
	////Players State preconditions for this action
	//AddPreCondition("Infected", true);

	//How this action will affect the Players state
	AddEffect("HasKey", true);

	m_sActionName = "Get The Key";
	m_fCost = 1.0f;//Figurative cost of performing this action.
	m_bRequiresInRange = true;
}

Action_GetKey::~Action_GetKey()
{
}

void Action_GetKey::ResetGA()
{
	m_bPerformingAction = false;
	m_paTarget = nullptr;
}

bool Action_GetKey::IsActionFinished()
{
	return m_bPerformingAction;
}

bool Action_GetKey::CheckPreCondition(AActor * a_paAIAgent)
{
	if (Cast<ACPP_GOAP>(a_paAIAgent)->GetHaveKey() == true)
	{
		return false;
	}
	else
	{
		m_vTargetLocation = FVector(500.0f, 1700.0f, 253.0f);
		return true;
	}
}

bool Action_GetKey::PerformAction(AActor * a_paAIAgent)
{
	Cast<ACPP_GOAP>(a_paAIAgent)->SetHaveKey(true);
	m_bPerformingAction = true;
	return true;
}

bool Action_GetKey::NeedsToBeInRange()
{
	return m_bRequiresInRange;
}