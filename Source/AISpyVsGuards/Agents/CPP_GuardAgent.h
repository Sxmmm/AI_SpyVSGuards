#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CPP_GuardAgent.generated.h"

class Behaviour;

UCLASS()
class AISPYVSGUARDS_API ACPP_GuardAgent : public ACharacter
{
	GENERATED_BODY()

public:
	ACPP_GuardAgent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetHasSpottedStatus();
	void SetHasSpottedStatus(bool a_btrue);

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnSightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	Behaviour* m_pCurrentBehaviour;
	UPROPERTY(EditAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* m_pTriggerCapsule;
	UPROPERTY(EditAnywhere, Category = "Sight Trigger Box")
		class UBoxComponent* m_pSightTrigger;

	UPROPERTY(EditAnywhere)
		bool m_bSpotted = false;
};