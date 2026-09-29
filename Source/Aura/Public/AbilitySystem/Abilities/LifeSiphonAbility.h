// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraPassiveAbility.h"
#include "LifeSiphonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULifeSiphonAbility : public UAuraPassiveAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level, float Value) override;
	virtual FString GetNextLevel(int32 Level, float Value) override;
};
