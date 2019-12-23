#pragma once

#include "Agents/CPP_SpyAgent.h"
#include "Agents/CPP_GOAP.h"

class GOAP_Behaviour
{
public:
	GOAP_Behaviour(ACPP_SpyAgent* pOwner) { m_pOwner = pOwner; }
	virtual ~GOAP_Behaviour() {};

	virtual void Update() = 0;
	virtual GOAP_Behaviour* CheckConditions() = 0;

	ACPP_SpyAgent* GetOwner() { return m_pOwner; }

private:
	ACPP_SpyAgent* m_pOwner;
};