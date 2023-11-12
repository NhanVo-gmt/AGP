// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManagerSubsystem.h"

#include "EngineUtils.h"
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
	UE_LOG(LogTemp, Warning, TEXT("Enemy Spawn"));
	if (const UAGPGameInstance* GameInstance =
		GetWorld()->GetGameInstance<UAGPGameInstance>())
	{
		for (TActorIterator<APlayerController> It(GetWorld()); It; ++It)
		{
			FVector SpawnLocation = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->FindRandomNodeWithDistanceCondition(It->K2_GetActorLocation(), MinSpawnDistance, MaxSpawnDistance);
			SpawnLocation.Z += 50.0f;
			GetWorld()->SpawnActor<AEnemyCharacter>(GameInstance->GetEnemyCharacterClass(), SpawnLocation, FRotator::ZeroRotator);
		}
	}
}
