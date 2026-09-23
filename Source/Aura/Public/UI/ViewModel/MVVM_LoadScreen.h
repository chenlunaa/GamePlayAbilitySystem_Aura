// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	
	void InitializeLoadSlots();
	
	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelectedDelegate;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;
	
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;
	
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnterName);
	
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);
	
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);
	
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();
	
	// 只负责加载Load界面所需要的数据
	void LoadData();
	
	
private:
	UPROPERTY()
	UMVVM_LoadSlot* SelectedSlot;
	
	UPROPERTY(EditDefaultsOnly, Category = "Load Slots", meta = (ClampMin = "1"))
	int32 NumLoadSlots = 9;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMVVM_LoadSlot>> LoadSlots;
};
