#pragma once
#include "GOAP/GOAP_Behaviour.h"

class ACPP_SpyAgent;
class Planner;
class Action;

class GOAP_PerformAction : public GOAP_Behaviour
{
public:
	GOAP_PerformAction(ACPP_SpyAgent* pOwner, Action* a_pAction);
	~GOAP_PerformAction() {};

	void Update();
	GOAP_Behaviour* CheckConditions();

private:

	Action* m_pAction;
};