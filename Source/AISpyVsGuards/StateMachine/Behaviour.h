#pragma once

#include "Agents/CPP_GuardAgent.h"

class Behaviour
{
public:
	Behaviour(ACPP_GuardAgent* pOwner) { m_pOwner = pOwner; }
	virtual ~Behaviour() {};

	virtual void Update() = 0;
	virtual Behaviour* CheckConditions() = 0;

	ACPP_GuardAgent* GetOwner() { return m_pOwner; }

private:
	ACPP_GuardAgent* m_pOwner;
};