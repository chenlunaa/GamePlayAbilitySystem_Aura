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
			"<Title>ELECTROCUTE</>\n\n"
			
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
			"<Default>Emits a beam of Lighting, Connecting with the target, repeatedly causing </>"
			"<Damage>%d</>"
			"<Default> lighting damage with a chance to stun.</>"),
			Level,
			ManaCost,
			CoolDown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>ELECTROCUTE</>\n\n"
			
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
			"<Default>Emits a beam of Lighting, propagating to %d additional targets nearby, causing </>"
			"<Damage>%d</>"
			"<Default> lighting damage with a chance to stun.</>"),
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
		"<Title>NEXT LEVEL</>\n\n"
		
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
		"<Default>Emits a beam of Lighting, propagating to %d additional targets nearby, causing </>"
		"<Damage>%d</>"
		"<Default> lighting damage with a chance to stun.</>"),
		Level,
		ManaCost,
		CoolDown,
		FMath::Min(Level, MaxNumShockTargets),
		ScaledDamage);
}
