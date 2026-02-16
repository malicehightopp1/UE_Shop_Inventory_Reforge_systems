// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BS_UE_MiniAssignment/BS_UE_MiniAssignmentCharacter.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "MyCharacter.generated.h"

class UCameraComponent;

UCLASS()
class BS_UE_MINIASSIGNMENT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(VisibleAnywhere) UPhysicsHandleComponent* PhysicsHandleComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UFUNCTION() void Look(const FInputActionValue& Value);
	UFUNCTION() void Move(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputMappingContext* MyMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* JumpAction;
	UPROPERTY(VisibleAnywhere, Category = "Player | Components") UCameraComponent* CameraComp;
};
