#pragma once

#include "Core/Basic/ObjectPlus.h"
#include "Core/Basic/SignalEventManager.h"
#include "GameflowContextManager.h"
#include "GameflowContext.generated.h"

UCLASS()
class UGameflowContext : public UObjectPlus {
	GENERATED_BODY()
	friend class UGameflowContextManager;
	friend struct FGameflowSignal;
protected:
	virtual void Activate() {}
	virtual void Deavtivate() {}

public:
	virtual ~UGameflowContext() {}

private:
	bool bActivated = false;
	TArray<int32> LocalEvents;
};

struct FGameflowSignal {
	FName Name;
	UGameflowContext* Owner = nullptr;

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
            UGameflowContext* Owner = nullptr;

#define GAMEFLOW_CONTEXT_END() \
        } Signals { this };

#define GAMEFLOW_CONTEXT_SIGNAL(SignalName) \
            FGameflowSignal SignalName{ #SignalName , Owner };

