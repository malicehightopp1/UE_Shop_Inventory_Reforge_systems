// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Detection/WeaponDetection.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Systems/Characters/MyCharacter.h"
#include "Core/Systems/Items/Weapon.h"
#include "Core/Systems/Reforging/ReforgeData.h"

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
	if (CurrentWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Reforging: %s"), *CurrentWeapon->GetName());
		TriggerReforge();
	}
	else
	{
		FString debugmsgFail = FString::Printf(TEXT("Item is Not a reforge Item!!"));
		
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Cyan, debugmsgFail);
	}
}

void AWeaponDetection::TriggerReforge()
{
	auto* Weapon = Cast<AWeapon>(CurrentWeapon);

	if (!Weapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Not a reforgable Item"));
		return;
	}
	Weapon->WeaponStats = Weapon->ReforgeData->GetRandomReforge();
	
	if (GEngine)
	{
		FString debugmsg = FString::Printf(TEXT("%s %s --- StatMultiply: %fx"), *Weapon->WeaponStats.Prefixes, *Weapon->WeaponName, Weapon->WeaponStats.StatMultiplier);
		
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Cyan, debugmsg);
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
void AWeaponDetection::UpdateWidgetUI()
{
	bool bShouldShowUI = (bPlayerIsInRange && CurrentWeapon != nullptr);
	
	if (ReforgeTrigger)
	{
		ReforgeTrigger->SetVisibility(bShouldShowUI);
	}
}
#pragma endregion