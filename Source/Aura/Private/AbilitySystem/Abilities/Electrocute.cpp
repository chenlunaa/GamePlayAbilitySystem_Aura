// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>雷电光束</>\n\n"
			
			"<Small>等级: </><Level>%d</>\n"
			"<Small>魔力消耗: </><ManaCost>%.1f</>\n"
			"<Small>冷却: </><CoolDown>%.1f</>\n\n"
			"<Default>激发一束雷电, 会连接目标敌人, 反复造成 </>"
			"<Damage>%d</>"
			"<Default> 点雷电伤害并有概率附加晕眩.</>"),
			Level,
			ManaCost,
			CoolDown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>雷电光束</>\n\n"
			
			"<Small>等级: </><Level>%d</>\n"
			"<Small>魔力消耗: </><ManaCost>%.1f</>\n"
			"<Small>冷却: </><CoolDown>%.1f</>\n\n"
			"<Default>激发一束雷电, 会连接 %d 个附加的额外目标, 反复造成 </>"
			"<Damage>%d</>"
			"<Default> 点雷电伤害并有概率附加晕眩.</>"),
			Level,
			ManaCost,
			CoolDown,
			FMath::Min(Level, MaxNumShockTargets),
			ScaledDamage);
	}
}

FString UElectrocute::GetNextLevel(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	
	return FString::Printf(TEXT(
		"<Title>升级效果</>\n\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Small>魔力消耗: </><ManaCost>%.1f</>\n"
		"<Small>冷却: </><CoolDown>%.1f</>\n\n"
		"<Default>激发一束雷电, 会连接 %d 个附加的额外目标, 反复造成 </>"
		"<Damage>%d</>"
		"<Default> 点雷电伤害并有概率附加晕眩.</>"),
		Level,
		ManaCost,
		CoolDown,
		FMath::Min(Level, MaxNumShockTargets),
		ScaledDamage);
}
