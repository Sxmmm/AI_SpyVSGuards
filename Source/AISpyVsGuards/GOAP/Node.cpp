#include "GOAP/Node.h"
#include "GOAP/Action.h"

Node::Node(Node* a_nParentNode, float a_fCost, TSet<TPair<FString, bool>> a_smState, Action* a_Action)
{
	m_nParentNode = a_nParentNode;
	m_fCost = a_fCost;
	m_smState = a_smState;
	m_Action = a_Action;
}

Node::~Node()
{
}