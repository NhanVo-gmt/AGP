// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "SquadComponent.h"
#include "Characters/EnemyCharacter.h"
#include "GameFramework/Actor.h"
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

	std::vector<USquadComponent> activeSquads;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
