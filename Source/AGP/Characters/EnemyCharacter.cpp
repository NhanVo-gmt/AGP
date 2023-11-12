// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "AGP/AGPGameInstance.h"
#include "AGP/MyLegacyCameraShake.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
	
}


FVector AEnemyCharacter::PlayerLocation()
{
	
	return playerLocation;
}

void AEnemyCharacter::AdjustPathfindingError()
{
	PathfindingError += 30;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Spawned Squad Member."))

	CurrentState = EEnemyState::Patrol;
	PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>();
	if (PathfindingSubsystem)
	{
		CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the PathfindingSubsystem"))
	}
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSensedPawn);
	}
}

void AEnemyCharacter::MoveAlongPath()
{
	// Execute the path. Should be called each tick.

	// If the path is empty do nothing.
	if (CurrentPath.IsEmpty()) return;
	
	// 1. Move towards the current stage of the path.
	//		a. Calculate the direction from the current position to the target of the current stage of the path.
	FVector MovementDirection = CurrentPath[CurrentPath.Num()-1] - GetActorLocation();
	double x = FMath::RandRange(-100, 100);
	double y = FMath::RandRange(-100, 100);
	MovementDirection.X += x;
	MovementDirection.Y += y;
	MovementDirection.Normalize();

	//		b. Apply movement in that direction.
	if (SensedCharacter != nullptr || flank == true)
	{
		AddMovementInput(MovementDirection);
	}
	else
	{
		AddMovementInput(MovementDirection, 0.5);
	}
	// 2. Check if it is close to the current stage of the path then pop it off.
	if (FVector::Distance(GetActorLocation(), CurrentPath[CurrentPath.Num() - 1]) < PathfindingError)
	{
		CurrentPath.Pop();
	}
	if (CurrentPath.IsEmpty())
	{
		flank = false;
	}
}


void AEnemyCharacter::TickPatrol()
{
	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	}
	MoveAlongPath();
}

void AEnemyCharacter::TickEngage()
{
	if (!SensedCharacter) return;
	// if (HasWeapon())
	// {
	// 	WeaponComponent->Reload();
	// }
	
	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), SensedCharacter->GetActorLocation());
	}
	MoveAlongPath();
	
	// Fire(SensedCharacter->GetActorLocation());
}

void AEnemyCharacter::TickEvade()
{
	// Find the player and return if it can't find it.
	if (!SensedCharacter) return;

	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetPathAway(GetActorLocation(), SensedCharacter->GetActorLocation());
	}
	MoveAlongPath();
}

bool AEnemyCharacter::CheckIfPlayerInExplodingRange()
{
	if (!SensedCharacter) return false;

	if (FVector::Dist(GetActorLocation(), SensedCharacter->GetActorLocation()) < ReadyToExplodeRadius)
	{
		return true;
	}

	return false;
}

void AEnemyCharacter::ReadyToExplode()
{
	CurrentState = EEnemyState::Explode;
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AEnemyCharacter::Explode, ExplodingDelay, false);
}

void AEnemyCharacter::Explode()
{
	// Set what actors to seek out from it's collision channel
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	// Ignore any specific actors
	TArray<AActor*> ignoreActors;
	// Ignore self or remove this line to not ignore any
	ignoreActors.Init(this, 1);

	// Array of actors that are inside the radius of the sphere
	TArray<AActor*> outActors;
	
	// Sphere's spawn loccation within the world
	FVector sphereSpawnLocation = GetActorLocation();
	// Class that the sphere should hit against and include in the outActors array (Can be null)
	UClass* seekClass = ABaseCharacter::StaticClass(); // NULL;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, ExplodingRadius, traceObjectTypes, seekClass, ignoreActors, outActors);
	
	// Finally iterate over the outActor array
	for (AActor* overlappedActor : outActors) {
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(overlappedActor);
		if (BaseCharacter != nullptr)
		{
			UHealthComponent* BaseHealtComponent = BaseCharacter->GetComponentByClass<UHealthComponent>();
			if (BaseHealtComponent != nullptr)
			{
				BaseHealtComponent->ApplyDamage(ExplodingDamage);
			}
		}
	}
	ServerExplode();
}

