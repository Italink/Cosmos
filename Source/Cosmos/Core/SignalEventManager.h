#pragma once

#include "Core/DynamicLambda.h"
#include "CosmosGlobals.h"
#include "TemplateTraits.h"
#include "SignalEventManager.generated.h"

class ISignalEvent
{
public:
	virtual ~ISignalEvent() {};
	virtual bool IsNative() { return true; }
	virtual void Invoke(void* Params) {};
	int32 GetID() const { return ID; }
	void SetOnlyOnce(bool inOnce) { bOnlyOnce = inOnce; }
	bool IsOnlyOnce() const { return bOnlyOnce; }
private:
	int32 ID = -1;
	bool bOnlyOnce = false;
};

template <typename FuncType>
class FSignalEvent_Lambda: public ISignalEvent {
public:
	FSignalEvent_Lambda(const FuncType& InCallback)
		: Callback(InCallback)
	{
	}
	bool IsNative() { return false; }
	virtual void Invoke(void* Params) override {
		using TupleType = typename TemplateTraits::FuncTraits<FuncType>::TupleType;
		constexpr size_t ArgCount = std::tuple_size<TupleType>::value;
		auto Args = TemplateTraits::ParseParamsFromBuffer<TupleType>(Params, std::make_index_sequence<ArgCount>{});
		std::apply(Callback, Args);
	}
private:
	FuncType Callback;
};

UCLASS(MinimalAPI)
class USignalEventManager: public UGameInstanceSubsystem {
	GENERATED_BODY()
public:
	static USignalEventManager* Get(UObject* InWorldContext = nullptr);

	template <typename FuncType>
	int32 AddSignalEvent(FName SignalName, const FuncType& EventCallback, bool bOnlyOnce = false) {
		auto& Event = EventNameMap.FindOrAdd(SignalName).Add_GetRef(MakeShared<FSignalEvent_Lambda<FuncType>>(EventCallback));
		if (bOnlyOnce){
			Event->SetOnlyOnce(true);
		}
		EventIDMap.Add(Event->GetID(), TPair<FName, ISignalEvent*>(SignalName, Event.Get()));
		return Event->GetID();
	}

	bool RemoveSignalEvent(int32 ID);
	void CleanupSignalEvents(FName SignalName);

	template <typename... ArgsType>
	void InvokeSignal(FName SignalName, ArgsType... Args) {
#if !UE_BUILD_SHIPPING
		UE_LOG(LogCmsSignal, Warning, TEXT("Dispatch[Lambda][%s]"), *SignalName.ToString());
#endif
		if (auto EventArrayPtr = EventNameMap.Find(SignalName)){
			TArray<TSharedPtr<ISignalEvent>> NewEventIter = *EventArrayPtr;
			const std::vector<uint8_t>& ParamsBuffer = TemplateTraits::BuildParamsBuffer(Args...);
			for (TSharedPtr<ISignalEvent>& EventSPtr : NewEventIter){
				if (ISignalEvent* Event = EventSPtr.Get()){
					if (Event->IsNative()) {
						// TODO
					}
					else {
						FSignalEvent_Lambda<TFunction<void(ArgsType...)>>* LambdaEvent =  static_cast<FSignalEvent_Lambda<TFunction<void(ArgsType...)>>*>(Event);
						LambdaEvent->Invoke((void*)ParamsBuffer.data());
					}
					if (Event->IsOnlyOnce()) {
						EventNameMap.FindOrAdd(SignalName).Remove(EventSPtr);
						EventIDMap.Remove(Event->GetID());
					}
				}
			}
		}
	}

private:
	TMap<FName, TArray<TSharedPtr<ISignalEvent>>> EventNameMap;
	TMap<int32, TPair<FName, ISignalEvent*>> EventIDMap;
};
