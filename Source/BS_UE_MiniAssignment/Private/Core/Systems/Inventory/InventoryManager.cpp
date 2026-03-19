// Brandin stanfield


#include "Core/Systems/Inventory/InventoryManager.h"

#include "Blueprint/UserWidget.h"
#include "Core/Systems/Items/ItemData.h"

UInventoryManager::UInventoryManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UInventoryManager::BeginPlay()
{
	Super::BeginPlay();
	
	InventorySlots.Init(FInventorySlot(), MaxSlots); //Pre filling the slots so theyre empty
}

void UInventoryManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
bool UInventoryManager::AddItemToInventory(FName ItemRowName, int32 Quantity) //adding items to the inventory and checking whether stackable and or adding to empty slot
{
	if (!ItemDataTable) return false;
	
	FItemDataInfo* ItemData = ItemDataTable->FindRow<FItemDataInfo>(ItemRowName, TEXT("")); //grabbing the item data off the data table
	
	if (ItemData->bIsStackable) //trying to stack if item is stackable
	{
		int32 ExistingIndex = FindSlotIndex(ItemRowName);
		if (ExistingIndex != -1) //seeing if we have that item already (-1 means not found)
		{
			InventorySlots[ExistingIndex].Quantity += Quantity; //just increase the quantity of said item
			OnInventoryChanged.Broadcast(); //telling ui to update
			return true;
		}
	}
		int32 EmptySlotIndex = FindEmptySlotIndex(); //finding an empty slot in the inventory system
		
		if (EmptySlotIndex == -1) return false;
		
		//fill the empty slot with new data
		InventorySlots[EmptySlotIndex].ItemRowName = ItemRowName;
		InventorySlots[EmptySlotIndex].Quantity = Quantity;
		InventorySlots[EmptySlotIndex].bIsEmpty = false;
		
		OnInventoryChanged.Broadcast(); //update UI again
		return true;
}

bool UInventoryManager::RemoveItemFromInventory(FName ItemRowName, int32 Quantity)
{
	int32 Index = FindSlotIndex(ItemRowName);
	if (Index == -1) return false;
	
	InventorySlots[Index].Quantity -= Quantity; //subtracting the amount by the quantity 
	
	if (InventorySlots[Index].Quantity <= 0) //check if we removed all the items in the slot AKA making it empty
	{
		InventorySlots[Index] = FInventorySlot();
	}
	
	OnInventoryChanged.Broadcast(); //updating UI
	return true;
}

bool UInventoryManager::Hasitem(FName ItemRowName, int32 Quantity)
{
	//Searches and checks for the amount of an item and returns it 
	//Could be used for crafting AKA check if the player has 5 and 3 of an item then crafts one
	return GetItemQuantity(ItemRowName) >= Quantity;
}

int32 UInventoryManager::GetItemQuantity(FName ItemRowName)
{
	int32 Index = FindSlotIndex(ItemRowName);
	if (Index == -1) return 0; //if none are found still give a number back 
	return InventorySlots[Index].Quantity; //gives how many items we have in that slot
}

bool UInventoryManager::IsInventoryFull()
{
	return FindEmptySlotIndex() == -1; //if there is no empty slots check
}

// ========================================================================================================
// ------ Private helpers ---------------------------------------------------------------------------------
// ========================================================================================================
int32 UInventoryManager::FindSlotIndex(FName ItemRowName)
{
	//loops through inventory slots
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		//skips slots that are empty
		//makes sure slots hold the item were looking for
		if (!InventorySlots[i].bIsEmpty && InventorySlots[i].ItemRowName == ItemRowName)
		{
			return i;
		}
	}
	return -1;
}

int32 UInventoryManager::FindEmptySlotIndex()
{
	//same as findslotindex but looks for empty slots instead 
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		if (InventorySlots[i].bIsEmpty) //checks for empty slots so it knows which ones are empty
		{
			return i;
		}
	}
	return -1;
}
// ========================================================================================================
// ------ UI Updating -------------------------------------------------------------------------------------
// ========================================================================================================
FInventorySlot UInventoryManager::GetSlotAtIndex(int32 SlotIndex) const
{
	if (InventorySlots.IsValidIndex(SlotIndex))
	{
		return InventorySlots[SlotIndex];
	}
	return FInventorySlot();
}

void UInventoryManager::RefreshInventory() //call to refresh the inventory, updating inventory UI
{
	OnInventoryChanged.Broadcast();
}

void UInventoryManager::Inventory() //for turning on and off the UI
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	UE_LOG(LogTemp, Warning, TEXT("InventoryManager::Inventory"));
	if (InventoryWidgetInstance && InventoryWidgetInstance->IsInViewport() && bIsInventoryOpen == true) //checking if open, close it
	{
		InventoryWidgetInstance->RemoveFromParent();
		InventoryWidgetInstance = nullptr;
		bIsInventoryOpen = false;

		PC->SetShowMouseCursor(false);
		PC->SetIgnoreLookInput(false);
		PC->SetIgnoreMoveInput(false);
		return;
	}

	if (InventoryWidgetClass) //create the inventory Ui if not open
	{
		InventoryWidgetInstance = CreateWidget<UUserWidget>(PC, InventoryWidgetClass);
		if (InventoryWidgetInstance && bIsInventoryOpen == false)
		{
			InventoryWidgetInstance->AddToViewport();

			bIsInventoryOpen = true;
			PC->SetShowMouseCursor(true);
			PC->SetIgnoreLookInput(true);
			PC->SetIgnoreMoveInput(true);
		}
	}
}

