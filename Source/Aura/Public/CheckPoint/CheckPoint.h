// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/MutualInterface.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_API ACheckPoint : public APlayerStart, public IMutualInterface, public ISaveInterface
{
	GENERATED_BODY()
	
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);
	
	/* Save Interface*/
	virtual bool ShouldLoadTransform_Implementation() override {return false;}
	virtual void LoadActor_Implementation() override;
	/* end Save Interface*/
	
	/* MutualInterface*/
	virtual void ActivateMutual(AActor* OtherActor) override;	
	/* end Mutual Interface*/
	
	// 只有有SaveGame标记的UPROPERTY才会被序列化
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;
	
protected:
	
	// virtual void OnSphereOverride(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void FinishSave();
	
	void HandleGlowEffects();

private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
	bool bIsFirst = true;
};
