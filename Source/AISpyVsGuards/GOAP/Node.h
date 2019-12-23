#pragma once
#include "Containers/Queue.h"
#include "Containers/Map.h"
#include "Containers/Set.h"

class Action;

class Node
{
public:
	Node();
	Node(Node* a_nParentNode, float a_fCost, TSet<TPair<FString, bool>> a_smState, Action* a_Action);
	~Node();

	Node* m_nParentNode = nullptr;//Stores the parent (The preceeding node)
	float m_fCost = 1.0f;//The cost of this action and its preceeding nodes.
	TSet<TPair<FString, bool>> m_smState = TSet<TPair<FString, bool>>();//The state that this action satisfies
	Action* m_Action = nullptr;//The action this node completes
};
