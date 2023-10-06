// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadActor.h"

#include "AGPGameInstance.h"

// Sets default values
ASquadActor::ASquadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Location Component"));
	SetRootComponent(LocationComponent);

}

// Called when the game starts or when spawned
void ASquadActor::BeginPlay()
{
	Super::BeginPlay();
	squadSize = FMath::RandRange(2, 3);
	for (int i = 0; i < squadSize; i++)
	{
		if (const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>())
		{
			FVector spawnLocation = GetActorLocation();
			spawnLocation.Z += 50;
			AEnemyCharacter* SquadMember = GetWorld()->SpawnActor<AEnemyCharacter>(GameInstance->GetEnemyCharacterClass(), spawnLocation, FRotator::ZeroRotator);
			members.push_back(SquadMember);
			UE_LOG(LogTemp, Display, TEXT("Spawning Squad Member...")); 
		}
	}
	
}

void ASquadActor::SquadRegroup()
{
}

void ASquadActor::SquadRetreat()
{
}

void ASquadActor::SquadInvestigate()
{
}

void ASquadActor::SquadFlank()
{
}

// Called every frame
void ASquadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

