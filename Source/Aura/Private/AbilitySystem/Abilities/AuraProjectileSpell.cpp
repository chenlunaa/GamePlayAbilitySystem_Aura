// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Aura/Public/AuraGameplayTags.h"

FString UAuraProjectileSpell::GetDescription(int32 Level)
{
	return Super::GetDescription(Level);
}

FString UAuraProjectileSpell::GetNextLevel(int32 Level)
{
	return Super::GetNextLevel(Level);
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotator = (ProjectileTargetLocation - SocketLocation).Rotation(); 
	
	if (bOverridePitch)
	{
		Rotator.Pitch = PitchOverride;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotator.Quaternion());

		
	AAuraProjectile* Projectile= GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass, 
		SpawnTransform, 
		GetAvatarActorFromActorInfo(), 
		Cast<APawn>(GetAvatarActorFromActorInfo()), 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		
	// 手动构建EffectContextHandle来传递信息给AttributeSet
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);
		
		
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
		
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		
	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}
		
	Projectile->DamageEffectSpecHandle = SpecHandle;
		
	Projectile->FinishSpawning(SpawnTransform);
	
}
