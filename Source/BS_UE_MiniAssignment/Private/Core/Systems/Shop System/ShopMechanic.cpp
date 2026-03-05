// Brandin stanfield


#include "Core/Systems/Shop System/ShopMechanic.h"

#include "Components/BoxComponent.h"
#include "Components/ScrollBox.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Systems/Characters/MyCharacter.h"

// Sets default values
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
}

void AShopMechanic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AShopMechanic::InteractPure(AMyCharacter* player) //interaction
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	if (bPlayerInRange && bShopOpen == false) // turn on the UI
	{
		if (!ShopSystemUIInstance && ShopsystemUIClass) //create the widget only if it doesnt exist
		{
			ShopSystemUIInstance = CreateWidget<UUserWidget>(PC, ShopsystemUIClass);
		}
		if (ShopSystemUIInstance)
		{
			ShopSystemUIInstance->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Shop turned On"));
			bShopOpen = true;
			
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
						
						ScrollBox->AddChild(NewSlot); //adding the button to the scroll box
						NewSlot->SetPadding(50);
					}
				}
			
			}
		}
		
		if (PC)
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
		UE_LOG(LogTemp, Warning, TEXT("Shop Turned Off"));
		bShopOpen = false;
		if (PC)
		{
			PC->SetIgnoreMoveInput(false);
			PC->SetIgnoreLookInput(false);
			PC->bShowMouseCursor = false;
		}
	}
}

void AShopMechanic::BuyItem()
{
	// auto* Player = Cast<AMyCharacter>(Player);
	// if (Player)
	// {
	// 	
	// }
}

void AShopMechanic::SellItem()
{
	
}


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
void AShopMechanic::UpdateWidgetUI()
{
	bool bRangeCheck = bPlayerInRange;
	
	if (WidgetDetectionComponent)
	{
		WidgetDetectionComponent->SetVisibility(bRangeCheck);
	}
}

