// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGenerationAlgorithm.h"

void UProceduralGenerationAlgorithm::OnWorldBeginPlay(UWorld& InWorld)
{
	
}

TSet<FVector2D> UProceduralGenerationAlgorithm::SimpleRandomWalk(FVector2D StartPos, int WalkLength)
{
	TSet<FVector2D> Path;
	Path.Add(StartPos);
	FVector2D PrevPos = StartPos;

	for (int i = 0; i < WalkLength; i++)
	{
		FVector2D NewPos = PrevPos + MyUtilities::GetRandomCardinalDirection();
		Path.Add(NewPos);
		PrevPos = NewPos;
	}
	return Path;
}
