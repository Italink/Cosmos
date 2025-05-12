#include "ObjectPlus.h"

void UObjectPlus::BeginDestroy()
{
    Super::BeginDestroy();
    if (!HasAnyFlags(RF_ClassDefaultObject) ) {
        UClass* Class = GetClass();
        for (auto DynamicLambda : DynamicLambdas) {
            Class->RemoveFunctionFromFunctionMap(DynamicLambda);
        }
        DynamicLambdas.Empty();
    }
}

void UObjectPlus::ProcessEvent(UFunction* Function, void* Parms)
{
    if (UDynamicLambdaFunction* DynamicLambda = Cast<UDynamicLambdaFunction>(Function)) {
        DynamicLambda->Invoke(Parms);
    }
    else {
        UObject::ProcessEvent(Function, Parms);
    }
}

