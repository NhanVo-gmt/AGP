// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGP/Utilities/MyUtilities.h"
#include "Subsystems/WorldSubsystem.h"
#include "ProceduralGenerationAlgorithm.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UProceduralGenerationAlgorithm : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	
	TSet<FVector2D> SimpleRandomWalk(FVector2D StartPos, int WalkLength);
	
};
