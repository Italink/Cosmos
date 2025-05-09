#pragma once

#include "CosmosGlobals.h"
#include "Core/SignalEventManager.h"

class IGameflowNode : public TSharedFromThis<IGameflowNode> {
	friend struct FGameflowSignal;

protected:
	virtual void Activate() {}
	virtual void Deavtivate() {}

public:
	virtual ~IGameflowNode() {}

	template<typename NodeType>
	static TSharedPtr<NodeType> Push() {
		TSharedPtr<NodeType> Node = MakeShareable(new NodeType, [](NodeType* Node) {
			Node->Deavtivate();
			Node->Pop();
			delete Node;
			});
		if (AllNodes.Contains(NodeType::GetNodeName())) {
			UE_LOG(LogCmsSignal, Warning, TEXT("Node [%s] already existed!"), *NodeType::GetNodeName().ToString());
		}
		AllNodes.Add(NodeType::GetNodeName(), Node.Get());
		Node->Activate();
		Node->bActivated = false;
		return Node;
	}

	template<typename NodeType>
	void Pop() {
		if (NodeType* Node = Get<NodeType>()) {
			Node->Pop();
		}
	}

	void Pop();

	template<typename NodeType>
	NodeType* Get() {
		return AllNodes.Find(NodeType::GetNodeName());
	}

private:
	bool bActivated = false;
	TArray<int32> LocalEvents;
	static TMap<FName, IGameflowNode*> AllNodes;
};

struct FGameflowSignal {
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
        static FName GetNodeName() { return TEXT(#Name); }; \
        struct FSignals { \
            IGameflowNode* Owner = nullptr;

#define GAMEFLOW_NODE_END() \
        } Signals { this };

#define GAMEFLOW_NODE_SIGNAL(SignalName) \
            FGameflowSignal SignalName{ #SignalName , Owner };


