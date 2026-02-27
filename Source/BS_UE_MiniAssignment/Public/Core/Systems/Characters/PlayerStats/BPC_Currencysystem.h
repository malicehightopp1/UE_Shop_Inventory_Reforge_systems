// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BPC_Currencysystem.generated.h"

/* Dynamic allows it to be bound in blueprint
 * Multicast allows multiple functions to listen when the event takes place
 * OneParam - it will pass only one function
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOncurrencyChange,float,CurrencyChange); //declared before UCLASS
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BS_UE_MINIASSIGNMENT_API UBPC_Currencysystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBPC_Currencysystem();
	UFUNCTION(BlueprintCallable, Category = "PlayerStats") void ChangePlayerCurrencey(float CurrencyTochange);
	UFUNCTION(BlueprintCallable, Category = "PlayerStats") void UpdateCurrencyUI();
	
	UPROPERTY(EditDefaultsOnly,BlueprintAssignable, Category = "PlayerStats") FOncurrencyChange OnCurrencyChange;

	UFUNCTION(BlueprintCallable, Category = "PlayerStats") float GetPlayerCurrentCurrency(){return PlayerCurrentCurrency;}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerStats") float PlayerCurrentCurrency = 0;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerStats") float PlayerMaxCurrency = 1000;
};
