// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "Core/Systems/Interaction/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "ShopMechanic.generated.h"

class UWidgetComponent;
class USphereComponent;

UCLASS()
class AShopMechanic : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShopMechanic();
	virtual void InteractPure(AMyCharacter* player) override; //player interaction
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | DefaultValues") float ShopDefaultDetection = 10.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION() void UpdateWidgetUI();
	
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | Components") USphereComponent* SphereDetectionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | UI") UWidgetComponent* WidgetDetectionComponent;

};
