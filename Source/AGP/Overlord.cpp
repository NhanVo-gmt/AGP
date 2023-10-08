// Fill out your copyright notice in the Description page of Project Settings.


#include "Overlord.h"

#include "EngineUtils.h"

// Sets default values
AOverlord::AOverlord()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOverlord::BeginPlay()
{
	Super::BeginPlay();
	

	for (TActorIterator<APlayerCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(*ActorItr);
	}
}

//Finds somewhere far to spawn the player.
void AOverlord::FindSpawnLocation()
{
	spawnLocation = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->FindFurthestNode(playerLocation)->GetActorLocation();

}

//Spawns squad according to the Allowed Squads variable.
void AOverlord::SpawnEnemySquad()
{
	for (int i = 0; i < allowedSquads; i++)
	{
		FindSpawnLocation();
		if (const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>())
		{
			ASquadActor* Squad = GetWorld()->SpawnActor<ASquadActor>(GameInstance->GetSquadActorClass(), spawnLocation, FRotator::ZeroRotator);
			activeSquads.push_back(Squad);
			UE_LOG(LogTemp, Display, TEXT("Spawning Squad...")); 
		}
	}
}

// Called every frame
void AOverlord::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	playerLocation = PlayerCharacter->GetActorLocation();
	
	//UE_LOG(LogTemp, Display, TEXT("The float value is: %d"), int(activeSquads.size())); 

	if (activeSquads.size() < allowedSquads)
	{
		SpawnEnemySquad();
	}
	

}

