// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacter.h"
#include "Engine/GameInstance.h"
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
	
public:
	UClass* GetWeaponPickupClass() const;
	UClass* GetEnemyCharacterClass() const;
	
};
