// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Systems/Items/Item.h"
#include "Core/Systems/Reforging/ReforgeData.h"
#include "Core/Systems/Utility/DataInfo.h"
#include "Weapon.generated.h"

UCLASS(BlueprintType)
class BS_UE_MINIASSIGNMENT_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons | Types") EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons | Reforge") UReforgeData* ReforgeData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons | Reforge") FReforgeDataChanges WeaponStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons | Reforge") FString WeaponName;
};