void AEnemyCharacter::ServerExplode_Implementation()
{
	MulticastExplode();
}

void AEnemyCharacter::MulticastExplode_Implementation()
{
	if (UAGPGameInstance* GameInstance = Cast<UAGPGameInstance>(GetWorld()->GetGameInstance()))
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(UMyLegacyCameraShake::StaticClass());
		GameInstance->SpawnExplodeParticles(GetActorLocation());
		K2_DestroyActor();
	}
}

void AEnemyCharacter::TickAwaitingOrders()
{
	MoveAlongPath();
}

void AEnemyCharacter::ReceiveOrders(TArray<FVector> orders)
{
	double x = FMath::RandRange(-30, 30);
	double y = FMath::RandRange(-30, 30);
	for(FVector f : orders)
	{
		f.X += x;
		f.Y += y;
	}
	CurrentPath = orders;
}

void AEnemyCharacter::ReceiveOrders(EEnemyState state)
{
	CurrentState = state;
}

void AEnemyCharacter::ReceiveOrders(bool x)
{
	flank = x;
}

float AEnemyCharacter::ReturnHealth()
{
	return HealthComponent->GetCurrentHealth();
}

bool AEnemyCharacter::NeedOrders()
{
	if (CurrentPath.IsEmpty())
	{
		return true;
	}
	return false;
}

bool AEnemyCharacter::PlayerCheck()
{
	if (SensedCharacter)
	{
		return true;
	}
		return false;
	
}

void AEnemyCharacter::SquadBroken()
{
	CurrentState = EEnemyState::Evade;
}


void AEnemyCharacter::OnSensedPawn(APawn* SensedActor)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(SensedActor))
	{
		SensedCharacter = Player;
		playerLocation = SensedCharacter->GetActorLocation();
		UE_LOG(LogTemp, Display, TEXT("Sensed Player"))
	}
}

void AEnemyCharacter::UpdateSight()
{
	if (!SensedCharacter) return;
	if (PawnSensingComponent)
	{
		if (!PawnSensingComponent->HasLineOfSightTo(SensedCharacter))
		{
			SensedCharacter = nullptr;
			UE_LOG(LogTemp, Display, TEXT("Lost Player"))
		}
	}
}


// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSight();

	if (CurrentState != EEnemyState::Explode)
	{
		if (CheckIfPlayerInExplodingRange())
		{
			ReadyToExplode();
		}
		else if (HealthComponent->IsDead())
		{
			Explode();
		}
	}
	
	
	switch(CurrentState)
	{
	case EEnemyState::Patrol:
		TickPatrol();
		if (SensedCharacter)
		{
			if (HealthComponent->GetCurrentHealthPercentage() >= 0.4f)
			{
				CurrentState = EEnemyState::Engage;
			} else
			{
				CurrentState = EEnemyState::Evade;
			}
			CurrentPath.Empty();
		}
		break;
	case EEnemyState::Engage:
		TickEngage();
		if (HealthComponent->GetCurrentHealthPercentage() < 0.4f)
		{
			CurrentPath.Empty();
			CurrentState = EEnemyState::Evade;
		} else if (!SensedCharacter)
		{
			CurrentState = EEnemyState::Patrol;
		}
		break;
	case EEnemyState::Evade:
		TickEvade();
		if (HealthComponent->GetCurrentHealthPercentage() >= 0.4f)
		{
			CurrentPath.Empty();
			CurrentState = EEnemyState::Engage;
		} else if (!SensedCharacter)
		{
			CurrentState = EEnemyState::Patrol;
		}
		break;
	case EEnemyState::ReceivingOrders:
		TickAwaitingOrders();
		break;
	}
	
		
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APlayerCharacter* AEnemyCharacter::FindPlayer() const
{
	APlayerCharacter* Player = nullptr;
	for (TActorIterator<APlayerCharacter> It(GetWorld()); It; ++It)
	{
		Player = *It;
		break;
	}
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the Player Character in the world."))
	}
	return Player;
}

