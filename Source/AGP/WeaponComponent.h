// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UENUM(BlueprintType)
enum EWeaponType : uint8
{
	Rifle,
	Pistol
};

UENUM(BlueprintType)
enum EAttributeType
{
	Accuracy,
	FireRate,
	BaseDamage,
	MagazineSize,
	ReloadTime,
};

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()
public:
	
	EWeaponType WeaponType = EWeaponType::Rifle;
	UPROPERTY(VisibleAnywhere)
	float Accuracy = 1.0f;
	UPROPERTY(VisibleAnywhere)
	float FireRate = 0.2f;
	UPROPERTY(VisibleAnywhere)
	float BaseDamage = 10.0f;
	UPROPERTY(VisibleAnywhere)
	int32 MagazineSize = 5;
	UPROPERTY(VisibleAnywhere)
	float ReloadTime = 1.0f;
	

	TArray<EAttributeType> Types = {EAttributeType::Accuracy, EAttributeType::FireRate, EAttributeType::BaseDamage, EAttributeType::MagazineSize, EAttributeType::ReloadTime};
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGP_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	bool Fire(const FVector& BulletStart, const FVector& FireAtLocation);
	void SetStats(const FWeaponStats WeaponStats);
	void Reload();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	FWeaponStats WeaponStats;
	UPROPERTY(VisibleAnywhere)
	int32 RoundsRemaningInMagazine;
	float TimeSinceLastShot = 0.0f;
	float TimeSinceLastReload = 0.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool OutOfBullet();
};
