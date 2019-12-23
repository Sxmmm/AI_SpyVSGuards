#pragma once

#include "GOAP/GOAP_Behaviour.h"
#include "GOAP/Action.h"

class GOAP_Move : public GOAP_Behaviour
{
public:
	GOAP_Move(ACPP_SpyAgent* pOwner, Action* a_ActionToGoTo);
	~GOAP_Move() {};

	void Update();
	GOAP_Behaviour* CheckConditions();

private:

	AActor* m_pTargetActor;//Stores the actor that we are chasing

	Action* m_pCurrentAction;//The action we are moving to
};
