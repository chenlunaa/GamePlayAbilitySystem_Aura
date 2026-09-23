// Copyright Druid Mechanics


#include "Actor/AuraFireAround.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraFireAround::AAuraFireAround()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAuraFireAround::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan_Around);
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->Deactivate();
}

void AAuraFireAround::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasAuthority())
	{
		return;
	}

	if (!IsValid(OrbitCenter))
	{
		Destroy();
		return;
	}

	OrbitAngle = FMath::Fmod(OrbitAngle + OrbitDegreesPerSecond * DeltaSeconds, 360.f);
	const FVector RadialDirection = FVector::ForwardVector.RotateAngleAxis(OrbitAngle, FVector::UpVector);
	const FVector OrbitLocation = OrbitCenter->GetActorLocation() + RadialDirection * OrbitRadius + FVector::UpVector * OrbitHeight;

	SetActorLocationAndRotation(OrbitLocation, RadialDirection.Rotation());
}

void AAuraFireAround::InitializeOrbit(AActor* InOrbitCenter, float InOrbitRadius, float InOrbitHeight,
	float InInitialAngle, float InOrbitDegreesPerSecond)
{
	OrbitCenter = InOrbitCenter;
	OrbitRadius = FMath::Max(0.f, InOrbitRadius);
	OrbitHeight = InOrbitHeight;
	OrbitAngle = InInitialAngle;
	OrbitDegreesPerSecond = InOrbitDegreesPerSecond;
}


