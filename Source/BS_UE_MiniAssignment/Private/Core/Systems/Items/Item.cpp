// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Items/Item.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	RootComponent = ItemMesh;

	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetEnableGravity(true);
	ItemMesh->SetCollisionProfileName(TEXT("Physics Actor"));

	bIsHeld = false;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

