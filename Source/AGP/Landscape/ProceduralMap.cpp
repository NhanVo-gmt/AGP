// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMap.h"

#include "EngineUtils.h"
#include "ProceduralGenerationAlgorithm.h"
#include "ProceduralWall.h"
#include "AGP/AGPGameInstance.h"

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
	Walls.Empty();
	
	for (TActorIterator<AProceduralWall> It(GetWorld()); It; ++It)
	{
		It->DestroyWall();
	}
}

void AProceduralMap::GenerateWalls()
{
	TSet<FVector2D> Set;
	Set.Empty();
	for (int i = 0; i < Iterations; i++)
	{
		FVector2D StartPos = FVector2D(FMath::RandRange(0, Width - 1), FMath::RandRange(0, Height - 1));
		UE_LOG(LogTemp, Warning, TEXT("StartPos: %s"), *StartPos.ToString());
		Set = RunRandomWalk(StartPos);
	
		for (auto& Element : Set)
		{
			FVector SpawnPos = FVector(Element.X * VertexSpacing, Element.Y * VertexSpacing, 0);
			DrawDebugSphere(GetWorld(), SpawnPos, 100, 8, FColor::Blue, true, -1);
			
			AProceduralWall* Wall = GetWorld()->SpawnActor<AProceduralWall>(AProceduralWall::StaticClass(), SpawnPos, FRotator::ZeroRotator);
			Wall->GenerateWall(VertexSpacing);
		}
	
		Set.Empty();
	}

	// FVector SpawnPos = FVector(1 * VertexSpacing, 1 * VertexSpacing, 0);
	// AProceduralWall* Wall = GetWorld()->SpawnActor<AProceduralWall>(AProceduralWall::StaticClass(), SpawnPos, FRotator::ZeroRotator);
	// Wall->GenerateWall(VertexSpacing);
}

// Called every frame
void AProceduralMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldGenerate)
	{
		bShouldGenerate = false;
		ClearMap();
		CreateSimplePlane();
		GenerateWalls();
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

