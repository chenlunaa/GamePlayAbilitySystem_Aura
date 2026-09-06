// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
			"<Default>Launches a firebolt that explodes on impact, dealing </>"
			"<Damage>%d</>"
			"<Default> fire damage with a chance to burn.</>"),
			Level,
			ManaCost,
			CoolDown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
			"<Default>Launches %d firebolts that explodes on impact, dealing </>"
			"<Damage>%d</>"
			"<Default> fire damage with a chance to burn.</>"),
			Level,
			ManaCost,
			CoolDown,
			FMath::Min(Level, NumProjectiles),
			Damage);
	}
}

FString UAuraFireBolt::GetNextLevel(int32 Level)
{
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	const int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL</>\n\n"
		
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
		"<Default>Launches %d firebolts that explodes on impact, dealing </>"
		"<Damage>%d</><Default> fire damage with a chance to burn.</>\n\n"),
		Level,
		ManaCost,
		CoolDown,
		FMath::Min(Level, NumProjectiles),
		Damage);
}
