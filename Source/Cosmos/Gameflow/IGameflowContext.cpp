#include "IGameflowContext.h"

void IGameflowContext::Pop()
{
    if (USignalEventManager::Get() == nullptr || !bActivated)
        return;
    for (int32 ID : LocalEvents) {
        USignalEventManager::Get()->RemoveSignalEvent(ID);
    }        
    for (auto Context : AllContexts) {
        if (Context.Value == this) {
            AllContexts.Remove(Context.Key);
            break;
        }
    }
    bActivated = false;
}

TMap<FName, IGameflowContext*> IGameflowContext::AllContexts;
