// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "Core/Systems/Interaction/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "ShopMechanic.generated.h"

class UInventoryManager;
class UBoxComponent;
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
	UFUNCTION(BlueprintCallable, Category = "Shop System | Item Systems") void BuyItem(FName ItemKey, AMyCharacter* Player);
	UFUNCTION(BlueprintCallable, Category = "Shop System | Item Systems") void SellItem();
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional),Category = "Shop System | Data") class UScrollBox* ShopScrollBox;

	UPROPERTY(EditAnywhere, Category = "Shop System | Data") TArray<FName> ShopItemNames;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//UI functions
	UFUNCTION() void RequestDispatch(FName ItemKey);
	UFUNCTION() void UpdateWidgetUI();

	//Interaction Functions
	UFUNCTION() void SetupShopSystem();
	//Overlap Events functions
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Overlap Variables 
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | DefaultValues") bool bPlayerInRange = false;
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | DefaultValues") bool bShopOpen = false;
	
	//Items 
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | Data") UDataTable* ItemDataTable;
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | Data") TSubclassOf<UUserWidget> ShopItemSlots;
	
	//Components
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | Components") USphereComponent* SphereDetectionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | UI") UWidgetComponent* WidgetDetectionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | Components") UBoxComponent* BoxDetectionComponent;

	//Defaults
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | DefaultValues") float ShopDefaultDetection = 10.0f;

	//UserWidgets
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | UI") TSubclassOf<UUserWidget> ShopsystemUIClass; //reference to the shop widget
	UPROPERTY() UUserWidget* ShopSystemUIInstance; //the actual instance, AKA what we spawn
	
	UPROPERTY() UInventoryManager* inventoryManager;
};
