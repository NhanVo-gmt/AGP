// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PickupManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UPickupManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	};

protected:
	TArray<FVector> PossibleSpawnLocations;

	float PickupSpawnRate = 0.5f;
	float TimeSinceLastSpawn = 0.0f;

private:
	void PopulateSpawnLocations();
	void SpawnWeaponPickup();

	virtual void Tick(float DeltaTime) override;
};
