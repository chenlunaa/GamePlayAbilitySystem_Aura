// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName ActorName = FName();
	
	UPROPERTY()
	FTransform Transform = FTransform();
	
	// 保存Actor中序列化的变量
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString MapAssetName = FString();
	
	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel;
};
inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}


/**
 * 保存的值都是数值而不是指针，无法保证下次运行时该地址还是指向原来的内存地址
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	FString SlotName = FString();
	
	UPROPERTY()
	int32 SlotIndex = 0;
	
	UPROPERTY()
	FString PlayerName = FString("Default Name");
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;
	
	UPROPERTY()
	FString MapName = FString("Default Name");
	
	UPROPERTY()
	FName PlayerStartTag;
	
	UPROPERTY()
	bool bFirstTimeLoad = true;
	/*
	 * Player
	 */
	
	UPROPERTY()
	int32 PlayerLevel = 1;
	
	UPROPERTY()
	int32 XP = 0;
	
	UPROPERTY()
	int32 SpellPoints = 0;
	
	UPROPERTY()
	int32 AttributePoints = 0;
	
	/* Attributes */
	UPROPERTY()
	float Strength = 0.f;
	
	UPROPERTY()
	float Intelligence = 0.f;
	
	
	UPROPERTY()
	float Resilience = 0.f;
	
	UPROPERTY()
	float Vigor = 0.f;
	
	/* SavedAbilities*/
	
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

	UPROPERTY()
	TArray<FSavedMap> SavedMaps;
	
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
};
