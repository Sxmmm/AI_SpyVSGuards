#include "GOAP/Action_Exit.h"
#include "Agents/CPP_SpyAgent.h"
#include "Agents/CPP_GOAP.h"
#include "AIController.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

Action_Exit::Action_Exit()
{
	//The AI needs the key before can exit
	AddPreCondition("HasKey", true);

	AddEffect("Exit", true);

	m_sActionName = "Get To Exit";
	m_fCost = 1.0f;
	m_bRequiresInRange = true;
}

Action_Exit::~Action_Exit()
{
}

void Action_Exit::ResetGA()
{
	m_bPerformingAction = false;
	m_paTarget = nullptr;
}
//Used to determine if the current action has finished
bool Action_Exit::IsActionFinished()
{
	return m_bPerformingAction;
}
//Used for the planner to check if it can perform this action
bool Action_Exit::CheckPreCondition(AActor * a_paAIAgent)
{
	UE_LOG(LogTemp, Warning, TEXT("Checking EXIT"))
	m_vTargetLocation = FVector(1585.0f, -1690.0, 253.0f);

	return true;
}
//If it can perform the action it will perform this
bool Action_Exit::PerformAction(AActor * a_paAIAgent)
{
	UE_LOG(LogTemp, Warning, TEXT("Spy has won!"));
	UE_LOG(LogTemp, Warning, TEXT("EXIT DONE!!!"));
	m_bPerformingAction = true;
	return true;
}

bool Action_Exit::NeedsToBeInRange()
{
	return m_bRequiresInRange;
}