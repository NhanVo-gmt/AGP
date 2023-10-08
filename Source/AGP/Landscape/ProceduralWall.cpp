// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralWall.h"

#include "ProceduralMeshComponent.h"

// Sets default values
AProceduralWall::AProceduralWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WallProceduralMeshComponents = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Wall procedural mesh"));
	SetRootComponent(WallProceduralMeshComponents);
}

void AProceduralWall::DestroyWall()
{
	Vertices.Empty();
	Triangles.Empty();
	UVCoords.Empty();
	WallProceduralMeshComponents->ClearMeshSection(0);
	FlushPersistentDebugLines(GetWorld());
	K2_DestroyActor();
}

// Called when the game starts or when spawned
void AProceduralWall::BeginPlay()
{
	Super::BeginPlay();
}

void AProceduralWall::GenerateWall(int32 VertexSpacing, int32 WallHeight)
{
	int32 OffSet = VertexSpacing / 2;
	TArray<FVector> WallVertexLocations;
	
	WallVertexLocations.Add(FVector(-OffSet, OffSet, 0.0f));
	WallVertexLocations.Add(FVector(-OffSet, -OffSet, 0.0f));
	WallVertexLocations.Add(FVector(OffSet, -OffSet, 0.0f));
	WallVertexLocations.Add( FVector(OffSet, OffSet, 0.0f));
	
	for (int i = 0; i < WallVertexLocations.Num(); i++)
	{
		int nextIndex = i+1;
		if (i == WallVertexLocations.Num() - 1)
		{
			nextIndex = 0;
		}
		Vertices.Add(WallVertexLocations[i]);
		Vertices.Add(WallVertexLocations[nextIndex]);
		Vertices.Add(WallVertexLocations[i] + FVector(0.0f, 0.0f, WallHeight));
		Vertices.Add(WallVertexLocations[nextIndex] + FVector(0.0f, 0.0f, WallHeight));
		
		Triangles.Add(i*4 +0);
		Triangles.Add(i*4 +2);
		Triangles.Add(i*4 +1);
		Triangles.Add(i*4 +1);
		Triangles.Add(i*4 +2);
		Triangles.Add(i*4 +3);

		UVCoords.Add(FVector2D(i, i+0.0f));
		UVCoords.Add(FVector2D(i+1.0f, i+0.0f));
		UVCoords.Add(FVector2D(i+0.0f, i+1.0f));
		UVCoords.Add(FVector2D(i+1.0f, i+1.0f));
	}

	GenerateRoof(WallVertexLocations, WallHeight);

	if (WallProceduralMeshComponents)
	{
		WallProceduralMeshComponents->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVCoords, TArray<FColor>(), TArray<FProcMeshTangent>(),true);
	}
}

void AProceduralWall::GenerateRoof(TArray<FVector> WallVertexLocations, int32 WallHeight)
{
	
	Vertices.Add(WallVertexLocations[0] + FVector(0.0f, 0.0f, WallHeight));
	Vertices.Add(WallVertexLocations[1] + FVector(0.0f, 0.0f, WallHeight));
	Vertices.Add(WallVertexLocations[3] + FVector(0.0f, 0.0f, WallHeight));
	Vertices.Add(WallVertexLocations[2] + FVector(0.0f, 0.0f, WallHeight));
		
	Triangles.Add(4*4 +0);
	Triangles.Add(4*4 +2);
	Triangles.Add(4*4 +1);
	Triangles.Add(4*4 +1);
	Triangles.Add(4*4 +2);
	Triangles.Add(4*4 +3);

	UVCoords.Add(FVector2D(4+0.0f, 4+0.0f));
	UVCoords.Add(FVector2D(4+1.0f, 4+0.0f));
	UVCoords.Add(FVector2D(4+0.0f, 4+1.0f));
	UVCoords.Add(FVector2D(4+1.0f, 4+1.0f));
}

// Called every frame
void AProceduralWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

