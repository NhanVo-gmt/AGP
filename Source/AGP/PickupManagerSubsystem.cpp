// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManagerSubsystem.h"

#include "Pickups/WeaponPickup.h"
#include "AGP/AGPGameInstance.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"

void UPickupManagerSubsystem::OnWeaponPickup(const FVector& PickupLocation)
{
	if (SpawnManager.Contains(PickupLocation))
	{
		AWeaponPickup* SpawnedWeapon = SpawnManager[PickupLocation];
		SpawnManager.Remove(PickupLocation);
		SpawnedWeapon->K2_DestroyActor();
	}
}

void UPickupManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We don't want this pickup manager to do any spawning if it isn't
	// on the server.
	// A value < NM_Client is any type of server. So if it is >=
	// to NM_Client or == NM_Client then we know it is the client
	// and we don't want to spawn.
	if (GetWorld()->GetNetMode() >= NM_Client)
	{
		return;
	}

	if (PossibleSpawnLocations.IsEmpty())
	{
		PopulateSpawnLocations();
	}

	if (SpawnManager.IsEmpty())
	{
		PopulateSpawnManager();
	}

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= PickupSpawnRate)
	{
		SpawnWeaponPickup();
		TimeSinceLastSpawn = 0.0f;
	}
}

void UPickupManagerSubsystem::SpawnWeaponPickup()
{
	if (PossibleSpawnLocations.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to spawn weapon pickup."))
		return;
	}
	
	if (const UAGPGameInstance* GameInstance =
		GetWorld()->GetGameInstance<UAGPGameInstance>())
	{
		FVector SpawnPosition =
			PossibleSpawnLocations[FMath::RandRange(0, PossibleSpawnLocations.Num()-1)];
		SpawnPosition.Z += 50.0f;
		
		if (!SpawnManager.Contains(SpawnPosition))
		{
			AWeaponPickup* WeaponPickup = GetWorld()->SpawnActor<AWeaponPickup>(
				GameInstance->GetWeaponPickupClass(), SpawnPosition, FRotator::ZeroRotator);
			SpawnManager.Add(SpawnPosition, WeaponPickup);

			UE_LOG(LogTemp, Display, TEXT("New Weapon Pickup Spawned"));
		}
		else
		{
			AWeaponPickup* SpawnedWeapon = SpawnManager[SpawnPosition];
			SpawnedWeapon->K2_DestroyActor();
			
			AWeaponPickup* WeaponPickup = GetWorld()->SpawnActor<AWeaponPickup>(
				GameInstance->GetWeaponPickupClass(), SpawnPosition, FRotator::ZeroRotator);
			SpawnManager[SpawnPosition] = WeaponPickup;

			UE_LOG(LogTemp, Display, TEXT("Replaced Weapon Pickup Spawned"));
		}
		
		//UE_LOG(LogTemp, Display, TEXT("Weapon Pickup Spawned"))
	}
}

void UPickupManagerSubsystem::PopulateSpawnLocations()
{
	PossibleSpawnLocations.Empty();
	if (UPathfindingSubsystem* PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>())
	{
		PossibleSpawnLocations = PathfindingSubsystem->GetWaypointPositions();
	}
}

void UPickupManagerSubsystem::PopulateSpawnManager()
{
	SpawnManager.Empty();
}
