// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/FireAround.h"

#include "Actor/AuraFireAround.h"
#include "GameFramework/Pawn.h"

FString UFireAround::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	return FString::Printf(TEXT(
		"<Title>火球护身</>\n\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Small>魔力消耗: </><ManaCost>%.1f</>\n"
		"<Small>冷却: </><CoolDown>%.1f</>\n\n"
		"<Default>生成 %d </>"
		"<Default>枚火球, 火球会围绕角色运动 </>"
		"<Default>最后发生爆炸, 造成 </>"
		"<Damage>%d</>"
		"<Default> 点范围火焰伤害并且有一定概率触发燃烧.</>"),
		Level,
		ManaCost,
		CoolDown,
		FMath::Min(Level + 2, MaxNumFireBolls),
		ScaledDamage);
}

FString UFireAround::GetNextLevel(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = -GetManaCost(Level);
	const float CoolDown = GetCoolDown(Level);
	
	return FString::Printf(TEXT(
		"<Title>升级效果</>\n\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Small>魔力消耗: </><ManaCost>%.1f</>\n"
		"<Small>冷却: </><CoolDown>%.1f</>\n\n"
		"<Default>生成 %d </>"
		"<Default>枚火球, 火球会围绕角色运动 </>"
		"<Default>最后发生爆炸, 造成 </>"
		"<Damage>%d</>"
		"<Default> 点范围火焰伤害并且有一定概率触发燃烧.</>"),
		Level,
		ManaCost,
		CoolDown,
		FMath::Min(Level + 2, MaxNumFireBolls),
		ScaledDamage);
}

TArray<AAuraFireAround*> UFireAround::SpawnFireBoll()
{
	TArray<AAuraFireAround*> FireBalls;
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!IsValid(AvatarActor) || !IsValid(GetWorld()) || !FireBallClass || !AvatarActor->HasAuthority())
	{
		return FireBalls;
	}

	const int32 NumFireBalls = FMath::Min(GetAbilityLevel() + 2, MaxNumFireBolls);
	if (NumFireBalls == 0)
	{
		return FireBalls;
	}

	FireBalls.Reserve(NumFireBalls);
	const FVector CenterLocation = AvatarActor->GetActorLocation();
	const FVector Forward = AvatarActor->GetActorForwardVector().GetSafeNormal2D();
	const float AngleStep = 360.f / NumFireBalls;

	for (int32 Index = 0; Index < NumFireBalls; ++Index)
	{
		const float Angle = AngleStep * Index;
		const FVector RadialDirection = Forward.RotateAngleAxis(Angle, FVector::UpVector);
		const FVector SpawnLocation = CenterLocation + RadialDirection * SpawnRadius + FVector::UpVector * SpawnHeight;
		const FTransform SpawnTransform(RadialDirection.Rotation(), SpawnLocation);

		AAuraFireAround* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireAround>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(AvatarActor),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);

		if (!IsValid(FireBall))
		{
			continue;
		}

		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefault();
		FireBall->SetOwner(AvatarActor);
		FireBall->InitializeOrbit(AvatarActor, SpawnRadius, SpawnHeight,
			RadialDirection.Rotation().Yaw, OrbitDegreesPerSecond);
		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefault();
		FireBall->FinishSpawning(SpawnTransform);
		FireBalls.Add(FireBall);
	}

	return FireBalls;
}
