#pragma once

#include "StateMachine/Behaviour.h"

class ACPP_GuardAgent;

enum ATTACK_STATE
{
	MOVE_TO_SPY_POSITION,
	MOVE_TO_SPY_LAST_KNOWN_POSITION,
	LOOK_AROUND_FOR_SPY,
	CAPTURE_SPY
};

class Attack : public Behaviour
{
public:
	Attack(ACPP_GuardAgent* pOwner);
	~Attack() {};

	void Update();
	Behaviour* CheckConditions();

private:
	ATTACK_STATE m_eCurrentAttackState;

	AActor* m_pTargetActor; //Stores the spy upon sight
};

