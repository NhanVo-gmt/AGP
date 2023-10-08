// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "SquadActor.h"
#include "Characters/EnemyCharacter.h"
#include "PickupManagerSubsystem.h"
#include "AGPGameInstance.h"
#include "Pathfinding/PathfindingSubsystem.h"
#include "GameFramework/Actor.h"
#include "Pathfinding/NavigationNode.h"
#include "Overlord.generated.h"



UCLASS()
class AGP_API AOverlord : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOverlord();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	int allowedSquads = 1;
	std::vector<ASquadActor*> activeSquads;
	FVector spawnLocation;

	//Tracking the player for future tactics.
	UPROPERTY(VisibleAnywhere)
	FVector playerLocation;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;
	
	void FindSpawnLocation();
	void SpawnEnemySquad();
	


	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
