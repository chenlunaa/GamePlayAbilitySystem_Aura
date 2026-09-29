// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint/CheckPoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;
	
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;
	
	/* Save Interface*/
	virtual bool ShouldLoadTransform_Implementation() override {return false;}
	virtual void LoadActor_Implementation() override;
	/* end Save Interface*/
	
	virtual void ActivateMutual(AActor* OtherActor) override;
};
