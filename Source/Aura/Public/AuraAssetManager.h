// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	
	static UAuraAssetManager& Get();
	
protected:
	
	// StartInitialLoading() 在所有资产加载之前调用，是注册原生标签的最佳时机。
	virtual void StartInitialLoading() override;
};
