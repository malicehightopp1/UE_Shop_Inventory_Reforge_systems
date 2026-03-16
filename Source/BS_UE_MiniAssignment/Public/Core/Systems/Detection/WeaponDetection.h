// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Systems/Interaction/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "WeaponDetection.generated.h"

class USphereComponent;
class AMyCharacter;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class BS_UE_MINIASSIGNMENT_API AWeaponDetection : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponDetection();
	
	virtual void InteractPure(AMyCharacter* player) override; //player interaction
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	//Reforge Calling
	UFUNCTION(BlueprintCallable, Category = "UI") void TriggerReforge();	
	UFUNCTION() void UpdateWidgetUI() const;
	UFUNCTION() void ConvertDataTypes();
	UFUNCTION() void SpawnParticle() const;
	
	//Overlap detection - Weapon and Player
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION() void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//UI
	UPROPERTY(EditDefaultsOnly, Category = "UI") UWidgetComponent* ReforgeTrigger;
	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UUserWidget> ReforgeInformationUI; //information for instance to grab from
	UPROPERTY() UUserWidget* ReforgeInformationInstance; //instance for making when interaaction happens
	UPROPERTY() class UButton* ReforgeButton;
	UPROPERTY() AMyCharacter* MyCharacterReference;
	
	//Reforging
	UPROPERTY() AActor* CurrentWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Detection | Reforging") bool bPlayerIsInRange = false;
	UPROPERTY(EditDefaultsOnly, Category = "Detection | Reforging") float PlayerCurrentCurrency = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Detection | Reforging") bool bShopOpen = false;
	UPROPERTY() FText FinalText;
	
	//Components
	UPROPERTY(EditDefaultsOnly, Category = "Detection | Components ") UBoxComponent* BoxComp;
	UPROPERTY(EditDefaultsOnly, Category = "Detection | Components ") USphereComponent* SphereComp;

	//Particles
	UPROPERTY(EditDefaultsOnly, Category = "Detection | Components | Particles") UNiagaraSystem* ReforgeParticle;
	UPROPERTY(EditDefaultsOnly, Category = "Detection | Components | Particles") FVector ParticleSpawnLocation;
	UPROPERTY(EditDefaultsOnly, Category = "Detection | Components | Particles") FRotator ParticleSpawnRotation;

};
