// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>奥术突刺</>\n\n"
			
			"<Small>等级: </><Level>%d</>\n"
			"<Small>魔力消耗: </><ManaCost>%.1f</>\n"
			"<Small>冷却: </><CoolDown>%.1f</>\n\n"
			"<Default>召唤一块奥术能量碎片, 在碎片周边范围性造成 </>"
			"<Damage>%d</>"
			"<Default> 点奥术伤害.</>"),
			Level,
			ManaCost,
			CoolDown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>奥术突刺</>\n\n"
			
			"<Small>等级: </><Level>%d</>\n"
			"<Small>魔力消耗: </><ManaCost>%.1f</>\n"
			"<Small>冷却: </><CoolDown>%.1f</>\n\n"
			"<Default>召唤 %d 块奥术能量碎片, 在碎片周边范围性造成 </>"
			"<Damage>%d</>"
			"<Default> 点奥术伤害.</>"),
			Level,
			ManaCost,
			CoolDown,
			FMath::Min(Level, MaxNumShocks),
			ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevel(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	
	return FString::Printf(TEXT(
		"<Title>升级效果</>\n\n"
				
		"<Small>等级: </><Level>%d</>\n"
		"<Small>魔力消耗: </><ManaCost>%.1f</>\n"
		"<Small>冷却: </><CoolDown>%.1f</>\n\n"
		"<Default>召唤 %d 块奥术能量碎片, 在碎片周边范围性造成 </>"
		"<Damage>%d</>"
		"<Default> 点奥术伤害.</>"),
		Level,
		ManaCost,
		CoolDown,
		FMath::Min(Level, MaxNumShocks),
		ScaledDamage);
}
