// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/Queue.h"
//#include "GOAP/GOAP_Behaviour.h"
//KEEP THIS INCLUDE LAST
#include "CPP_SpyAgent.generated.h"

class GOAP_Behaviour;

UCLASS()
class AISPYVSGUARDS_API ACPP_SpyAgent : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_SpyAgent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void InterruptBehaviour();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetBehaviour(GOAP_Behaviour* a_pBehaviour) { m_pCurrentBehaviour = a_pBehaviour; }

	UFUNCTION()
		void OnOverlapBeginWhatGuard(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* m_pTriggerRadius;

	GOAP_Behaviour* m_pCurrentBehaviour;
	GOAP_Behaviour* m_pIdleState;
	bool m_bInterruptBehaviour = false;
};
