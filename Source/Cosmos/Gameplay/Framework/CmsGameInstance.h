#pragma once

#include "Engine/GameInstance.h"
#include "CmsGameInstance.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UCmsGameInstance : public UGameInstance {
public:
	GENERATED_BODY()
private:
	void Init() override;
};