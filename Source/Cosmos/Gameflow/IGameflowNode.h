#pragma once

#include "CosmosGlobals.h"
#include "Core/SignalEventManager.h"

class IGameflowNode : public TSharedFromThis<IGameflowNode> {
    friend struct FGameflowSignal;
public:
    virtual ~IGameflowNode() {
        if (USignalEventManager::Get() == nullptr)
            return;
        for (int32 ID : LocalEvents) {
            USignalEventManager::Get()->RemoveSignalEvent(ID);
        }
    }

    virtual void Activate() {}
    virtual void Deavtivate() {}

    template<typename NodeType>
    static TSharedPtr<NodeType> Make() {
        TSharedPtr<NodeType> Node = MakeShareable(new NodeType, [](NodeType* Node) {
            Node->Deavtivate();
            AllNodes.Remove(NodeType::GetNodeName());
            delete Node;
        });
        if (AllNodes.Contains(NodeType::GetNodeName())) {
            UE_LOG(LogCmsSignal, Warning, TEXT("Node [%s] already existed!"), *NodeType::GetNodeName().ToString());
        }
        AllNodes.Add(NodeType::GetNodeName(), Node.Get());
        Node->Activate();
        return Node;
    }

    template<typename NodeType>
    NodeType* Get() {
        return AllNodes.Find(NodeType::GetNodeName());
    }
private:
    TArray<int32> LocalEvents;
    static TMap<FName, IGameflowNode*> AllNodes;
};

struct FGameflowSignal{
    FName Name;
    IGameflowNode* Owner = nullptr;

    template <typename FuncType>
    void BindEvent(const FuncType& EventCallback) {
        int32 ID = USignalEventManager::Get()->AddSignalEvent(Name, EventCallback);
        Owner->LocalEvents.Add(ID);
    }

    template <typename... ArgsType>
    void Invoke(ArgsType... Args) {
        USignalEventManager::Get()->InvokeSignal(Name, Args...);
    }
};


#define GAMEFLOW_NODE_BEGIN(Name) \
    public: \
        inline static struct FSignals_##Name { 

        

    