#pragma once

#include "CosmosGlobals.h"
#include "GameflowContextManager.generated.h"

class UGameflowContext;

UCLASS(MinimalAPI)
class UGameflowContextManager: public UGameInstanceSubsystem {
	GENERATED_BODY()
public:
	static UGameflowContextManager* Get(UObject* InWorldContext = nullptr);

	template<typename ContextType>
	TObjectPtr<ContextType> Push() {
		TObjectPtr<ContextType> Context = NewObject<ContextType>();
		if (AllContexts.Contains(ContextType::GetContextName())) {
			UE_LOG(LogCmsSignal, Warning, TEXT("Context [%s] already existed!"), *ContextType::GetContextName().ToString());
		}
		UGameflowContext* ContextBase = Context.Get();
		AllContexts.Add(ContextType::GetContextName(), ContextBase);
		ContextBase->Activate();
		ContextBase->bActivated = false;
		return Context;
	}

	template<typename ContextType>
	void Pop() {
		if (ContextType* Context = Get<ContextType>()) {
			if (Context->bActivated) {
				Context->Deavtivate();
			}
			AllContexts.Remove(ContextType::GetContextName());
		}
	}

	template<typename ContextType>
	ContextType* Get() {
		if (auto ContextPtr = AllContexts.Find(ContextType::GetContextName())) {
			return Cast<ContextType>(*ContextPtr);
		}
		return nullptr;
	}

private:
	UPROPERTY()
	TMap<FName, UGameflowContext*> AllContexts;
};
