// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUtilities.h"

MyUtilities::MyUtilities()
{
}

MyUtilities::~MyUtilities()
{
}

FVector2D MyUtilities::GetRandomCardinalDirection()
{
	return CardinalDirectionList[FMath::RandRange(0, CardinalDirectionList.Num() - 1)];
}

FVector2D MyUtilities::ElementAt(TSet<FVector2D> Set, int Index)
{
	int CurrentIndex = 0;
	for(auto& Element: Set)
	{
		if (CurrentIndex == Index)
		{
			return Element;	
		}
		CurrentIndex++;
	}

	return FVector2D(-10, -10);
}
