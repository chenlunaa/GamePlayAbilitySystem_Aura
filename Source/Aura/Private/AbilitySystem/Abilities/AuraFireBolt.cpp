// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
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
			ScaledDamage);
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
			ScaledDamage);
	}
}

FString UAuraFireBolt::GetNextLevel(int32 Level)
{
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
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
		ScaledDamage);
}
