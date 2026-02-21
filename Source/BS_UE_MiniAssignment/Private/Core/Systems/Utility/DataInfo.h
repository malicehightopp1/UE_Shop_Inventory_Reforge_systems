// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataInfo.generated.h"

USTRUCT(BlueprintType)
struct FReforgeDataChanges //What could happen in any given reroll of stats
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reforge") FString Prefixes; //name EX. Godly, Broken, Damaged, ETC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reforge") float StatMultiplier; //random stat multiplier
};
USTRUCT(BlueprintType)
struct FReforgeItemPool //for all possiable reforge for a specific weapon type
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FReforgeDataChanges> ReforgeData; //used because a TMap cannot directly use an Enum to a TArray, so allows the TMap to "USE" the TArray
};

UENUM(BlueprintType) enum class EWeaponType : uint8 //what type my weapon is
{
	Sword,
	Axe,
	Hammer
};
/**
 * 
 */
UCLASS()
class UDataInfo : public UObject
{
	GENERATED_BODY()
	
};
