// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMap.h"

#include "EngineUtils.h"
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

void AProceduralMap::GenerateLandScape()
{
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			FVector VertexLocation = FVector(i * VertexSpacing, j * VertexSpacing, 0.0f);
			DrawDebugSphere(GetWorld(), VertexLocation, 100.0f, 8, FColor::Blue, true, -1);
			
		}
	}
	AProceduralWall* Wall = GetWorld()->SpawnActor<AProceduralWall>(AProceduralWall::StaticClass(), FVector(0.0, 0.0, 0.0f), FRotator::ZeroRotator);
	Wall->GenerateWall(VertexSpacing);
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
		GenerateLandScape();
		// GenerateWall(FVector(0.0f, 0.0f, 0.0f));
	}
}

