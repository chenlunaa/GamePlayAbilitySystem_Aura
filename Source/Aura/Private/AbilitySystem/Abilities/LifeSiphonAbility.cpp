// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/LifeSiphonAbility.h"

FString ULifeSiphonAbility::GetDescription(int32 Level, float Value)
{
	const float Enhance = LevelBuff.GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
	"<Title>自动回血</>\n\n"
		
	"<Small>等级: </><Level>%d</>\n"
	"<Default>每秒恢复 </><Damage>%.1f</><Default>点血量</>"
		
	),
	Level,
	Value + Enhance
	);
}

FString ULifeSiphonAbility::GetNextLevel(int32 Level, float Value)
{
	const float Enhance = LevelBuff.GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
	"<Title>升级效果</>\n\n"
		
	"<Small>等级: </><Level>%d</>\n"
	"<Default>每秒恢复 </><Damage>%.1f</><Default>点血量</>"
		
	),
	Level,
	Value + Enhance
	);
}
