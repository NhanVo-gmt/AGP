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
	//Generate a random amount of squad members.
	squadSize = FMath::RandRange(3, 4);
	//Hook into the pathfinding to create coordinated movement.
	PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>();
	if (PathfindingSubsystem)
	{
		squadPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the PathfindingSubsystem"))
	}
	//Spawn a squad at the spawn location.
	for (int i = 0; i < squadSize; i++)
	{
		if (const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>())
		{
			//Essentially a rework of the pickup system.
			FVector spawnLocation = GetActorLocation();
			spawnLocation.Z += 50;
			AEnemyCharacter* SquadMember = GetWorld()->SpawnActor<AEnemyCharacter>(GameInstance->GetEnemyCharacterClass(), spawnLocation, FRotator::ZeroRotator);
			members.Push(SquadMember);
			squadHealth += SquadMember->ReturnHealth();
			SquadMember->AdjustPathfindingError();
			UE_LOG(LogTemp, Display, TEXT("Spawning Squad Member...")); 
		}
	}
	maxHealth = squadHealth;
	//Set the default state.
	SquadPatrol();
}
//Currently unimplemented.
void ASquadActor::SquadRegroup()
{
}
//Currently unimplemented.
void ASquadActor::SquadRetreat()
{
}
//Patrol state behaviour -- Move the squad together across the map.
void ASquadActor::SquadPatrol()
{
	//Generate a patrol path.
	if (squadPath.IsEmpty())
	{
		squadPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
		SetActorLocation(squadPath[0]);
	}

	//Send the path to all squad members.
	for (AEnemyCharacter* squaddie : members)
	{
		squaddie->ReceiveOrders(squadPath);
	}

	
}

//Although the function doesn't always create a flanking manuever, it will at minimum cause a few members to take a different evasive action from the rest.
void ASquadActor::SquadScatter()
{
	UE_LOG(LogTemp, Display, TEXT("Begin flanking action.")); 
	//Variable keeping track of squad members.
	int i = 0;

	// FVector flankLocation = PathfindingSubsystem->GetPathAway(playerLocation);
	// flankLocation += 

	// Old flank finding code. Inconsistent due to random seeds and level layouts.
	// Variable determining which way to flank.
	// int x = FMath::RandRange(int(0), int(1));
	 //A "midpoint" where the ai will attempt to go to first before heading to the player.
	 
	 //Decide whether it is x or y based on RandRange. Also decide whether it is positive or negative through the same means.
	 // if (x == 0)
	 // {
	 // 	flankLocation.X += 500 * (FMath::RandRange(int(-1), int(1)));
	 // }
	 // else
	 // {
	 // 	flankLocation.Y += 500 * (FMath::RandRange(int(-1), int(1)));
	 //
	 // }

	UE_LOG(LogTemp, Display, TEXT("Scattering!")); 

	//Path to somewhere away from the player. Then move from there to the player. Ideally, this can simulate the the first two members of the squad splitting off.
	//What's interesting, at least for me, is that they act on the last time the player was seen which simulates that enemies aren't omniscient and are only acting off what their last order was.
	//buffer to hold the flanking path.
	//populate buffer path. Since its a path away this range from just splitting up to actually taking alternate route -- which can make their tactics feel more dynamic.
	TArray<FVector> bufferPath = PathfindingSubsystem->GetPathAway(members[0]->GetActorLocation(), playerLocation);
	
	int y = 0;
	// Small check to see if the scatter is too long, uses an arbitrary number.
	 if (bufferPath.Num() >= 3)
	 {
	 	y = int(bufferPath.Num()/2);
	 }
	FVector vertexLocation = bufferPath[y];

	int z = 0;
	// Small check to see if the scatter is too long, uses an arbitrary number.
	TArray<FVector> bufferRandPath = PathfindingSubsystem->GetRandomPath(vertexLocation);
	if (bufferPath.Num() >= 3)
	{
		z = bufferRandPath.Num()/2;
	}
	FVector bufferRandDest = bufferRandPath[z];
	//Create a path from the flank to the player.
	PlayerCheck();
	squadPath = PathfindingSubsystem->GetPath(bufferRandDest, playerLocation);

	//The additional paths are appended as such to account for possible halving of the paths.
	for (int x = z; x < bufferRandPath.Num(); x++)
	{
		squadPath.Push(bufferRandPath[x]);
	}
	for (int x = y; x < bufferPath.Num(); x++)
	{
		squadPath.Push(bufferPath[x]);
	}
	
	// Issue orders.
	for (AEnemyCharacter* squaddie : members)
	{
		
		//A small "loop" to only issue the flank to the first two members.
		//Note for video, describe the many iterations of this.
		//Arbitrary amount of squad members to take a flank action.
		if (i < 2)
		{
			
			squaddie->ReceiveOrders(squadPath);
			i++;
		}
		else
		{
			//None scattering members will just pursue the player.
			squadPath.Empty();
			PlayerCheck();
			squadPath = PathfindingSubsystem->GetPath(squaddie->GetActorLocation(), playerLocation);
			squaddie->ReceiveOrders(squadPath);
		}
		//Communicate to the whole squad to Engage. This allows them to take on individual action.
		squaddie->ReceiveOrders(EEnemyState::Engage);
	}
}
//Checking if the squad is on the move or not.
bool ASquadActor::OrdersCheck()
{
	for (AEnemyCharacter* squaddie : members)
	{
		if(squaddie->NeedOrders() == false)
		{
			
			return false;
		}
		else
		{
			return true;
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Orders completed.")); 
	return true;
	
	
}
//Check if anyone in the squad has sight on the player.
bool ASquadActor::PlayerCheck()
{
	for (AEnemyCharacter* squaddie : members)
	{
		if(squaddie->PlayerCheck() == true)
		{
			//UE_LOG(LogTemp, Display, TEXT("Enemy sighted..."));
			playerLocation = squaddie->PlayerLocation();
			return true;
		}
	}
	//UE_LOG(LogTemp, Display, TEXT("Enemy lost.")); 
	return false;
}
//helper function to reset all pathfinding.
void ASquadActor::ClearOrders()
{
	squadPath.Empty();
	for (AEnemyCharacter* squaddie : members)
	{
		
		squaddie->ReceiveOrders(squadPath);
	}
	
}

// Called every frame
void ASquadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Prototype feature to break squad when down a third of their strength.
	if (squadHealth <= maxHealth/3)
	{
		CurrentState = ESquadState::Broken;
	}
	//A state machine similar to the EnemyCharacter state machine.
	switch(CurrentState)
	{
	case ESquadState::Patrol:
		//UE_LOG(LogTemp, Display, TEXT("Patrolling.")); 

		if (OrdersCheck())
		{
			squadPath.Empty();
			SquadPatrol();
		}
		if (PlayerCheck())
		{
			ClearOrders();
			SquadScatter();
			CurrentState = ESquadState::Engage;
		}
		break;
	case ESquadState::Engage:
		//UE_LOG(LogTemp, Display, TEXT("Engaging.")); 
		if (!PlayerCheck())
		{
			UE_LOG(LogTemp, Display, TEXT("Enemy lost.")); 
			squadPath.Empty();
			CurrentState = ESquadState::Patrol;
		}
		break;
	//Currently unused. Would get called based on the broken condition at the start of tick. Would make all enemies act individually.
	case ESquadState::Broken:
		for (AEnemyCharacter* squaddie : members)
		{
			squaddie->SquadBroken();
		}
		break;
	}
	
	

}

