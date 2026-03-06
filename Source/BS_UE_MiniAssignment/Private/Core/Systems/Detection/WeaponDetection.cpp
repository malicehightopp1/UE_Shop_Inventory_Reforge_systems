// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Detection/WeaponDetection.h"
#include "Components/BoxComponent.h"
#include "Components/Button.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Core/Systems/Characters/MyCharacter.h"
#include "Core/Systems/Items/Weapon.h"
// Sets default values

AWeaponDetection::AWeaponDetection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent")); //not a -> its a =
	SphereComp->SetupAttachment(BoxComp);
	
	ReforgeTrigger = CreateDefaultSubobject<UWidgetComponent>(TEXT("ReforgeTrigger"));
	ReforgeTrigger->SetupAttachment(BoxComp);
	ReforgeTrigger->SetWidgetSpace(EWidgetSpace::Screen); //setting the widget to be screen
	ReforgeTrigger->SetVisibility(false); //turn off ui as start
}

void AWeaponDetection::BeginPlay()
{
	Super::BeginPlay();
	//Weapon detection for reforge
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponDetection::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AWeaponDetection::OnOverlapEnd);
	
	//player detection and UI turn on
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponDetection::OnSphereOverlapBegin);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AWeaponDetection::OnSphereOverlapEnd);
}
void AWeaponDetection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
#pragma region weapon Detection
void AWeaponDetection::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* ItemDetected = Cast<AWeapon>(OtherActor))
	{
		CurrentWeapon = ItemDetected;
		UpdateWidgetUI();
		UE_LOG(LogTemp, Error, TEXT("weapon on the Anvil"));
	}
}
void AWeaponDetection::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CurrentWeapon)
	{
		CurrentWeapon = nullptr;
		UpdateWidgetUI();
		UE_LOG(LogTemp, Error, TEXT("No weapon on the Anvil"));
	}
}
#pragma endregion
#pragma region Reforge Interaction
void AWeaponDetection::InteractPure(AMyCharacter* player)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	auto* Weapon = Cast<AWeapon>(CurrentWeapon);

	if (bPlayerIsInRange && bShopOpen == false && CurrentWeapon != nullptr)
	{
		if (!ReforgeInformationInstance && ReforgeInformationUI)
		{
			ReforgeInformationInstance = CreateWidget<UUserWidget>(PC, ReforgeInformationUI); //creating widget for reforge
		}
		if (ReforgeInformationInstance)
		{
			ReforgeInformationInstance->AddToViewport();
			bShopOpen = true;
			//ConvertDataTypes();
			
			ReforgeButton = Cast<UButton>(ReforgeInformationInstance->GetWidgetFromName(TEXT("ReforgeButton")));
			UTextBlock* TextBlock = Cast<UTextBlock>(ReforgeInformationInstance->GetWidgetFromName(TEXT("ReforgeInformation")));
			
			if (ReforgeButton && !ReforgeButton->OnClicked.IsBound()) //binding button to reforge button
			{
				UE_LOG(LogTemp, Error, TEXT("ReforgeButton Clicked"));
				ReforgeButton->OnClicked.AddDynamic(this, &AWeaponDetection::TriggerReforge);
			}
			if (TextBlock) //setting text and reforge information
			{
				ConvertDataTypes();
				TextBlock->SetText(FinalText); //setting text to equal to Reforge information
			}
		}
		if (PC)
		{
			PC->SetShowMouseCursor(true);
			PC->SetIgnoreMoveInput(true);
			PC->SetIgnoreLookInput(true);
		}
	}
	else if (bPlayerIsInRange && bShopOpen == true)
	{
		if (ReforgeInformationInstance)
		{
			ReforgeInformationInstance->RemoveFromParent();
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
void AWeaponDetection::TriggerReforge() //this can probably be reworked lots to process on one button call
{
	auto* Weapon = Cast<AWeapon>(CurrentWeapon);
	//AMyCharacter* Player = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController());

	// PlayerCurrentCurrency = Player->GetCurrencySystem()->GetPlayerCurrentCurrency();//player has access to the currency system so cast to player to get currency system
	// if (CurrentWeapon && PlayerCurrentCurrency > 99)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Reforging: %s"), *CurrentWeapon->GetName());
	// 	TriggerReforge();
	// 	Player->GetCurrencySystem()->ChangePlayerCurrencey(-100); //might move this to the Trigger Reforge function so all reforge actions run at once
	// }
	// else if (PlayerCurrentCurrency <= 0)
	// {
	// 	FString debugmsgFail = FString::Printf(TEXT("Not Enough Money!!"));
	// 	GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, debugmsgFail);
	// }
	Weapon->WeaponStats = Weapon->ReforgeData->GetRandomReforge(); //reforging the item
	UTextBlock* TextBlock = Cast<UTextBlock>(ReforgeInformationInstance->GetWidgetFromName(TEXT("ReforgeInformation")));

	if (TextBlock) //setting text and reforge information
	{
		ConvertDataTypes();
		TextBlock->SetText(FinalText); //setting text to equal to Reforge information
	}
}
#pragma endregion
#pragma region Reforge UI
void AWeaponDetection::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AMyCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Player detected"));
		bPlayerIsInRange = true;
		UpdateWidgetUI();
	}
}
void AWeaponDetection::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(AMyCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Player left"));
		bPlayerIsInRange = false;
		UpdateWidgetUI();
	}
}
void AWeaponDetection::UpdateWidgetUI() const
{
	bool bShouldShowUI = (bPlayerIsInRange && CurrentWeapon != nullptr);
	
	if (ReforgeTrigger)
	{
		ReforgeTrigger->SetVisibility(bShouldShowUI);
	}
}

void AWeaponDetection::ConvertDataTypes()
{
	auto* Weapon = Cast<AWeapon>(CurrentWeapon);
	FText WeaponTemplate = NSLOCTEXT("Combat", "WeaponDisplay", "{Prefix:s} {Name} (x{Multiplier:s})"); //setting the format that i want the info to be in
				
	FText finalText = FText::Format //converting to string
		(WeaponTemplate,
		FText::FromString(*Weapon->WeaponStats.Prefixes),
		FText::FromString(*Weapon->WeaponName),
		FText::AsNumber(Weapon->WeaponStats.StatMultiplier)
		);
	FinalText = finalText;
}
#pragma endregion
