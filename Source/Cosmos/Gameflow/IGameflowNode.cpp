#include "IGameflowNode.h"

void IGameflowNode::Pop()
{
    if (USignalEventManager::Get() == nullptr || !bActivated)
        return;
    for (int32 ID : LocalEvents) {
        USignalEventManager::Get()->RemoveSignalEvent(ID);
    }        
    for (auto Node : AllNodes) {
        if (Node.Value == this) {
            AllNodes.Remove(Node.Key);
            break;
        }
    }
    bActivated = false;
}

TMap<FName, IGameflowNode*> IGameflowNode::AllNodes;
