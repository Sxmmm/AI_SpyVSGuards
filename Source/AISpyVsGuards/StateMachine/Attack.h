#pragma once

#include "StateMachine/Behaviour.h"

class ACPP_GuardAgent;

enum ATTACK_STATE
{
	GET_SPY_POSITION,
	MOVE_TO_SPY_POSITION,
	GET_SPY_LAST_POSITION,
	LOOK_AROUND_FOR_SPY,
};

class Attack : public Behaviour
{
public:
	Attack(ACPP_GuardAgent* pOwner);
	~Attack() {};

	void Update();
	Behaviour* CheckConditions();

	FVector GetLastKnownPosition();
	void SetLastKnownPosition(FVector a_vpos);

private:
	ATTACK_STATE m_eCurrentAttackState;

	AActor* m_pTargetActor;
	UPROPERTY(EditAnywhere)
		FVector LastKnowSpyPos;
	UPROPERTY(EditAnywhere)
		int LookLoop;
};

