// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BS_UE_MiniAssignment/BS_UE_MiniAssignmentCharacter.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PlayerStats/BPC_Currencysystem.h"
#include "MyCharacter.generated.h"

class UCanvasPanel;
class UBPC_Currencysystem;
class UWidgetComponent;
class UCameraComponent;
class UInventoryManager;

UCLASS()
class BS_UE_MINIASSIGNMENT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(VisibleAnywhere) UPhysicsHandleComponent* PhysicsHandleComp; //physics component for items
	UPROPERTY(EditDefaultsOnly, Category = "Player | Components") UUserWidget* PlayerWidget;

	UFUNCTION(BlueprintCallable, Category = "Player | Components")
	FORCEINLINE UBPC_Currencysystem* GetCurrencySystem() const {return CurrencySystem;}
	UPROPERTY(EditDefaultsOnly, Category = "Player | Components") bool bPlayerInShop;
	UPROPERTY(EditDefaultsOnly, Category = "Player") UInventoryManager* InventoryManagerRef;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Components") TSubclassOf<UUserWidget> PlayerInteraction;
	UPROPERTY() UUserWidget* PlayerInteractionWidget;
	
	UPROPERTY() AActor* PlayerInteractionActor;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY() UBPC_Currencysystem* CurrencySystem;
	UFUNCTION() void PerformLookTrace();
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// functions - Locomotion
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION() void Look(const FInputActionValue& Value);
	UFUNCTION() void Move(const FInputActionValue& Value);
	UFUNCTION() void Interact(const FInputActionValue& Value);
	UFUNCTION() void InventoryToggle(const FInputActionValue& Value);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// funtions - item pickup
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION() void Grab();
	UFUNCTION() void Release();
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Inputs
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputMappingContext* MyMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* GrabAction;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* InteractionAction;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* InventoryAction;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Components
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(VisibleAnywhere, Category = "Player | Components") UCameraComponent* CameraComp;
	
};
