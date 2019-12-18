#pragma once

#include "StateMachine/Behaviour.h"

class ACPP_GuardAgent;

enum PATROL_STATE
{
	FIND_NEXT_POSITION,
	MOVE_TO_POSITION,
};

class Patrol : public Behaviour
{
public:
	Patrol(ACPP_GuardAgent* pOwner);
	~Patrol() {};

	void Update();
	Behaviour* CheckConditions();

private:
	PATROL_STATE m_eCurrentPatrolState;

	AActor* m_pTargetActor;
};
