// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSpawnSubsystem.h"

#include "MathUtil.h"
#include "AGP/Characters/EnemyCharacter.h"

bool UWorldSpawnSubsystem::IsValid(const FVector2D& Candidate, const int& Width, const int& Height, const int& CellSize, const float& Radius)
{
	if (Candidate.X < 0 || Candidate.Y < 0 || Candidate.X > Width - 1 || Candidate.Y > Height - 1) return false;

	int CellX = (int)Candidate.X / CellSize;
	int CellY = (int)Candidate.Y / CellSize;
	int StartX = FMathf::Max(0, CellX - CellSize - 1);
	int EndX = FMathf::Min(Width - 1, CellX + CellSize + 1);
	int StartY = FMathf::Max(0, CellY - CellSize - 1);
	int EndY = FMathf::Min(Height - 1, CellY + CellSize + 1);
	
	for (int i = StartX; i <= EndX; i++)
	{
		for (int j = StartY; j <= EndY; j++)
		{
			int PointIndex = Grids[i][j] - 1;
			if (PointIndex != -1)
			{
				float SqrDist = FVector2D::DistSquared(Candidate, Points[PointIndex]);
				if (SqrDist < Radius * Radius)
				{
					return false;
				}
			}
		}
	}

	return true;
}

TArray<FVector2D> UWorldSpawnSubsystem::GeneratePoints(const int& Width, const int& Height, const int& CellSize, const int& NumberBeforeRejection)
{
	float Radius = CellSize * FMathf::Sqrt(2);
	Grids.Empty();
	Points.Empty();
	SpawnPoints.Empty();

	for (int i = 0; i < Width; i++)
	{
		TArray<int> Temp;
		Temp.Init(0, Height);
		Grids.Add(Temp);
	}

	SpawnPoints.Add(FVector2D(FMath::RandRange(0, Width - 1), FMath::RandRange(0, Height - 1)));
	while (SpawnPoints.Num() > 0)
	{
		int SpawnIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		FVector2D SpawnPos = SpawnPoints[SpawnIndex];
		bool IsCandidateValid = false;
	
		for (int i = 0; i < NumberBeforeRejection; i++)
		{
			float Angle = FMath::RandRange(0.0f, 1.0f) * FMathf::Pi * 2;
			FVector2D Direction = FVector2D(FMathf::Sin(Angle), FMathf::Cos(Angle));
			FVector2D Candidate = SpawnPos + Direction * FMath::RandRange(Radius, Radius * 2);
			// FVector2D CandidateToGrid = Fv
			if (IsValid(Candidate, Width, Height, CellSize, Radius))
			{
				IsCandidateValid = true;
				Points.Add(Candidate);
				SpawnPoints.Add(Candidate);
				Grids[(int)Candidate.X / CellSize][(int)Candidate.Y / CellSize] = Points.Num();
				break;
			}
		}
		
		if (!IsCandidateValid)
		{
			SpawnPoints.RemoveAt(SpawnIndex);
		}
	}

	return Points;
}

