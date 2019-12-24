#include "Agents/CPP_SpyAgent.h"
#include "Agents/CPP_GuardAgent.h"
#include "Agents/CPP_GOAP.h"
#include "GOAP/GOAP_Behaviour.h"
#include "GOAP/GOAP_Idle.h"

#include "GOAP/Planner.h"
#include "GOAP/Action.h"

#include "Engine/Classes/Components/CapsuleComponent.h"


// Sets default values
ACPP_SpyAgent::ACPP_SpyAgent()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pTriggerRadius = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Radius"));
	m_pTriggerRadius->InitCapsuleSize(650.0f, 96.0f);
	m_pTriggerRadius->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	m_pTriggerRadius->SetHiddenInGame(false);
	m_pTriggerRadius->SetupAttachment(RootComponent);
	m_pTriggerRadius->OnComponentBeginOverlap.AddDynamic(this, &ACPP_SpyAgent::OnOverlapBeginWhatGuard);
}

// Called when the game starts or when spawned
void ACPP_SpyAgent::BeginPlay()
{
	Super::BeginPlay();

	m_pIdleState = new GOAP_Idle(this);
	m_pCurrentBehaviour = m_pIdleState;
}

// Called every frame
void ACPP_SpyAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_pCurrentBehaviour)
	{
		GOAP_Behaviour* pNewBehaviour = m_pCurrentBehaviour->CheckConditions();
		if (pNewBehaviour)
		{
			delete m_pCurrentBehaviour;

			m_pCurrentBehaviour = pNewBehaviour;
		}
		else if (m_bInterruptBehaviour)
		{
			m_bInterruptBehaviour = false;
			delete m_pCurrentBehaviour;
			m_pCurrentBehaviour = new GOAP_Idle(this);
		}

		m_pCurrentBehaviour->Update();
	}
}

void ACPP_SpyAgent::InterruptBehaviour()
{
	m_bInterruptBehaviour = true;
}

// Called to bind functionality to input
void ACPP_SpyAgent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACPP_SpyAgent::OnOverlapBeginWhatGuard(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("guard in spy range"));
	}
}