#pragma once

#include "CoreMinimal.h"
#include "CosmosEditorLibrary.generated.h"

UCLASS()
class UCosmosEditorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static void SpawnGS();
};
