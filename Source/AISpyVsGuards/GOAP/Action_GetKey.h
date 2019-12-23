#pragma once
#include "GOAP/Action.h"

class Action_GetKey : public Action
{
public:
	Action_GetKey();
	~Action_GetKey();

	virtual void ResetGA() override;
	virtual bool IsActionFinished() override;
	virtual bool CheckPreCondition(AActor* a_paAIAgent) override;
	virtual bool PerformAction(AActor* a_paAIAgent) override;
	virtual bool NeedsToBeInRange() override;

private:
	bool m_bRequiresInRange = true;
	bool m_bPerformingAction = false;
};
