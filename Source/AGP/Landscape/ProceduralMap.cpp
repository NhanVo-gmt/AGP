// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMap.h"

#include "EngineUtils.h"
#include "ProceduralGenerationAlgorithm.h"
#include "ProceduralWall.h"
#include "AGP/AGPGameInstance.h"
#include "AGP/Characters/EnemyCharacter.h"
#include "AGP/SpawnSystem/WorldSpawnSubsystem.h"
#include "Core/Tests/Containers/TestUtils.h"

// Sets default values
AProceduralMap::AProceduralMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	SetRootComponent(ProceduralMeshComponent);
}

bool AProceduralMap::ShouldTickIfViewportsOnly() const
{
	return true;
}

bool AProceduralMap::IsValid(FVector2D Pos)
{
	return Pos.X > 0 && Pos.X <= Width - 1 && Pos.Y > 0 && Pos.Y <= Height - 1;
}

bool AProceduralMap::IsValid(FVector Pos)
{
	return Pos.X > 0 && Pos.X <= Width - 1 && Pos.Y > 0 && Pos.Y <= Height - 1;
}

bool AProceduralMap::IsInWall(FVector2D SpawnPos)
{
	if (Walls.Num() <= 0) return true;
	
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			if (!Walls[i][j]) continue;

			
			if (FVector2D::DistSquared(FVector2D(i, j), SpawnPos) < 1)
			{
				return true;
			}
		}
	}

	return false;
}


// Called when the game starts or when spawned
void AProceduralMap::BeginPlay()
{
	Super::BeginPlay();
}

void AProceduralMap::CreateSimplePlane()
{
	int32 OffSet = -VertexSpacing / 2;
	Vertices.Add(FVector(OffSet, OffSet, 0.0f));
	Vertices.Add(FVector(Width * VertexSpacing + OffSet, OffSet, 0.0f));
	Vertices.Add(FVector(OffSet, Height * VertexSpacing + OffSet, 0.0f));
	Vertices.Add(FVector(Width * VertexSpacing + OffSet, Height * VertexSpacing + OffSet, 0.0f));
	
	Triangles.Add(0);
	Triangles.Add(2);
	Triangles.Add(1);
	Triangles.Add(1);
	Triangles.Add(2);
	Triangles.Add(3);
	
	UVCoords.Add(FVector2D(0.0f, 0.0f));
	UVCoords.Add(FVector2D(1.0f, 0.0f));
	UVCoords.Add(FVector2D(0.0f, 1.0f));
	UVCoords.Add(FVector2D(1.0f, 1.0f));

	if (ProceduralMeshComponent)
	{
		ProceduralMeshComponent->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVCoords, TArray<FColor>(), TArray<FProcMeshTangent>(),true);
	}
}

void AProceduralMap::ClearMap()
{
	Vertices.Empty();
	Triangles.Empty();
	UVCoords.Empty();
	ProceduralMeshComponent->ClearMeshSection(0);
	
	FlushPersistentDebugLines(GetWorld());

	DestroyWalls();
}

void AProceduralMap::GenerateInsideWalls()
{
	TSet<FVector2D> Set;
	Set.Empty();

	for (int i = 0; i < Width; i++)
	{
		TArray<bool> Temp;
		Temp.Init(false, Height);
		Walls.Add(Temp);
	}
	
	for (int i = 0; i < Iterations; i++)
	{
		FVector2D StartPos = FVector2D(FMath::RandRange(0, Width - 1), FMath::RandRange(0, Height - 1));

		Set = RunRandomWalk(StartPos);
	
		for (auto& Element : Set)
		{
			if (IsValid(Element))
			{
				FVector SpawnPos = FVector(Element.X * VertexSpacing, Element.Y * VertexSpacing, 0);
				
				AProceduralWall* Wall = GetWorld()->SpawnActor<AProceduralWall>(AProceduralWall::StaticClass(), SpawnPos, FRotator::ZeroRotator);
				Wall->GenerateWall(VertexSpacing, 400);
				Walls[Element.X][Element.Y] = true;
			}
		}
	
		Set.Empty();
	}
}

