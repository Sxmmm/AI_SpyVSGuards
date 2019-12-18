#include "CPP_GuardAgent.h"
#include "StateMachine/Behaviour.h"
#include "StateMachine/Patrol.h"
#include "StateMachine/Attack.h"
#include "Engine/Classes/Components/CapsuleComponent.h"

// Sets default values
ACPP_GuardAgent::ACPP_GuardAgent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pTriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BoxOverlapThingy"));
	m_pTriggerCapsule->InitCapsuleSize(55.f, 96.0f);
	m_pTriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	m_pTriggerCapsule->SetupAttachment(RootComponent);
	m_pTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GuardAgent::OnOverlapBegin);
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

void ACPP_GuardAgent::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision"));
	//if (OtherActor && (OtherActor != this) && OtherComp)
	//{
	//	ACPP_GuardAgent* pOtherAgent = Cast<ACPP_GuardAgent>(OtherActor);
	//	if (pOtherAgent && pOtherAgent->GetInfectedStatus())
	//	{
	//		//printFString("Overlapped Actor = %s", *OtherActor->GetName());

	//		SetInfectedStatus(true);
	//	}
	//}
}