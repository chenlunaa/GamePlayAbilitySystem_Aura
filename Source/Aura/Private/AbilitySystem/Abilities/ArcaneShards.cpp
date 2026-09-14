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
			"<Title>ARCANE SHARDS</>\n\n"
			
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
			"<Default>Summon a shard of arcane energy causing radial arcane damage of </>"
			"<Damage>%d</>"
			"<Default> at the shard origin.</>"),
			Level,
			ManaCost,
			CoolDown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>ARCANE SHARDS</>\n\n"
			
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
			"<Default>Summon %d shards of arcane energy causing radial arcane damage of </>"
			"<Damage>%d</>"
			"<Default> at the shard origin</>"),
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
		"<Title>NEXT LEVEL</>\n\n"
				
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
		"<Default>Summon %d shards of arcane energy causing radial arcane damage of </>"
		"<Damage>%d</>"
		"<Default> at the shard origin</>"),
		Level,
		ManaCost,
		CoolDown,
		FMath::Min(Level, MaxNumShocks),
		ScaledDamage);
}
