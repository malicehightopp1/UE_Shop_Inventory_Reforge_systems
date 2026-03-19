// Brandin stanfield


#include "Core/Systems/Shop System/ShopMechanic.h"

#include "Components/BoxComponent.h"
#include "Components/ScrollBox.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Systems/Characters/MyCharacter.h"
#include "Core/Systems/Inventory/InventoryManager.h"
#include "Core/Systems/Items/ItemData.h"

/*
 * Handles input of shop system, Ui on shop, and buying/selling of items
 * Reads off Data Table of items to set values on the widgets
 * 
 * TODO Buying and selling items, along with rework to bring up Reforge UI.
 *
 * - button to call reforgeItem - bind function to button
 * - bring up Ui when interaction happens - easy part
 */
AShopMechanic::AShopMechanic()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//Player detection for UI
	SphereDetectionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Player Detection"));
	SphereDetectionComponent->SetupAttachment(RootComponent);
	
	//Setting overlap event on
	SphereDetectionComponent->SetGenerateOverlapEvents(true);
	SphereDetectionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereDetectionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	//default values - Size
	ShopDefaultDetection = 300.0f;
	SphereDetectionComponent->SetSphereRadius(ShopDefaultDetection); //default sphere radius
	
	//NPC Hit box
	BoxDetectionComponent = CreateDefaultSubobject<UBoxComponent>("Interaction Box Component");
	BoxDetectionComponent->SetupAttachment(SphereDetectionComponent);
	BoxDetectionComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	//Creating widget
	WidgetDetectionComponent = CreateDefaultSubobject<UWidgetComponent>("Player Interaction Widget");
	
	WidgetDetectionComponent->SetupAttachment(SphereDetectionComponent);
	WidgetDetectionComponent->SetVisibility(false);
	WidgetDetectionComponent->SetWidgetSpace(EWidgetSpace::Screen);
}
void AShopMechanic::BeginPlay()
{
	Super::BeginPlay();
	SphereDetectionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShopMechanic::OnOverlapBegin); //binding sphere to begin overlap
	SphereDetectionComponent->OnComponentEndOverlap.AddDynamic(this, &AShopMechanic::OnOverlapEnd); //binding sphere to end overlap
	
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn(); //getting player pawn to grab the inventory system
	if (PlayerPawn)
	{
		inventoryManager = PlayerPawn->FindComponentByClass<UInventoryManager>();
	}
}

void AShopMechanic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Shop Mechanics / interaction with shop systems
void AShopMechanic::InteractPure(AMyCharacter* player) //interaction
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	AMyCharacter* character = Cast<AMyCharacter>(PC->GetPawn());
	
	UInventoryManager* inventory = character->FindComponentByClass<UInventoryManager>();
	
	if (bPlayerInRange && bShopOpen == false) // turn on the UI
	{
		SetupShopSystem();
		inventory->Inventory();
		character->bPlayerInShop = true;
		if (PC) //locking movement and turning cursor on 
		{
			PC->SetShowMouseCursor(true);
			PC->SetIgnoreMoveInput(true);
			PC->SetIgnoreLookInput(true);
		}
	}
	else if (bShopOpen == true) //if shop is active turn it off
	{
		if (ShopSystemUIInstance)
		{
			ShopSystemUIInstance->RemoveFromParent();
		}
		bShopOpen = false;
		character->bPlayerInShop = false;
		inventory->Inventory();
		if (PC)
		{
			PC->SetIgnoreMoveInput(false);
			PC->SetIgnoreLookInput(false);
			PC->bShowMouseCursor = false;
		}
	}
}

//actually buying the item and using the players current currency
void AShopMechanic::BuyItem(FName ItemKey, AMyCharacter* Player) //TODO buying and selling items from the shop. Right now just have it print out the item youre trying to buy and take currency 
{
	if (!ItemDataTable || !Player) return;

	if (inventoryManager->IsInventoryFull())
	{
		UE_LOG(LogTemp, Display, TEXT("Inventory Full"));
		return;
	}
	FItemDataInfo* ItemToBuy = ItemDataTable->FindRow<FItemDataInfo>(ItemKey, ""); //looking up the data on the table
	
	if (Player->GetCurrencySystem()->GetPlayerCurrentCurrency() >= ItemToBuy->BuyPrice)
	{
		Player->GetCurrencySystem()->ChangePlayerCurrencey(-ItemToBuy->BuyPrice);
		
		inventoryManager->AddItemToInventory(ItemKey, 1); //needs the item key AKA the row name NOT the itemtobuy
		inventoryManager->RefreshInventory();
	}
}

