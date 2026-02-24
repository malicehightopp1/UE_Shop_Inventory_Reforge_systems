// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BPC_Currencysystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BS_UE_MINIASSIGNMENT_API UBPC_Currencysystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBPC_Currencysystem();

	UFUNCTION(Blueprintable, Category = "PlayerStats") void ChangePlayerCurrencey(float CurrencyTochange);
	//UFUNCTION(blueprintCallable, Category = "PlayerStats") void UpdatePlayerCurrency(const FString& CurrencyTochange);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerStats") float PlayerCurrentCurrency = 0;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerStats") float PlayerMaxCurrency = 999;
};
