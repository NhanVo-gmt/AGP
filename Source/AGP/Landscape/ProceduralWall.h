// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ProceduralWall.generated.h"

UCLASS()
class AGP_API AProceduralWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralWall();

	void DestroyWall();
	
protected:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* WallProceduralMeshComponents;
	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector2D> UVCoords;
	UPROPERTY(EditAnywhere)
	int32 WallHeight = 1000;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateWall(int32 VertexSpacing);
	void GenerateRoof(TArray<FVector> WallVertexLocations);
};
