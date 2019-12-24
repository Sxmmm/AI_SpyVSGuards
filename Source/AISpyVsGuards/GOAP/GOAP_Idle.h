#pragma once
#include "GOAP/GOAP_Behaviour.h"

class ACPP_SpyAgent;
class Planner;

class GOAP_Idle : public GOAP_Behaviour
{
public:
	GOAP_Idle(ACPP_SpyAgent* pOwner);
	~GOAP_Idle();

	void Update();
	GOAP_Behaviour* CheckConditions();

private:

	AActor* m_pTargetActor;
	TQueue<Action*> ActionPlan;

	Planner* m_ActionPlanner;
};
