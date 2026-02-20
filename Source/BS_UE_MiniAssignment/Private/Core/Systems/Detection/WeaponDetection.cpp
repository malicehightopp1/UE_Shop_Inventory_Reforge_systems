// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Detection/WeaponDetection.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Systems/Characters/MyCharacter.h"
#include "Core/Systems/Items/Weapon.h"
#include "Core/Systems/Reforging/ReforgeData.h"

#pragma region Setup Components
// Sets default values
AWeaponDetection::AWeaponDetection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	//SphereComp->CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//SphereComp->SetupAttachment(RootComponent); //dont attach to Box Comp it crashes
	
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
	//SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponDetection::OnSphereOverlapBegin);
	//SphereComp->OnComponentEndOverlap.AddDynamic(this, &AWeaponDetection::OnSphereOverlapEnd);
}
void AWeaponDetection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion
#pragma region weapon Detection
void AWeaponDetection::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* ItemDetected = Cast<AWeapon>(OtherActor))
	{
		CurrentWeapon = ItemDetected;
	}
}
void AWeaponDetection::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CurrentWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("place holder"));
	}
}
#pragma endregion
// void AWeaponDetection::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if (auto* player = Cast<AMyCharacter>(OtherActor))
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Player detected"));
// 		ReforgeTrigger->SetVisibility(true);
// 	}
// }

// void AWeaponDetection::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	if (OtherActor == nullptr)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Player left"));
// 		ReforgeTrigger->SetVisibility(false);
// 	}
// }

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
		UE_LOG(LogTemp, Error, TEXT("No Reforgable item on anvil"));
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

	UE_LOG(LogTemp, Warning, TEXT("Reforging Item..."));
	Weapon->WeaponStats = Weapon->ReforgeData->GetRandomReforge();
	UE_LOG(LogTemp, Warning, TEXT("Reforged Item!"));
}
#pragma endregion