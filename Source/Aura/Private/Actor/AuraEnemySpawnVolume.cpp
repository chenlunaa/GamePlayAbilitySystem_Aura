// Copyright Druid Mechanics


#include "Actor/AuraEnemySpawnVolume.h"

#include "Actor/AuraEnemySpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"

// Sets default values
AAuraEnemySpawnVolume::AAuraEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpawnMesh");
	SpawnMesh->SetupAttachment(GetRootComponent());
	SpawnMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SpawnMesh->SetCollisionResponseToAllChannels(ECR_Block);
	
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(SpawnMesh);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AAuraEnemySpawnVolume::LoadActor_Implementation()
{
	if (bIsMutual)
	{
		Box->SetVisibility(false);
		HandleGlowEffects();
	}
}

void AAuraEnemySpawnVolume::ActivateMutual(AActor* OtherActor)
{
	if (bIsMutual) return;
	
	if (!OtherActor->Implements<UPlayerInterface>()) return;
	for (AAuraEnemySpawnPoint* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy();
		} 
	}
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandleGlowEffects();
	bIsMutual = true;
}


void AAuraEnemySpawnVolume::HandleGlowEffects()
{
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(SpawnMesh->GetMaterial(0), this);
	SpawnMesh->SetMaterial(0, DynamicMaterialInstance);
	CheckPointReached(DynamicMaterialInstance);
}

void AAuraEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}


