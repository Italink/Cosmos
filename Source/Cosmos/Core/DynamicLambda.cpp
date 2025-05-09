#include "DynamicLambda.h"

void UDynamicLambdaFunction::Invoke(void* Params)
{
	if (LambdaInvoker) {
		LambdaInvoker(Params);
	}
}

class UDynamicLambdaSubsystemPrivate : public FUObjectArray::FUObjectDeleteListener, public FGCObject {
public:
	UDynamicLambdaSubsystemPrivate(UDynamicLambdaSubsystem* InPublic) {
		Public = InPublic;
		GUObjectArray.AddUObjectDeleteListener(this);
	}
	~UDynamicLambdaSubsystemPrivate() {
		GUObjectArray.RemoveUObjectDeleteListener(this);
	}
	void RemoveAll(const UObjectBase* Owner){
		if (auto FunctionsPtr = DynamicLambdaFunctionsMap.Find(Owner)) {
			for (auto Function : *FunctionsPtr) {
				UClass* Class = Public->GetClass();
				Class->RemoveFunctionFromFunctionMap(Function);
			}
		}
	}
	void RegisterDynamicLambdaFunction(const UObjectBase* Owner, UDynamicLambdaFunction* Function) {
		DynamicLambdaFunctionsMap.FindOrAdd(Owner).Add(Function);
	}
protected:
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override {
		for (auto DynamicLambdaFunctions : DynamicLambdaFunctionsMap) {
			Collector.AddReferencedObjects(DynamicLambdaFunctions.Value);
		}
	}
	virtual FString GetReferencerName() const override {
		return TEXT("DynamicLambda");
	}
	virtual void NotifyUObjectDeleted(const class UObjectBase* Object, int32 Index) override {
		RemoveAll(Object);
	}
	virtual void OnUObjectArrayShutdown() override {
		GUObjectArray.RemoveUObjectDeleteListener(this);
	}
private:
	UDynamicLambdaSubsystem* Public = nullptr;
	TMap<const UObjectBase*, TArray<UDynamicLambdaFunction*>> DynamicLambdaFunctionsMap;
};

void UDynamicLambdaSubsystem::RemoveAll(const UObjectBase* Owner)
{
	Private->RemoveAll(Owner);
}

void UDynamicLambdaSubsystem::RegisterDynamicLambdaFunction(const UObject* Owner, UDynamicLambdaFunction* Function)
{
	Private->RegisterDynamicLambdaFunction(Owner, Function);
}

void UDynamicLambdaSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Private = new UDynamicLambdaSubsystemPrivate(this);
}

void UDynamicLambdaSubsystem::Deinitialize()
{
	delete Private;
}

void UDynamicLambdaSubsystem::ProcessEvent(UFunction* Function, void* Parms)
{
	if (UDynamicLambdaFunction* DynamicLambda = Cast<UDynamicLambdaFunction>(Function)) {
		DynamicLambda->Invoke(Parms);
	}
	else {
		UObject::ProcessEvent(Function, Parms);
	}
}
