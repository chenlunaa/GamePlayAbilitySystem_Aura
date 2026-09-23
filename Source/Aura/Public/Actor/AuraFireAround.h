// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraFireBall.h"
#include "AuraFireAround.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraFireAround : public AAuraFireBall
{
	GENERATED_BODY()

public:
	AAuraFireAround();
	virtual void Tick(float DeltaSeconds) override;

	void InitializeOrbit(AActor* InOrbitCenter, float InOrbitRadius, float InOrbitHeight, float InInitialAngle, float InOrbitDegreesPerSecond);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<AActor> OrbitCenter;
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan_Around = 5.f;

	float OrbitRadius = 150.f;
	float OrbitHeight = 80.f;
	float OrbitAngle = 0.f;
	float OrbitDegreesPerSecond = 90.f;
};