void AProceduralMap::GenerateOutsideWalls()
{
	for (int i = 0; i < Width; i++)
	{
		FVector SpawnPos = FVector(i * VertexSpacing, -VertexSpacing, 0.0);
		AProceduralWall* Wall = GetWorld()->SpawnActor<AProceduralWall>(AProceduralWall::StaticClass(), SpawnPos, FRotator::ZeroRotator);
		Wall->GenerateWall(VertexSpacing, 1000);

		SpawnPos = FVector(i * VertexSpacing, (Height) * VertexSpacing - VertexSpacing / 2, 0.0);
		Wall = GetWorld()->SpawnActor<AProceduralWall>(AProceduralWall::StaticClass(), SpawnPos, FRotator::ZeroRotator);
		Wall->GenerateWall(VertexSpacing, 1000);
	}

	for (int i = 0; i < Height; i++)
	{
		FVector SpawnPos = FVector(-VertexSpacing, i * VertexSpacing, 0.0);
		AProceduralWall* Wall = GetWorld()->SpawnActor<AProceduralWall>(AProceduralWall::StaticClass(), SpawnPos, FRotator::ZeroRotator);
		Wall->GenerateWall(VertexSpacing, 1000);

		SpawnPos = FVector(Width * VertexSpacing - VertexSpacing / 2, i * VertexSpacing, 0.0);
		Wall = GetWorld()->SpawnActor<AProceduralWall>(AProceduralWall::StaticClass(), SpawnPos, FRotator::ZeroRotator);
		Wall->GenerateWall(VertexSpacing, 1000);
	}
}

void AProceduralMap::DestroyWalls()
{
	Walls.Empty();
	for (TActorIterator<AProceduralWall> It(GetWorld()); It; ++It)
	{
		It->DestroyWall();
	}
}

void AProceduralMap::GeneratePickups()
{
	TArray<FVector2D> SpawnPoints = GetWorld()->GetSubsystem<UWorldSpawnSubsystem>()->GeneratePoints(Width, Height, 1, NumberBeforeRejection);
	UE_LOG(LogTemp, Warning, TEXT("SpawnPoints: %d"), SpawnPoints.Num());

	int MaxPickupSpawned = MaxPickup;
	Test::Shuffle(SpawnPoints);
	for (int i = 0; i < SpawnPoints.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnPoints: %s"), *SpawnPoints[i].ToString());

		if (IsInWall(SpawnPoints[i]))
		{
			continue;
		}
		
		FVector SpawnPos = FVector(SpawnPoints[i].X * VertexSpacing, SpawnPoints[i].Y * VertexSpacing, 100);
		AWeaponPickup* WeaponPickup = GetWorld()->SpawnActor<AWeaponPickup>(AWeaponPickup::StaticClass(), SpawnPos, FRotator::ZeroRotator);
		MaxPickupSpawned--;
		if (MaxPickupSpawned <= 0)
		{
			break;
		}
	}

	
}

void AProceduralMap::DestroyPickups()
{
	for (TActorIterator<AWeaponPickup> It(GetWorld()); It; ++It)
	{
		GetWorld()->DestroyActor(*It);
	}
}

// Called every frame
void AProceduralMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGenerateMap)
	{
		bGenerateMap = false;
		ClearMap();
		CreateSimplePlane();
		GenerateOutsideWalls();
		GenerateInsideWalls();
	}

	if (bGeneratePickup)
	{
		bGeneratePickup = false;
		DestroyPickups();
		GeneratePickups();
	}
}

void AProceduralMap::RunProceduralGeneration(FVector2D StartPos)
{
	TSet<FVector2D> FloorPos = RunRandomWalk(StartPos);
	for (auto& Element : FloorPos)
	{
		
	}
}

TSet<FVector2D> AProceduralMap::RunRandomWalk(FVector2D StartPos)
{
	FVector2D CurrentPos = StartPos;
	TSet<FVector2D> FloorPos;
	for (int i = 0; i < IterationsEachWall; i++)
	{
		TSet<FVector2D> Path = GetWorld()->GetSubsystem<UProceduralGenerationAlgorithm>()->SimpleRandomWalk(CurrentPos, WalkLength);
		FloorPos = FloorPos.Union(Path);
		if (StartRandomlyEachIteration)
		{
			CurrentPos = MyUtilities::ElementAt(FloorPos, FMath::RandRange(0, FloorPos.Num() - 1));
		}
	}

	return FloorPos;
}

