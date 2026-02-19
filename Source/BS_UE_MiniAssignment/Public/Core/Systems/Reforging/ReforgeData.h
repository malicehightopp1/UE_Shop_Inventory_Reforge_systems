// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Systems/Utility/DataInfo.h"
#include "ReforgeData.generated.h"


UCLASS(BlueprintType)
class BS_UE_MINIASSIGNMENT_API UReforgeData : public UPrimaryDataAsset //*THIS IS HOW YOU MAKE A DATA ASSET* actual data Asset 
{
	GENERATED_BODY()
public:
	//UFUNCTION(BlueprintCallable, Category = "Weapons | Map") FReforgeDataChanges GetRandomReforge(EWeaponType Type);//the weapon type used
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons | Map") TMap<EWeaponType, FReforgeItemPool> ReforgeMap;//the TMap that holds al the data

	UFUNCTION(BlueprintCallable, Category = "Weapons") FReforgeDataChanges GetRandomReforge();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons") TArray<FReforgeDataChanges> ReforgeData;
};