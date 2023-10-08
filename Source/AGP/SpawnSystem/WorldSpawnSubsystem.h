// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldSpawnSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UWorldSpawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
private:
	TArray<TArray<int>> Grids;
	TArray<FVector2D> Points;
	TArray<FVector2D> SpawnPoints;
	bool IsValid(const FVector2D& Candidate, const int& Width, const int& Height, const int& CellSize, const float& Radius);
	
public:
	TArray<FVector2D> GeneratePoints(const int& Width, const int& Height, const int& CellSize, const int& NumberBeforeRejection);
};
