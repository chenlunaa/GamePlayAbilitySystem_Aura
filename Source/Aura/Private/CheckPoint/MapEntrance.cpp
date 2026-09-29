// Copyright Druid Mechanics


#include "CheckPoint/MapEntrance.h"

#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

void AMapEntrance::LoadActor_Implementation()
{
	
}

void AMapEntrance::ActivateMutual(AActor* OtherActor)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		
		bReached = true;
		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
		FinishSave();
		
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}	
}
