// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Systems/Items/Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType) enum class EWeaponType : uint8
{
	Sword,
	Axe,
	Hammer
};
UCLASS()
class BS_UE_MINIASSIGNMENT_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons | Types") EWeaponType WeaponType; 
};
