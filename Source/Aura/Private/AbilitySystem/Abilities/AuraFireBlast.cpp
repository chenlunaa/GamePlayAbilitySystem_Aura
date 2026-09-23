// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	return FString::Printf(TEXT(
		"<Title>火焰爆发</>\n\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Small>魔力消耗: </><ManaCost>%.1f</>\n"
		"<Small>冷却: </><CoolDown>%.1f</>\n\n"
		"<Default>向四周发射 %d </>"
		"<Default>枚火球, 每一个都会返回并且 </>"
		"<Default>最后发生爆炸, 造成 </>"
		"<Damage>%d</>"
		"<Default> 点范围火焰伤害并且有一定概率触发燃烧.</>"),
		Level,
		ManaCost,
		CoolDown,
		MaxNumFireBolls,
		ScaledDamage);
}

FString UAuraFireBlast::GetNextLevel(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	return FString::Printf(TEXT(
		"<Title>升级效果</>\n\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Small>魔力消耗: </><ManaCost>%.1f</>\n"
		"<Small>冷却: </><CoolDown>%.1f</>\n\n"
		"<Default>向四周发射 %d </>"
		"<Default>枚火球, 每一个都会返回并且 </>"
		"<Default>最后发生爆炸, 造成 </>"
		"<Damage>%d</>"
		"<Default> 点范围火焰伤害并且有一定概率触发燃烧.</>"),
		Level,
		ManaCost,
		CoolDown,
		MaxNumFireBolls,
		ScaledDamage);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 330.f, MaxNumFireBolls);
	
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass, 
			SpawnTransform, 
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
		
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefault();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefault();
		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(SpawnTransform);
	}
	
	return FireBalls;
}
