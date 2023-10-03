// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManagerSubsystem.h"

#include "AGPGameInstance.h"
#include "Pathfinding/PathfindingSubsystem.h"


void UPickupManagerSubsystem::PopulateSpawnLocations()
{
	PossibleSpawnLocations.Empty();

	PossibleSpawnLocations = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->GetWaypointPositions();
}

void UPickupManagerSubsystem::SpawnWeaponPickup()
{
	if (PossibleSpawnLocations.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to spawn weapon pickup"));
		return;
	}

	if (const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>())
	{
		FVector SpawnPosition = PossibleSpawnLocations[FMath::RandRange(0, PossibleSpawnLocations.Num() - 1)];
		SpawnPosition.Z += 50.0f;
		AWeaponPickup* Pickup = GetWorld()->SpawnActor<AWeaponPickup>(GameInstance->GetWeaponPickupClass(), SpawnPosition, FRotator::ZeroRotator);

		UE_LOG(LogTemp, Display, TEXT("Weapon pickup spawned")); 
	}
}

void UPickupManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PossibleSpawnLocations.IsEmpty())
	{
		PopulateSpawnLocations();
	}

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= PickupSpawnRate)
	{
		TimeSinceLastSpawn = 0.0f;
		SpawnWeaponPickup();
	}
}



