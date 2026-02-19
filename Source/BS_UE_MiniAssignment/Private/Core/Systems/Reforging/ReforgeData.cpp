// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Reforging/ReforgeData.h"

FReforgeDataChanges UReforgeData::GetRandomReforge()
{
	if (ReforgeData.Num() > 0) //making sure th array isnt empty
	{
		int32 RandomIndex = FMath::RandRange(0, ReforgeData.Num() - 1); //basic random range that looks through the information  
		return ReforgeData[RandomIndex]; //returning whatever the Random Range gets above
	}
	//returning a normal value if no result is found
	FReforgeDataChanges defaultResult;
	defaultResult.Prefixes = TEXT("Normal");
	defaultResult.StatMultiplier = 1.0f;
	return defaultResult;

	/*
	if (ReforgeMap.Contains(Type)) //checking if the enum type exists aka the key of the values for that set weapon
	{
		if (const FReforgeItemPool* Pool = ReforgeMap.Find(Type)) //finds the pointer to the pool if doesnt exist gives nullptr
		{
			if (Pool->ReforgeData.Num() > 0) //making sure th array isnt empty
			{
				int32 RandomIndex = FMath::RandRange(0, Pool->ReforgeData.Num() - 1); //basic random range that looks through the information  
				return Pool->ReforgeData[RandomIndex]; //returning whatever the Random Range gets above
			}
		}

	}
	*/
}