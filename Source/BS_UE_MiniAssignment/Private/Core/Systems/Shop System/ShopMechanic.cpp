// Brandin stanfield


#include "Core/Systems/Shop System/ShopMechanic.h"

#include "Components/BoxComponent.h"
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
	
	ShopsystemUI = CreateDefaultSubobject<UUserWidget>("ShopsystemUI");
}
void AShopMechanic::InteractPure(AMyCharacter* player) //interaction
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	if (bPlayerInRange && ShopsystemUI && bShopOpen == false)
	{
		{
			ShopsystemUI->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Shop turned On"));
			bShopOpen = true;
			PC->SetIgnoreMoveInput(true);
			PC->SetIgnoreLookInput(true);
			if (PC)
			{
				FInputModeGameAndUI mode;
				mode.SetWidgetToFocus(ShopsystemUI->TakeWidget());
				mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->SetInputMode(mode);
				
				PC->bShowMouseCursor = true;
			}
		}
	}
	else if (bShopOpen == true)
	{
		ShopsystemUI->RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("Shop Turned Off"));
		bShopOpen = false;
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
		if (PC)
		{
			FInputModeGameAndUI mode;
			PC->SetInputMode(mode);
			PC->bShowMouseCursor = false;
		}
	}
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

