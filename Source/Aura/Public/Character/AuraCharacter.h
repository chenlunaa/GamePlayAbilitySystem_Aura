// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	// 俯视角 RPG 移动方案——角色在平面上移动，自动面向移动方向，相机固定俯拍。
	AAuraCharacter();
	// 服务器端的InitAbilityActorInfo
	virtual void PossessedBy(AController* NewController) override;
	// 客户端的InitAbilityActorInfo
	virtual void OnRep_PlayerState() override;
	
	// Combat Interface
	virtual int32 GetPlayerLevel() override;
private:
	void InitAbilityActorInfo() override;
};
