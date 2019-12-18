#include "CPP_GuardAgent.h"
#include "StateMachine/Behaviour.h"
#include "StateMachine/Patrol.h"
#include "StateMachine/Attack.h"

// Sets default values
ACPP_GuardAgent::ACPP_GuardAgent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_GuardAgent::BeginPlay()
{
	Super::BeginPlay();
	
	m_pCurrentBehaviour = new Patrol(this);
}

// Called every frame
void ACPP_GuardAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Checking to see if our state has changed
	if (m_pCurrentBehaviour)
	{

		Behaviour* pNewBehaviour = m_pCurrentBehaviour->CheckConditions();
		if (pNewBehaviour)
		{
			delete m_pCurrentBehaviour;

			m_pCurrentBehaviour = pNewBehaviour;
		}
		m_pCurrentBehaviour->Update();
	}
}

// Called to bind functionality to input
void ACPP_GuardAgent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ACPP_GuardAgent::GetHasSpottedStatus()
{
	return m_bSpotted;
}

void ACPP_GuardAgent::SetHasSpottedStatus(bool a_bTrue)
{
	m_bSpotted = a_bTrue;
}