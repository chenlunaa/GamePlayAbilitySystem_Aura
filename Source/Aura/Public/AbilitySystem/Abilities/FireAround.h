// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "FireAround.generated.h"

class AAuraFireAround;
/**
 * 
 */
UCLASS()
class AURA_API UFireAround : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevel(int32 Level) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireAround*> SpawnFireBoll();
	
	UPROPERTY(EditDefaultsOnly, Category="FireAround", meta=(ClampMin="0.0"))
	float SpawnRadius = 150.f;

	UPROPERTY(EditDefaultsOnly, Category="FireAround")
	float SpawnHeight = 80.f;

	UPROPERTY(EditDefaultsOnly, Category="FireAround")
	float OrbitDegreesPerSecond = 90.f;
	
private:
	
	UPROPERTY(EditDefaultsOnly, Category="FireAround", meta=(ClampMin="0"))
	int32 MaxNumFireBolls = 7;
	
	UPROPERTY(EditDefaultsOnly, Category="FireAround")
	TSubclassOf<AAuraFireAround> FireBallClass;
};
