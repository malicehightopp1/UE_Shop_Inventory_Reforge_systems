// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShopMechanic.generated.h"

class USphereComponent;

UCLASS()
class AShopMechanic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShopMechanic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | Components") USphereComponent* SphereDetectionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | DefaultValues") float ShopDefaultDetection = 10.0f;

};
