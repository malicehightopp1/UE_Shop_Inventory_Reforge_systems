// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Detection/WeaponDetection.h"
#include "Components/BoxComponent.h"
#include "Core/Systems/Items/Weapon.h"
#include "Core/Systems/Reforging/ReforgeData.h"

// Sets default values
AWeaponDetection::AWeaponDetection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
}
void AWeaponDetection::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* ItemDetected = Cast<AWeapon>(OtherActor);

	if (!ItemDetected)
	{
		UE_LOG(LogTemp, Error, TEXT("Not a reforgable Item"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Reforging Item..."));
	ItemDetected->WeaponStats = ItemDetected->ReforgeData->GetRandomReforge();
	UE_LOG(LogTemp, Warning, TEXT("Reforged Item!"));
}
// Called when the game starts or when spawned
void AWeaponDetection::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponDetection::OnOverlapBegin);
}

// Called every frame
void AWeaponDetection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

