#pragma once

#include "CosmosGlobals.h"
#include "Core/Basic/DynamicLambda.h"
#include "ObjectPlus.generated.h"

UCLASS()
class UObjectPlus : public UObject{
    GENERATED_BODY()

protected:
    virtual void BeginDestroy() override;
    
    virtual void ProcessEvent(UFunction* Function, void* Parms) override;

	template <typename FuncType>
	FScriptDelegate CreateLambdaDynamic(FuncType Lambda) {
		UClass* Class = GetClass();
		UDynamicLambdaFunction* NewDynamicLambda = NewObject<UDynamicLambdaFunction>(Class);
		NewDynamicLambda->SetFlags(RF_Transient);
		NewDynamicLambda->SetupLambda(Lambda);
		DynamicLambdas.Add(NewDynamicLambda);
		Class->AddFunctionToFunctionMap(NewDynamicLambda, NewDynamicLambda->GetFName());
		FScriptDelegate ScriptDelegate;
		ScriptDelegate.BindUFunction(this, NewDynamicLambda->GetFName());
		return ScriptDelegate;
	}
public:
    UPROPERTY()
    TArray<TObjectPtr<UDynamicLambdaFunction>> DynamicLambdas;
};

#define BindDynamicLocalLambda(Lambda) Bind(this->CreateLambdaDynamic(Lambda))
#define AddDynamicLocalLambda(Lambda) Add(this->CreateLambdaDynamic(Lambda))