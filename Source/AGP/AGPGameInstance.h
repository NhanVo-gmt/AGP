// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SquadActor.h"
#include "Characters/EnemyCharacter.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundCue.h"
#include "NiagaraSystem.h"
#include "Pickups/WeaponPickup.h"
#include "AGPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UAGPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Pickup Classes")
	TSubclassOf<AWeaponPickup> WeaponPickupClass;
	UPROPERTY(EditDefaultsOnly, Category="Enemy Classes")
	TSubclassOf<AEnemyCharacter> EnemyCharacterClass;
	UPROPERTY(EditDefaultsOnly, Category="Squad Classes")
	TSubclassOf<ASquadActor> SquadActorClass;

	UPROPERTY(EditDefaultsOnly, Category="Particle System")
	UNiagaraSystem* GroundHitParticle;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* BloodSplashParticle;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* ExplodeParticle;
	UPROPERTY(EditDefaultsOnly)
	USoundCue* GunshotSoundCue;
	
public:
	UClass* GetWeaponPickupClass() const;
	UClass* GetEnemyCharacterClass() const;
	UClass* GetSquadActorClass() const;

	void SpawnGroundHitParticles(const FVector& SpawnLocation);
	void SpawnBloodSplashParticles(const FVector& SpawnLocation);
	void SpawnExplodeParticles(const FVector& SpawnLocation);
	void PlayGunshotAtLocation(const FVector& Location);
	void PlayGunshotSound2D();
};
