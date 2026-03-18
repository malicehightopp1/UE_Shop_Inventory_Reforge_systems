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
	
	if (bPlayerInRange && bShopOpen == false) // turn on the UI
	{
		SetupShopSystem();
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
	
	FItemDataInfo* ItemToBuy = ItemDataTable->FindRow<FItemDataInfo>(ItemKey, ""); //looking up the data on the table
	
	if (Player->GetCurrencySystem()->GetPlayerCurrentCurrency() >= ItemToBuy->BuyPrice)
	{
		Player->GetCurrencySystem()->ChangePlayerCurrencey(-ItemToBuy->BuyPrice);
		
		inventoryManager->AddItemToInventory(ItemKey, 1); //needs the item key AKA the row name NOT the itemtobuy
		inventoryManager->RefreshInventory();
	}
}

//selling the item and giving the player money back
void AShopMechanic::SellItem()
{
	
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
			UE_LOG(LogTemp, Warning, TEXT("Shop turned On"));
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
						UE_LOG(LogTemp, Warning, TEXT("Shop added items"));
						if (FProperty* Prop = NewSlot->GetClass()->FindPropertyByName(TEXT("ItemKey"))) //set the itemkey 
						{
							FNameProperty* NameProp = CastField<FNameProperty>(Prop);
							NameProp->SetPropertyValue_InContainer(NewSlot, RowName); //setting the name and values to each new slot
						}
						FMulticastDelegateProperty* DelegateProp = FindFProperty<FMulticastDelegateProperty>(NewSlot->GetClass(), TEXT("RequestDispatch")); //getting delegate on each slot
						
						if (DelegateProp) //binding buy event to every button when created 
						{
							UE_LOG(LogTemp, Warning, TEXT("Shop request dispatch property"));
							FScriptDelegate Delegate;
							Delegate.BindUFunction(this, FName("RequestDispatch"));
							DelegateProp->AddDelegate(Delegate, NewSlot);
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
		UE_LOG(LogTemp, Warning, TEXT("Player Entered Shop Area"));
		bPlayerInRange = true;
		UpdateWidgetUI();
	}
}
void AShopMechanic::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Left Shop Area"));
		bPlayerInRange = false;
		UpdateWidgetUI();
	}
}
#pragma endregion

