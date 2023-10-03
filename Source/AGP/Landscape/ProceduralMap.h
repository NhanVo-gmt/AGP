// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralWall.h"
#include "GameFramework/Actor.h"
#include "ProceduralMap.generated.h"

UCLASS()
class AGP_API AProceduralMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMap();

	virtual bool ShouldTickIfViewportsOnly() const override;
	
protected:

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMeshComponent;
	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector2D> UVCoords;
	UPROPERTY()
	TArray<AProceduralWall*> Walls;

	UPROPERTY(EditAnywhere)
	bool bShouldGenerate;
	UPROPERTY(EditAnywhere)
	int32 Width;
	UPROPERTY(EditAnywhere)
	int32 Height;
	UPROPERTY(EditAnywhere)
	int32 VertexSpacing;
	
	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateSimplePlane();
	void ClearMap();
	void GenerateLandScape();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
