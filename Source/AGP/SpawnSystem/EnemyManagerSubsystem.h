// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UEnemyManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual TStatId GetStatId() const override
	{
		return TStatId();
	}

protected:

	UPROPERTY(EditAnywhere)
	float MinSpawnDistance = 4000.0f;
	UPROPERTY(EditAnywhere)
	float MaxSpawnDistance = 7000.0f;
	UPROPERTY(EditAnywhere)
	float EnemySpawnRate = 10.0f;
	float TimeSinceLastSpawn = 0.0f;
	
	virtual void Tick(float DeltaTime) override;

private:
	
	
	void SpawnEnemy();
};
