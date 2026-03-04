// Brandin stanfield


#include "Core/Systems/Shop System/ShopMechanic.h"
#include "Components/SphereComponent.h"

// Sets default values
AShopMechanic::AShopMechanic()
{
	PrimaryActorTick.bCanEverTick = true;

	//Player detection for UI
	SphereDetectionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Player Detection"));
	SphereDetectionComponent->SetupAttachment(RootComponent);
	SphereDetectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	
	ShopDefaultDetection = 300.0f;
	SphereDetectionComponent->SetSphereRadius(ShopDefaultDetection); //default sphere radius
}

void AShopMechanic::InteractPure(AMyCharacter* player) //interaction
{
	
}

void AShopMechanic::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShopMechanic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShopMechanic::UpdateWidgetUI()
{
	
}

