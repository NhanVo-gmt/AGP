// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AGP_API MyUtilities
{
public:
	MyUtilities();
	~MyUtilities();

	static inline TArray<FVector2D> CardinalDirectionList = {
		FVector2D(0, 1), //up
		FVector2D(1, 0), //right
		FVector2D(0, -1), //down
		FVector2D(-1, 0), //left
	};

	static FVector2D GetRandomCardinalDirection();
	static FVector2D ElementAt(TSet<FVector2D> Set, int Index);
};
