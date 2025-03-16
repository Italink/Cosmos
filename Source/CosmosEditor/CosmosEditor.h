// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"
#include "Materials/MaterialInterface.h"
#include "Styling/SlateBrush.h"

class FCosmosEditorModule: public IModuleInterface 
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
}; 
