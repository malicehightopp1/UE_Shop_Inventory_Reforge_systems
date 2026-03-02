// Brandin stanfield


#include "Core/Systems/Shop System/ShopMechanic.h"
#include "Components/SphereComponent.h"

// Sets default values
AShopMechanic::AShopMechanic()
{
	PrimaryActorTick.bCanEverTick = true;

	//Player detection for UI
	//SphereDetectionComponent->CreateDefaultSubobject<USphereComponent>(TEXT("Player Detection"));
	// SphereDetectionComponent->SetupAttachment(RootComponent);
	// SphereDetectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	//
	// SphereDetectionComponent->SetSphereRadius(ShopDefaultDetection); //default sphere radius
}

void AShopMechanic::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShopMechanic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

