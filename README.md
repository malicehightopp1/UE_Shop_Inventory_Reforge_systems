# **Unreal Engines Shop/Reforge/Inventoy system**

![Unreal Engine](https://img.shields.io/badge/Engine-Unreal%20Engine%205-blue)

## **Description**
This project is a first person reforge, inventory, and shop system using UE5

## **Engine Version**
- Unreal Engine 5.5.4
- Language: C++
- Make sure to use the correct version to avoid compatibility issues.
	
 # **Table Of Context**
- [Project Features](#Features)
- [Project Architecture](#Architecture-Overview)
- [Project Features](#Features)
- [Contacts](#Contact)


# Features
- Inventory system (stacking, equipment slots)
- Shop system (buy/sell, NPC vendors)
- Reforge system (stat rerolling, modifiers)
- Item system (data-driven via DataTables or Assets)
- UI built with UMG

# Architecture Overview
	
## Core Systems
- `UInventoryComponent`
  - Handles item storage, stacking, and queries
- `UShopComponent`
  - Manages vendor inventory and transactions
- `UReforgeComponent`
  - Handles stat reroll logic and cost scaling
- `UItemDataAsset` / `FItemStruct`
  - Defines item properties (rarity, stats, mesh, etc.)
	
	## Design Approach
	- Connecting systems that work well together
	- Data-driven items using DataTables or Data Assets
	
	Items are defined using:
	- DataTables (`FItemData`)
	
	### Example Fields
	- Name
	- ItemType
	- Sell Price
	- Buy price
	
	## Item Data Struct
	```cpp
	UENUM(BlueprintType)
	enum class EItemType : uint8 
	{
		Weapon UMETA(DisplayName = "Weapon"),
		Armour UMETA(DisplayName = "Armour"),
		Consumable UMETA(DisplayName = "Consumable"),
		Accessory UMETA(DisplayName = "Accesory"),
		Misc UMETA(DisplayName = "Misc")
	};
	USTRUCT(BlueprintType)
	struct FItemDataInfo : public FTableRowBase
	{
		GENERATED_BODY()
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemName;
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
	```

# Controls

| Action | Key |
|------|-----|
| Open Inventory | I |
| Interact | E |
| Open Shop && Reforge Panel | E |
| PLayer Movement | WASD |

# **Contact**
- Developer: Brandin Stanfield
- Email: Brandin.Stanfield@gmail.com
- GitHub: https://github.com/malicehightopp1
