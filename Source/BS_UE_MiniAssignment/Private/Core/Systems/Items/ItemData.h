// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8 
{
	Weapon UMETA(DisplayName = "Weapon"),
	Armour UMETA(DisplayName = "Armour"),
	Consumable UMETA(DisplayName = "Consumable"),
	Accessory UMETA(DisplayName = "Accesory"),
	Misc UMETA(DisplayName = "Misc"),
	All UMETA(DisplayName = "All")
};
USTRUCT(BlueprintType)
struct FItemDataInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	//Might add description *remember that will delete all Data on dataTable* 
	//Item rarity *for mini assignment* ccould just be another struct
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FText ItemDescription; //this might be crashing Unreal 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 BuyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 SellPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) UTexture2D* ItemTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) bool bIsStackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) EItemType ItemType;
};
UCLASS()
class UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
};
