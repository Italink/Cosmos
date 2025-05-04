#pragma once

#include "CosmosGlobals.h"

#define DEFINE_GAMEFLOW_DELEGATE(DelegateType) \
	public: F##DelegateType& Get##DelegateType() { return DelegateType; } \
	private: F##DelegateType DelegateType;

#define DEFINE_GAMEFLOW_DELEGATE_TYPED(DelegateVariable, DelegateType) \
	public: DelegateType& Get##DelegateVariable() { return DelegateVariable; } \
	private: DelegateType DelegateVariable;

template <typename T>
class IGameflow : public TSharedFromThis<T> {
public:
    static T& Get();
    static void Cleanup();
    IGameflow(const IGameflow&) = delete;
    IGameflow& operator=(const IGameflow&) = delete;
protected:
    IGameflow() = default;
    virtual ~IGameflow() = default;
private:
    static TSharedPtr<T> Instance;
};

template <typename T>
TSharedPtr<T> IGameflow<T>::Instance = nullptr;

template <typename T>
T& IGameflow<T>::Get()
{
    if (!Instance.IsValid()) {
        Instance = MakeShared<T>();
    }
    return *Instance;
}

template <typename T>
void IGameflow<T>::Cleanup()
{
    Instance.Reset();
}
