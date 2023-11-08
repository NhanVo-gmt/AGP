// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPGameInstance.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

UClass* UAGPGameInstance::GetWeaponPickupClass() const
{
	return WeaponPickupClass.Get();
}

UClass* UAGPGameInstance::GetEnemyCharacterClass() const
{
	return EnemyCharacterClass.Get();
}

UClass* UAGPGameInstance::GetSquadActorClass() const
{
	return SquadActorClass.Get();
}

void UAGPGameInstance::SpawnGroundHitParticles(const FVector& SpawnLocation)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GroundHitParticle, SpawnLocation);
}

void UAGPGameInstance::SpawnBloodSplashParticles(const FVector& SpawnLocation)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplashParticle, SpawnLocation);
}

void UAGPGameInstance::SpawnExplodeParticles(const FVector& SpawnLocation)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplodeParticle, SpawnLocation);
}

void UAGPGameInstance::PlayGunshotAtLocation(const FVector& Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunshotSoundCue, Location, FRotator::ZeroRotator);
}

void UAGPGameInstance::PlayGunshotSound2D()
{
	UGameplayStatics::PlaySound2D(GetWorld(), GunshotSoundCue);
}


