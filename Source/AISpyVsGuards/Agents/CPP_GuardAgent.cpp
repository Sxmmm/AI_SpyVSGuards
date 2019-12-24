#include "CPP_GuardAgent.h"
#include "Agents/CPP_SpyAgent.h"
#include "Agents/CPP_GOAP.h"
#include "StateMachine/Behaviour.h"
#include "StateMachine/Patrol.h"
#include "StateMachine/Attack.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Components/BoxComponent.h"

// Sets default values
ACPP_GuardAgent::ACPP_GuardAgent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Generating a Capsule used as a trigger to detect collision
	m_pTriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BoxOverlapThingy"));
	m_pTriggerCapsule->InitCapsuleSize(55.f, 96.0f);
	m_pTriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	m_pTriggerCapsule->SetupAttachment(RootComponent);
	m_pTriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GuardAgent::OnOverlapBegin);
	//Generating a Box used as a trigger for the sight of the Guards
	m_pSightTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxOverlapForSight"));
	m_pSightTrigger->InitBoxExtent(FVector(350.0f, 175.0f, 32.0f));
	m_pSightTrigger->SetAllPhysicsPosition(FVector(305.0f, 0.0f, 0.0f));
	m_pSightTrigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	m_pSightTrigger->SetHiddenInGame(false);
	m_pSightTrigger->SetupAttachment(RootComponent);
	m_pSightTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GuardAgent::OnSightOverlapBegin);
}

//On Game Begin
void ACPP_GuardAgent::BeginPlay()
{
	Super::BeginPlay();
	
	m_pCurrentBehaviour = new Patrol(this);
}

// Called every frame Update()
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
//Used for getting and setting has the bool for has spotted
bool ACPP_GuardAgent::GetHasSpottedStatus()
{
	return m_bSpotted;
}
void ACPP_GuardAgent::SetHasSpottedStatus(bool a_bTrue)
{
	m_bSpotted = a_bTrue;
}

//The event attached to the generated Capsule used to detect collision, If the spy is caught the game restarts
void ACPP_GuardAgent::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ACPP_SpyAgent* pOtherAgent = Cast<ACPP_SpyAgent>(OtherActor);
		if (pOtherAgent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Game Over"));
			//UGameplayStatics::OpenLevel(GetOwner()->GetWorld(), FName(*GetOwner()->GetWorld()->GetName()), false);
		}
	}
}
//The event attached to the generated Box used to detect if the AI can see the spy, if so it will change its
//		state to attack
void ACPP_GuardAgent::OnSightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Collision"));
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ACPP_SpyAgent* pOtherAgent = Cast<ACPP_SpyAgent>(OtherActor);
		if (pOtherAgent)
		{
			if (Cast<ACPP_GOAP>(OtherActor)->GetHasSpotted() == false)
			{
				Cast<ACPP_GOAP>(OtherActor)->SetHasSpotted(true);
				Cast<ACPP_GOAP>(OtherActor)->InterruptBehaviour();
			}
			UE_LOG(LogTemp, Warning, TEXT("Spotted The Spy!!!"));
			SetHasSpottedStatus(true);
		}
	}
}