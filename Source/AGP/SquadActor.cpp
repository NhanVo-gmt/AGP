// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadActor.h"

#include "AGPGameInstance.h"
#include "Pathfinding/PathfindingSubsystem.h"

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
	PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>();
	if (PathfindingSubsystem)
	{
		squadPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the PathfindingSubsystem"))
	}
	for (int i = 0; i < squadSize; i++)
	{
		if (const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>())
		{
			FVector spawnLocation = GetActorLocation();
			spawnLocation.Z += 50;
			AEnemyCharacter* SquadMember = GetWorld()->SpawnActor<AEnemyCharacter>(GameInstance->GetEnemyCharacterClass(), spawnLocation, FRotator::ZeroRotator);
			members.push_back(SquadMember);
			squadHealth += SquadMember->ReturnHealth();
			UE_LOG(LogTemp, Display, TEXT("Spawning Squad Member...")); 
		}
	}

	SquadPatrol();
}

void ASquadActor::SquadRegroup()
{
}

void ASquadActor::SquadRetreat()
{
}

void ASquadActor::SquadPatrol()
{
	if (squadPath.IsEmpty())
	{
		squadPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
		SetActorLocation(squadPath[0]);
	}

	for (AEnemyCharacter* squaddie : members)
	{
		squaddie->ReceiveOrders(squadPath);
	}

	
}

void ASquadActor::SquadFlank()
{
	int i = 0;
	for (AEnemyCharacter* squaddie : members)
	{
		if (i < 2)
		{
			//squaddie->ReceiveOrders();
		}
		else
		{
			//squaddie->ReceiveOrders();
		}
	}
}

bool ASquadActor::OrdersCheck()
{
	for (AEnemyCharacter* squaddie : members)
	{
		if(squaddie->NeedOrders() == false)
		{
			UE_LOG(LogTemp, Display, TEXT("Orders in progress...")); 
			return false;
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Orders completed.")); 
	return true;
	
	
}

bool ASquadActor::PlayerCheck()
{
	for (AEnemyCharacter* squaddie : members)
	{
		if(squaddie->PlayerCheck() == true)
		{
			UE_LOG(LogTemp, Display, TEXT("Enemy sighted...")); 
			return true;
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Orders completed.")); 
	return false;
}

// Called every frame
void ASquadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch(CurrentState)
	{
	case ESquadState::Patrol:
		if (OrdersCheck())
		{
			squadPath.Empty();
			SquadPatrol();
		}
		if (PlayerCheck())
		{
			squadPath.Empty();
			SquadFlank();
			CurrentState = ESquadState::Engage;
		}
		break;
	case EEnemyState::Engage:
		if (!PlayerCheck())
		{
			squadPath.Empty();
			CurrentState = ESquadState::Patrol;
		}
		break;
	}
	
	
	

}

