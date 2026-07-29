// Copyright Druid Mechanics


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Player/AuraPlayerState.h>
#include "AbilitySystemComponent.h"
#include <Player/AuraPlayerController.h>
#include <UI/HUD/AuraHUD.h>

#include "AbilitySystem/AuraAbilitySystemComponent.h"

AAuraCharacter::AAuraCharacter()
{
	// 角色自动朝向移动方向旋转
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 仅 Yaw 轴每秒旋转 400°，Pitch/Roll 不旋转
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// 约束在平面上移动（俯视角游戏标配）
	GetCharacterMovement()->bConstrainToPlane = true;
	// 开始时自动吸附到平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 禁止控制器旋转影响角色朝向
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 服务器端调用
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 客户端调用
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

// InitAbilityActorInfo() 的作用是 建立 GAS 中关键对象之间的关联关系，让 Ability System 知道"谁是谁"
// 相当于是初始化AbilitySystemComponent
void AAuraCharacter::InitAbilityActorInfo()
{
	// PlayerState和PlayerController绑定在了GameMode里面，因此是通过Controller来访问PlayerState的
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();

}
