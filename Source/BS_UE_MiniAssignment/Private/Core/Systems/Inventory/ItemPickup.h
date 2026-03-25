// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "Core/Systems/Interaction/InteractionInterface.h"
#include "Core/Systems/Items/ItemData.h"
#include "GameFramework/Actor.h"
#include "ItemPickup.generated.h"

struct FItemDataInfo;

UCLASS()
class AItemPickup : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPickup") FDataTableRowHandle ItemRowHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPickup | Components") UStaticMeshComponent* ItemMeshComp;
	
	FItemDataInfo ItemInfo; //for caching the data
	
	virtual void InteractPure(AMyCharacter* player) override;
	virtual FText GetInteractText_Implementation() override;

private:
	
	void LoaditemData();
};
