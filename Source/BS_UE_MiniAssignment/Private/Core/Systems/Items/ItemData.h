// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemDataInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FText ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 BuyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 SellPrice;
};
UCLASS()
class UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
};
