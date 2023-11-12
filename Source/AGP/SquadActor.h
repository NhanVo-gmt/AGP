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
	Broken
	
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
	UPROPERTY(Replicated)
	float maxHealth;
	UPROPERTY(Replicated)
	float squadHealth;
	UPROPERTY(Replicated)
	TArray<FVector> squadPath;
	FVector playerLocation;
	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<AEnemyCharacter*> members;

	UPROPERTY(EditDefaultsOnly, Replicated)
	float pathModifier = 0.75;
	UPROPERTY(EditDefaultsOnly, Replicated)
	int pathSizeCheck = 8;
	
	void SquadRegroup();

	void SquadRetreat();

	void SquadPatrol();

	void SquadScatter();

	bool OrdersCheck();

	bool PlayerCheck();

	void ClearOrders();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	UPROPERTY(EditAnywhere, Replicated)
	ESquadState CurrentState = ESquadState::Patrol;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
