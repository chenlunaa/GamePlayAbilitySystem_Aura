// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/ManaSiphonAbility.h"


FString UManaSiphonAbility::GetDescription(int32 Level, float Value)
{
	const float Enhance = LevelBuff.GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
		"<Title>自动回魔</>\n\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Default>每秒恢复 </><ManaCost>%.1f</><Default>点魔力</>"
		
		),
		Level,
		Value + Enhance
		);
}

FString UManaSiphonAbility::GetNextLevel(int32 Level, float Value)
{
	const float Enhance = LevelBuff.GetValueAtLevel(Level);
	return FString::Printf(TEXT(
		"<Title>升级效果</>\n\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Default>每秒恢复 </><ManaCost>%.1f</><Default>点魔力</>"
		
		),
		Level,
		Value + Enhance
		);
}
