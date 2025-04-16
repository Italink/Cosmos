#pragma once

#include "CmsLogChannels.h"
#include "UObject/Class.h"
#include "DynamicLambda.generated.h"

namespace DynamicLambdaUtils {
    template<typename Tuple, std::size_t... Indices>
    Tuple ParseParams(void* Params, std::index_sequence<Indices...>) {
        char* Buffer = static_cast<char*>(Params);
        return Tuple{ (*reinterpret_cast<typename std::tuple_element<Indices, Tuple>::type*>(
            Buffer + (Indices * sizeof(typename std::tuple_element<Indices, Tuple>::type))))
            ... };
    }

    template<typename T>
    struct FuncTraits;

    template<typename Ret, typename... Args>
    struct FuncTraits<Ret(Args...)> {
        using TupleType = std::tuple<Args...>;
    };

    template<typename Ret, typename... Args>
    struct FuncTraits<Ret(*)(Args...)> : FuncTraits<Ret(Args...)> {};

    template<typename Ret, typename... Args>
    struct FuncTraits<TFunction<Ret(Args...)>> : FuncTraits<Ret(Args...)> {};

    template<typename Lambda>
    struct FuncTraits : FuncTraits<decltype(&Lambda::operator())> {};

    template<typename ClassType, typename Ret, typename... Args>
    struct FuncTraits<Ret(ClassType::*)(Args...) const> : FuncTraits<Ret(Args...)> {};
};

UCLASS()
class UDynamicLambda : public UFunction {
    GENERATED_BODY()
public:
    template <typename FuncType>
    void SetupLambda(FuncType InFunc) {
        LambdaInvoker = [InFunc](void* Params) {
            using TupleType = typename DynamicLambdaUtils::FuncTraits<FuncType>::TupleType;

            constexpr size_t ArgCount = std::tuple_size<TupleType>::value;
            auto Args = DynamicLambdaUtils::ParseParams<TupleType>(Params, std::make_index_sequence<ArgCount>{});

            std::apply(InFunc, Args);
        };
    }
    void Invoke(void* Params);
protected:
    TFunction<void(void* Params)> LambdaInvoker;
};