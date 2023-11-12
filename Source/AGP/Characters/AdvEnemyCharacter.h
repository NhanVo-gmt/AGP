// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AdvEnemyCharacter.generated.h"

class UPawnSensingComponent;
class APlayerCharacter;
class UPathfindingSubsystem;

// UENUM(BlueprintType)
// enum class EEnemyState : uint8
// {
// 	Patrol,
// 	Engage,
// 	Evade
// };

class Task
{
	
};

UCLASS()
class AGP_API AAdvEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAdvEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Stores the tasks to be executed
	std::vector<Task> TaskQueue;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
