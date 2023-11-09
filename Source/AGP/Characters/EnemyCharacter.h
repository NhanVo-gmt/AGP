// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.generated.h"

// Forward declarations to avoid needing to #include files in the header of this class.
// When these classes are used in the .cpp file, they are #included there.
class UPawnSensingComponent;
class APlayerCharacter;
class UPathfindingSubsystem;

/**
 * An enum to hold the current state of the enemy character.
 */
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Patrol,
	Engage,
	Evade,
	ReceivingOrders,
	Explode
};

/**
 * A class representing the logic for an AI controlled enemy character. 
 */

UCLASS()
class AGP_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	void ReceiveOrders(TArray<FVector> orders);
	

	void ReceiveOrders(EEnemyState state);

	float ReturnHealth();

	bool NeedOrders();

	bool PlayerCheck();

	void SquadBroken();

	FVector playerLocation;

	FVector PlayerLocation();

	void AdjustPathfindingError();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Will move the character along the CurrentPath or do nothing to the character if the path is empty.
	 */
	void MoveAlongPath();

	/**
	 * Logic that controls the enemy character when in the Patrol state.
	 */
	void TickPatrol();
	/**
	 * Logic that controls the enemy character when in the Engage state.
	 */
	void TickEngage();
	/**
	 * Logic that controls the enemy character when in the Evade state.
	 */
	void TickEvade();

	bool CheckIfPlayerInExplodingRange();

	void ReadyToExplode();
	void Explode();
	UFUNCTION(Server, Reliable)
	void ServerExplode();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastExplode();

	//Beginning of "Squad-Based" logic.
	//Logic that will control the enemies in Investigation state.
	void TickAwaitingOrders();
	
	/**
	 * A function bound to the UPawnSensingComponent's OnSeePawn event. This will set the SensedCharacter variable
	 * if the pawn that was sensed was of type APlayerCharacter.
	 * @param SensedActor The pawn that was sensed by the UPawnSensingComponent.
	 */
	UFUNCTION()
	void OnSensedPawn(APawn* SensedActor);
	/**
	 * Will update the SensedCharacter variable based on whether the UPawnSensingComponent has a line of sight to the
	 * Player Character or not. This may cause the SensedCharacter variable to become a nullptr so be careful when using
	 * the SensedCharacter variable.
	 */
	void UpdateSight();

	/**
	 * A pointer to the Pathfinding Subsystem.
	 */
	UPROPERTY()
	UPathfindingSubsystem* PathfindingSubsystem;

	/**
	 * A pointer to the PawnSensingComponent attached to this enemy character.
	 */
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	/**
	 * A pointer to a PlayerCharacter that can be seen by this enemy character. If this is nullptr then the enemy cannot
	 * see any PlayerCharacter.
	 */
	UPROPERTY()
	APlayerCharacter* SensedCharacter = nullptr;

	/**
	 * An array of vectors representing the current path that the agent is traversing along.
	 */
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> CurrentPath;

	/**
	 * The current state of the enemy character. This determines which logic to use when executing the finite state machine
	 * found in the tick function of this enemy character.
	 */
	UPROPERTY(EditAnywhere)
	EEnemyState CurrentState = EEnemyState::Patrol;

	/**
	 * Some arbitrary error value for determining how close is close enough before moving onto the next step in the path.
	 */
	UPROPERTY(EditAnywhere)
	float PathfindingError = 100.0f; // 150 cm from target by default.

	UPROPERTY(EditAnywhere)
	float ReadyToExplodeRadius = 700.0f;
	UPROPERTY(EditAnywhere)
	float ExplodingDelay = 1.0f;
	UPROPERTY(EditAnywhere)
	float ExplodingDamage = 10.0f;
	UPROPERTY(EditAnywhere)
	float ExplodingRadius = 1000.0f;
	

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	/**
	 * NOT USED ANYMORE - Was used for TickEvade and TickEngage before we setup the UPawnSensingComponent.
	 * @return A pointer to one APlayerCharacter actor in the world.
	 */
	APlayerCharacter* FindPlayer() const;

};
