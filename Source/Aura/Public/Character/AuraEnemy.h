// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h" // 必须是最后一个include
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	// 定义模板方法并且定义接口
	AAuraEnemy();

	// 敌人接口
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// 结束敌人接口
protected:
	virtual void BeginPlay() override;
};
