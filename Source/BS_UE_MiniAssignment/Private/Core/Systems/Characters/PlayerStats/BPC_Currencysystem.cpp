// Brandin stanfield


#include "Core/Systems/Characters/PlayerStats/BPC_Currencysystem.h"

#include "Blueprint/UserWidget.h"
#include "Core/Systems/Characters/MyCharacter.h"
#include "Kismet/GameplayStatics.h"

UBPC_Currencysystem::UBPC_Currencysystem()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	PlayerCurrentCurrency = 0;
	PlayerMaxCurrency = 999;
}

void UBPC_Currencysystem::ChangePlayerCurrencey(float CurrencyTochange)
{
	PlayerCurrentCurrency += CurrencyTochange;
	PlayerCurrentCurrency = FMath::Clamp(PlayerCurrentCurrency, 0, PlayerMaxCurrency);
}

// void UBPC_Currencysystem::UpdatePlayerCurrency()
// {
// 	
// }
// Called when the game starts
void UBPC_Currencysystem::BeginPlay()
{
	Super::BeginPlay();
	AMyCharacter* player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player)
	{
		//player->PlayerWidget->
	}
	
}
// Called every frame
void UBPC_Currencysystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

