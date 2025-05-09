#pragma once

#include "CosmosGlobals.h"
#include "Core/SignalEventManager.h"

class IGameflowContext : public TSharedFromThis<IGameflowContext> {
	friend struct FGameflowSignal;

protected:
	virtual void Activate() {}
	virtual void Deavtivate() {}

public:
	virtual ~IGameflowContext() {}

	template<typename ContextType>
	static TSharedPtr<ContextType> Push() {
		TSharedPtr<ContextType> Context = MakeShareable(new ContextType, [](ContextType* Context) {
			Context->Deavtivate();
			Context->Pop();
			delete Context;
			});
		if (AllContexts.Contains(ContextType::GetContextName())) {
			UE_LOG(LogCmsSignal, Warning, TEXT("Context [%s] already existed!"), *ContextType::GetContextName().ToString());
		}
		IGameflowContext* ContextBase = Context.Get();
		AllContexts.Add(ContextType::GetContextName(), ContextBase);
		ContextBase->Activate();
		ContextBase->bActivated = false;
		return Context;
	}

	template<typename ContextType>
	void Pop() {
		if (ContextType* Context = Get<ContextType>()) {
			Context->Pop();
		}
	}

	void Pop();

	template<typename ContextType>
	ContextType* Get() {
		return AllContexts.Find(ContextType::GetContextName());
	}

private:
	bool bActivated = false;
	TArray<int32> LocalEvents;
	static TMap<FName, IGameflowContext*> AllContexts;
};

struct FGameflowSignal {
	FName Name;
	IGameflowContext* Owner = nullptr;

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

#define GAMEFLOW_CONTEXT_BEGIN(Name) \
    public: \
        static FName GetContextName() { return TEXT(#Name); }; \
        struct FSignals { \
            IGameflowContext* Owner = nullptr;

#define GAMEFLOW_CONTEXT_END() \
        } Signals { this };

#define GAMEFLOW_CONTEXT_SIGNAL(SignalName) \
            FGameflowSignal SignalName{ #SignalName , Owner };


