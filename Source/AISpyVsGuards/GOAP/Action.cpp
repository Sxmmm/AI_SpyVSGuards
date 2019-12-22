#include "Action.h"

Action::Action()
{
	m_smPreconditions = TSet<TPair<FString, bool>>();
	m_smEffects = TSet<TPair<FString, bool>>();
}

Action::~Action()
{
}

void Action::ResetAction()
{
	m_bInRange = false;
	m_paTarget = nullptr;
	ResetGA();
}

void Action::ResetGA()
{
}

bool Action::IsActionFinished()
{
	return false;
}

bool Action::CheckPreCondition(AActor * a_paAIAgent)
{
	return false;
}

bool Action::PerformAction(AActor * a_paAIAgent)
{
	return false;
}

bool Action::NeedsToBeInRange()
{
	return false;
}

bool Action::IsAgentInRange()
{
	return m_bInRange;
}

void Action::SetInRange(bool a_bTrue)
{
	m_bInRange = a_bTrue;
}

void Action::AddPreCondition(FString a_sPreCondition, bool a_bValue)
{
	m_smPreconditions.Add(TPair<FString, bool>(a_sPreCondition, a_bValue));
}

void Action::RemovePreCondition(FString a_sPreCondition)
{
	TPair<FString, bool> pPreConToRemove = TPair<FString, bool>();
	for (TPair<FString, bool> pPreCon : m_smPreconditions)
	{
		if (pPreCon.Key == a_sPreCondition)
		{
			pPreConToRemove = pPreCon;
			m_smPreconditions.Remove(pPreConToRemove);
		}
	}
}

void Action::AddEffect(FString a_sEffect, bool a_bValue)
{
	m_smEffects.Add(TPair<FString, bool>(a_sEffect, a_bValue));
}

void Action::RemoveEffect(FString a_sEffect)
{
	TPair<FString, bool> pEffectToRemove = TPair<FString, bool>();
	for (TPair<FString, bool> pEffect : m_smEffects)
	{
		if (pEffect.Key == a_sEffect)
		{
			pEffectToRemove = pEffect;
			m_smEffects.Remove(pEffectToRemove);
		}
	}
}

TSet<TPair<FString, bool>> Action::GetPreConditions()
{
	return m_smPreconditions;
}

TSet<TPair<FString, bool>> Action::GetEffects()
{
	return m_smEffects;
}
