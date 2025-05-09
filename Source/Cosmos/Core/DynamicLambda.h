#pragma once

#include "UObject/Class.h"
#include "Subsystems/EngineSubsystem.h"
#include "UObject/UObjectArray.h"
#include "TemplateTraits.h"
#include "DynamicLambda.generated.h"

UCLASS()
class UDynamicLambdaFunction : public UFunction {
    GENERATED_BODY()
public:
    template <typename FuncType>
    void SetupLambda(FuncType InFunc) {
        LambdaInvoker = [InFunc](void* Params) {
            using TupleType = typename TemplateTraits::FuncTraits<FuncType>::TupleType;

            constexpr size_t ArgCount = std::tuple_size<TupleType>::value;
            auto Args = TemplateTraits::ParseParamsFromBuffer<TupleType>(Params, std::make_index_sequence<ArgCount>{});

            std::apply(InFunc, Args);
        };
    }
    void Invoke(void* Params);
protected:
    TFunction<void(void* Params)> LambdaInvoker;
};

UCLASS()
class UDynamicLambdaSubsystem : public UEngineSubsystem{
    GENERATED_BODY()
public:
    template <typename FuncType>
    FScriptDelegate CreateLambdaDynamic(const UObject* Owner, FuncType Lambda) {
        UClass* Class = GetClass();
        UDynamicLambdaFunction* NewDynamicLambda = NewObject<UDynamicLambdaFunction>(Class);
        NewDynamicLambda->SetFlags(RF_Transient);
        NewDynamicLambda->SetupLambda(Lambda);
        Class->AddFunctionToFunctionMap(NewDynamicLambda, NewDynamicLambda->GetFName());
        if (Owner) {
            RegisterDynamicLambdaFunction(Owner, NewDynamicLambda);
        }
        FScriptDelegate ScriptDelegate;
        ScriptDelegate.BindUFunction(this, NewDynamicLambda->GetFName());
        return ScriptDelegate;
    }
    void RemoveAll(const UObjectBase* Owner);
protected:
    void RegisterDynamicLambdaFunction(const UObject* Owner, UDynamicLambdaFunction* Function);

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void ProcessEvent(UFunction* Function, void* Parms) override;
private:
    class UDynamicLambdaSubsystemPrivate* Private = nullptr;
};

#define BindDynamicLambda(OwnerUObject, Lambda) Bind(GEngine->GetEngineSubsystem<UDynamicLambdaSubsystem>()->CreateLambdaDynamic(OwnerUObject, Lambda))
#define AddDynamicLambda(OwnerUObject, Lambda) Add(GEngine->GetEngineSubsystem<UDynamicLambdaSubsystem>()->CreateLambdaDynamic(OwnerUObject, Lambda))