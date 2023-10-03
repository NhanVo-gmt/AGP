// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "AGP/WeaponComponent.h"
#include "WeaponPickup.generated.h"

UENUM(BlueprintType)
enum EWeaponRarity
{
	Common,
	Rare,
	Master,
	Legendary,
};

UENUM()
enum EStatQuality
{
	Good,
	Bad,
};

/**
 * 
 */
UCLASS()
class AGP_API AWeaponPickup : public APickupBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EWeaponRarity> WeaponRarity = EWeaponRarity::Common;

	FWeaponStats WeaponStats;
	
	void ShuffleAttributeTypes();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWeaponPickupMaterial();
	
	virtual void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo) override;

	virtual void BeginPlay() override;

private:
	void GenerateWeaponPickup();

	void GenerateStat();

	void GenerateAccuracyStat(EStatQuality StatQuality);
	void GenerateFireRateStat(EStatQuality StatQuality);
	void GenerateBaseDamageStat(EStatQuality StatQuality);
	void GenerateMagazineStat(EStatQuality StatQuality);
	void GenerateReloadTimeStat(EStatQuality StatQuality);

	
};
