// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Characters/MyCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Core/Systems/Interaction/InteractionInterface.h"
#include "Core/Systems/Inventory/InventoryManager.h"
#include "Core/Systems/Items/Item.h"
#include "Kismet/KismetSystemLibrary.h"

class UTextBlock;
// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
	CameraComp->SetRelativeLocation(FVector(0, 0, 75));
	CameraComp->bUsePawnControlRotation = true;
	
	PhysicsHandleComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	
	CurrencySystem = CreateDefaultSubobject<UBPC_Currencysystem>("currencySystem");
	PlayerWidget = CreateDefaultSubobject<UUserWidget>("PlayerWidget");
}
// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerWidget) //adding widget to viewport
	{
		PlayerWidget->AddToViewport();
		UE_LOG(LogTemp, Display, TEXT("Widget added to viewport"));
		CurrencySystem->UpdateCurrencyUI(); //update currency UI right after creating the UI
		CurrencySystem->ChangePlayerCurrencey(10);
	}
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) //setting player controller AKA Mapping context on start
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MyMappingContext, 0);
		}
	}
	InventoryManagerRef = FindComponentByClass<UInventoryManager>(); //grabbing inventory manager
	
	if (PlayerInteraction) //setting up interaction widget on start
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		
		if (PC)
		{
			PlayerInteractionWidget = CreateWidget<UUserWidget>(PC, PlayerInteraction);
			if (PlayerInteractionWidget)
			{
				PlayerInteractionWidget->AddToViewport();
				PlayerInteractionWidget->SetVisibility(ESlateVisibility::Hidden);
				UE_LOG(LogTemp, Warning, TEXT("Interaction UI Connected"));
			}
		}
	}
}
// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PhysicsHandleComp->GetGrabbedComponent())
	{
		FVector TargetLocation = CameraComp->GetComponentLocation() + (CameraComp->GetForwardVector() * 200.f);
		PhysicsHandleComp->SetTargetLocation(TargetLocation);
	}
	
	PerformLookTrace();
}
#pragma region Input Setup
// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &AMyCharacter::Grab);
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Completed, this, &AMyCharacter::Release);
		
		//Interaction
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &AMyCharacter::Interact);

		//Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AMyCharacter::InventoryToggle);
	}
}
#pragma endregion
#pragma region Interaction
void AMyCharacter::Interact(const FInputActionValue& Value) //interaction 
{
	FHitResult* Hit = new FHitResult();
	FVector start = CameraComp->GetComponentLocation();
	FVector end = start + (CameraComp->GetForwardVector() *300);
	
	UKismetSystemLibrary::SphereTraceSingle(this, start, end, 5.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), 
		false, TArray<AActor*>(), EDrawDebugTrace::None, *Hit, true);
	
	if (Hit->GetActor() != nullptr)
	{
		if (Hit->GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass())) //if the actor that was hit has a interface
		{
			Cast<IInteractInterface>(Hit->GetActor())->InteractPure(this);
		}
	}
}

void AMyCharacter::InventoryToggle(const FInputActionValue& Value)
{
	if (!InventoryManagerRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Inventory Ref"))
		return;
	}
	if (!bPlayerInShop)
	{
		InventoryManagerRef->Inventory();
	}
}

/*
 * for grabbing objects for physics moving
 */
void AMyCharacter::Grab()
{
	FHitResult Hit;
	FVector start = CameraComp->GetComponentLocation();
	FVector end = start + (CameraComp->GetForwardVector() * 500);

	ECollisionChannel GrabChannel = ECollisionChannel::ECC_Visibility;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, start,end, GrabChannel, CollisionParams))
	{
		AItem* HitItem = Cast<AItem>(Hit.GetActor());
		
		if (HitItem != nullptr) //need a check for the if statement
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *HitItem->GetName());
			UPrimitiveComponent* comp = Hit.GetComponent();
			PhysicsHandleComp->GrabComponentAtLocation(
			comp,
			NAME_None,
			Hit.ImpactPoint
			);
			HitItem->bIsHeld = true;
		}
	}
}

void AMyCharacter::Release()
{
	if (PhysicsHandleComp->GetGrabbedComponent())
	{
		PhysicsHandleComp->ReleaseComponent();
	}
}
#pragma endregion 
#pragma region Locomotion
void AMyCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMyCharacter::PerformLookTrace()
{
	FHitResult Hit;
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + (CameraComp->GetForwardVector() * 300.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, Start, End, ECC_Visibility, Params);

	if (bHit && Hit.GetActor() && 
		Hit.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		PlayerInteractionActor = Hit.GetActor();

		// Get item name from interface
		FText ItemText = IInteractInterface::Execute_GetInteractText(PlayerInteractionActor);

		// Show and update the widget
		if (PlayerInteractionWidget)
		{
			PlayerInteractionWidget->SetVisibility(ESlateVisibility::Visible);

			// Find the text block and set it
			UTextBlock* Label = Cast<UTextBlock>(
				PlayerInteractionWidget->GetWidgetFromName(TEXT("InteractionText")));
			if (Label)
			{
				Label->SetText(ItemText);
			}
		}
	}
	else
	{
		// Nothing in range, hide widget
		PlayerInteractionActor = nullptr;
		if (PlayerInteractionWidget)
		{
			PlayerInteractionWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
#pragma endregion