//selling the item and giving the player money back
void AShopMechanic::SellItem(FName ItemKey, AMyCharacter* Player)
{
	if (!ItemDataTable || !Player) return;

	FItemDataInfo* ItemToBuy = ItemDataTable->FindRow<FItemDataInfo>(ItemKey, ""); //looking up the data on the table
	
	if (inventoryManager->Hasitem(ItemKey, 1))
	{
		Player->GetCurrencySystem()->ChangePlayerCurrencey(ItemToBuy->SellPrice);

		inventoryManager->RemoveItemFromInventory(ItemKey, 1);
		inventoryManager->RefreshInventory();
	}
}

//delegate to buy items - binding the on click
void AShopMechanic::RequestDispatch(FName ItemKey) //for buying items
{
	AMyCharacter* Player = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (Player)
	{
		BuyItem(ItemKey, Player);
	}
}

void AShopMechanic::RequestSell(FName ItemKey)
{
	AMyCharacter* Player = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		SellItem(ItemKey, Player);
	}
}
#pragma endregion 

#pragma region Updating UI
void AShopMechanic::UpdateWidgetUI()
{
	bool bRangeCheck = bPlayerInRange;
	
	if (WidgetDetectionComponent)
	{
		WidgetDetectionComponent->SetVisibility(bRangeCheck);
	}
}

void AShopMechanic::SetShopFilter(EItemType NewFilter)
{
	ActiveFilter = NewFilter;

	UScrollBox* Scrollbox = Cast<UScrollBox>(ShopSystemUIInstance->GetWidgetFromName(TEXT("ShopScrollHolder")));
	if (Scrollbox)
	{
		FillShopItems(Scrollbox);
	}
}

void AShopMechanic::FillShopItems(UScrollBox* ScrollBox)
{
	if (!ScrollBox || !ItemDataTable) return;
	
}

void AShopMechanic::SetupShopSystem()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (!ShopSystemUIInstance && ShopsystemUIClass) //create the widget only if it doesnt exist
		{
			ShopSystemUIInstance = CreateWidget<UUserWidget>(PC, ShopsystemUIClass);
		}
		if (ShopSystemUIInstance) //if you do an "else if" statement it wont work
		{
			ShopSystemUIInstance->AddToViewport();
			bShopOpen = true;
			//
			//Keep in mind this fetch for the scroll box likes to reset the name - causes no Ui to show - go into widget and name the scroll box the name below
			//
			UScrollBox* ScrollBox = Cast<UScrollBox>(ShopSystemUIInstance->GetWidgetFromName(TEXT("ShopScrollHolder"))); //getting the scroll box *Spawn location
			
			if (ScrollBox && ItemDataTable)
			{
				ScrollBox->ClearChildren(); //clear out shop before opening

				for(const FName& RowName : ShopItemNames) //reading through names in the data table
				{
					if (ShopItemSlots)
					{
						UUserWidget* NewSlot = CreateWidget<UUserWidget>(PC, ShopItemSlots); //creating the button ui for each item
						if (FProperty* Prop = NewSlot->GetClass()->FindPropertyByName(TEXT("ItemKey"))) //set the itemkey 
						{
							FNameProperty* NameProp = CastField<FNameProperty>(Prop);
							NameProp->SetPropertyValue_InContainer(NewSlot, RowName); //setting the name and values to each new slot
						}
						FMulticastDelegateProperty* DelegateBuyRequest = FindFProperty<FMulticastDelegateProperty>(NewSlot->GetClass(), TEXT("RequestDispatch")); //getting delegate on each slot
						FMulticastDelegateProperty* DelegateSellRequest = FindFProperty<FMulticastDelegateProperty>(NewSlot->GetClass(), TEXT("RequestSell"));
						if (DelegateBuyRequest) //binding buy event to every button when created 
						{
							FScriptDelegate DelegateBuy;
							DelegateBuy.BindUFunction(this, FName("RequestDispatch"));
							DelegateBuyRequest->AddDelegate(DelegateBuy, NewSlot);
						}
						if (DelegateSellRequest)
						{
							FScriptDelegate DelegateSell;
							DelegateSell.BindUFunction(this, FName("RequestSell"));
							DelegateSellRequest->AddDelegate(DelegateSell, NewSlot);
						}
						
						ScrollBox->AddChild(NewSlot); //adding the button to the scroll box
						NewSlot->SetPadding(-12);
					}
				}
			}
		}
}
#pragma endregion

#pragma region OverlapEvents
void AShopMechanic::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
	{
		bPlayerInRange = true;
		UpdateWidgetUI();
	}
}
void AShopMechanic::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
	{
		bPlayerInRange = false;
		UpdateWidgetUI();
	}
}
#pragma endregion

