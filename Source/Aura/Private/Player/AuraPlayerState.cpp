// Copyright Druid Mechanics


#include "Player/AuraPlayerState.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include <AbilitySystem/My_AuraAbilitySystemComponent.h>

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMy_AuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	NetUpdateFrequency = 100.f; // 服务器的更新状态频率
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
