// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MutualInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMutualInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IMutualInterface
{
	GENERATED_BODY()
public:
	
	virtual void ActivateMutual(AActor* OtherActor) = 0;
};
