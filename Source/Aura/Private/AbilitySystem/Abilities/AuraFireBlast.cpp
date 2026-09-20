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
		"<Title>FIRE BLAST</>\n\n"
		
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
		"<Default>Launches %d </>"
		"<Default>fire balls in all direction, each coming back and </>"
		"<Default>exploding upon return, causing </>"
		"<Damage>%d</>"
		"<Default> radial fire damage with a chance to burn.</>"),
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
		"<Title>NEXT LEVEL</>\n\n"
		
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>CoolDown: </><CoolDown>%.1f</>\n\n"
		"<Default>Launches %d </>"
		"<Default>fire balls in all direction, each coming back and </>"
		"<Default>exploding upon return, causing </>"
		"<Damage>%d</>"
		"<Default> fire damage with a chance to burn.</>"),
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
