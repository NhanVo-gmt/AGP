// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "Characters/EnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "SquadActor.generated.h"

UENUM(BlueprintType)
enum class ESquadState : uint8
{
	Patrol,
	Engage,
	
};

UCLASS()
class AGP_API ASquadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASquadActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* LocationComponent;
	UPROPERTY()
	UPathfindingSubsystem* PathfindingSubsystem;

	UPROPERTY(EditAnywhere)
	int squadId;
	UPROPERTY(VisibleAnywhere)
	int squadSize;
	float squadHealth;

	TArray<FVector> squadPath;
	
	std::vector<AEnemyCharacter*> members;

	void SquadRegroup();

	void SquadRetreat();

	void SquadPatrol();

	void SquadFlank();

	bool OrdersCheck();

	bool PlayerCheck();

	UPROPERTY(EditAnywhere)
	ESquadState CurrentState = ESquadState::Patrol;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
