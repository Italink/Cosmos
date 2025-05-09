#pragma once

#include "Logging/LogMacros.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

COSMOS_API DECLARE_LOG_CATEGORY_EXTERN(LogCms, Log, All);
COSMOS_API DECLARE_LOG_CATEGORY_EXTERN(LogCmsSignal, Log, All);
COSMOS_API DECLARE_LOG_CATEGORY_EXTERN(LogCmsUI, Log, All);
COSMOS_API DECLARE_LOG_CATEGORY_EXTERN(LogCmsCharacter, Log, All);

template<class T>
inline TSharedPtr<FStreamableHandle> LoadObjectAsync(
	const TCHAR* ObjectPath, 
	TFunction<void(T*)> Callback,
	TAsyncLoadPriority Priority = 0,
	bool bManageActiveHandle = false,
	bool bStartStalled = false,
	FString DebugNameOrLocation = FString())
{
	FSoftObjectPath SoftObjectPath(ObjectPath);
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	FStreamableDelegate StreamableDelegate = FStreamableDelegate::CreateLambda([SoftObjectPath, Callback]() {
		FSoftObjectPtr SoftObjPtr = FSoftObjectPtr(SoftObjectPath);
		UObject* Obj = SoftObjPtr.Get();
		if (Callback) {
			Callback((T*)Obj);
		}
	});
	return StreamableManager.RequestAsyncLoad(SoftObjectPath, StreamableDelegate, Priority, bManageActiveHandle, bStartStalled, DebugNameOrLocation);
}