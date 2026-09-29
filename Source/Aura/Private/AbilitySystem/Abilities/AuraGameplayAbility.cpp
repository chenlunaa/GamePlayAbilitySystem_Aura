// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level, float Value)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum, LoremIpsum, LoremIpsum, LoremIpsum", Level);
}

FString UAuraGameplayAbility::GetNextLevel(int32 Level, float Value)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default> Causes more Damage </>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>该技能需要达到等级: %d</>\n"
		"<Default>并且解锁前置技能才能解锁</>"), 
		Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (auto Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCoolDown(float InLevel) const 
{
	float CoolDown = 0.f;
	if (const UGameplayEffect* CoolDownEffect = GetCooldownGameplayEffect())
	{
		CoolDownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, CoolDown);
	}
	return CoolDown;
}
