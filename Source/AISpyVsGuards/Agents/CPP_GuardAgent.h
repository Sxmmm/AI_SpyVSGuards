// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//KEEP THIS INCLUDE LAST
#include "CPP_GuardAgent.generated.h"

// Forward declarations
class Behaviour;

UCLASS()
class AISPYVSGUARDS_API ACPP_GuardAgent : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_GuardAgent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Used to modify state
	bool GetHasSpottedStatus();
	void SetHasSpottedStatus(bool a_btrue);

private:
	Behaviour* m_pCurrentBehaviour;

	//Tells us if the guard has spotted the spy
	UPROPERTY(EditAnywhere)
		bool m_bSpotted = false;
};
