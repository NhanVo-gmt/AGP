// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManagerSubsystem.h"

#include "EngineUtils.h"
#include "MathUtil.h"
#include "AGP/AGPGameInstance.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"

void UEnemyManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetNetMode() >= NM_Client)
	{
		return;
	}

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= EnemySpawnRate)
	{
		SpawnEnemy();
		TimeSinceLastSpawn = 0.0f;
	}
}

void UEnemyManagerSubsystem::SpawnEnemy()
{
	PopulatePlayers();
	if (PlayerControllers.Num() == 0) return;
	
	if (const UAGPGameInstance* GameInstance =
		GetWorld()->GetGameInstance<UAGPGameInstance>())
	{
		int rand = FMath::RandRange(0, 5);
		if (rand == 5)
		{
			FVector SpawnLocation = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->FindRandomNodeWithDistanceCondition(PlayerControllers[FMath::RandRange(0, 1)]->K2_GetActorLocation(), MinSpawnDistance, MaxSpawnDistance);
			SpawnLocation.Z += 50.0f;
			GetWorld()->SpawnActor<ASquadActor>(GameInstance->GetSquadActorClass(), SpawnLocation, FRotator::ZeroRotator);
		}
		else
		{
			FVector SpawnLocation = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->FindRandomNodeWithDistanceCondition(PlayerControllers[FMath::RandRange(0, 1)]->K2_GetActorLocation(), MinSpawnDistance, MaxSpawnDistance);
			SpawnLocation.Z += 50.0f;
			GetWorld()->SpawnActor<AEnemyCharacter>(GameInstance->GetEnemyCharacterClass(), SpawnLocation, FRotator::ZeroRotator);
		}
		
	}
}

void UEnemyManagerSubsystem::PopulatePlayers()
{
	if (PlayerControllers.Num() > 0) return;
	
	for (TActorIterator<APlayerController> It(GetWorld()); It; ++It)
	{
		PlayerControllers.Add(*It);
	}
}
