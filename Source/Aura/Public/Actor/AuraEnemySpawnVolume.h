// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/MutualInterface.h"
#include "Interaction/SaveInterface.h"
#include "AuraEnemySpawnVolume.generated.h"

class AAuraEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class AURA_API AAuraEnemySpawnVolume : public AActor, public ISaveInterface, public IMutualInterface
{
	GENERATED_BODY()
	
public:	
	AAuraEnemySpawnVolume();
	
	/* Save Interface*/
	virtual void LoadActor_Implementation() override;
	/* Mutual Interface*/
	virtual void ActivateMutual(AActor* OtherActor) override;
	
	UPROPERTY(EditAnywhere)
	TArray<AAuraEnemySpawnPoint*> SpawnPoints;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> SpawnMesh;
	
	UPROPERTY(SaveGame, BlueprintReadOnly)
	bool bIsMutual = false;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintCallable)
	void HandleGlowEffects();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
	
};
