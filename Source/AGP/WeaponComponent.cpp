// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/HealthComponent.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UWeaponComponent::Fire(const FVector& BulletStart, const FVector& FireAtLocation)
{
	if (RoundsRemaningInMagazine <= 0 || TimeSinceLastReload > 0) return false;
	
	// Determine if the character is able to fire.
	if (TimeSinceLastShot < WeaponStats.FireRate)
	{
		return false;
	}

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(HitResult, BulletStart, FireAtLocation, ECC_WorldStatic, QueryParams))
	{
		if (ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(HitResult.GetActor()))
		{
			if (UHealthComponent* HitCharacterHealth = HitCharacter->GetComponentByClass<UHealthComponent>())
			{
				HitCharacterHealth->ApplyDamage(WeaponStats.BaseDamage);
			}
			DrawDebugLine(GetWorld(), BulletStart, HitResult.ImpactPoint, FColor::Green, false, 1.0f);
		}
		else
		{
			DrawDebugLine(GetWorld(), BulletStart, HitResult.ImpactPoint, FColor::Orange, false, 1.0f);
		}
		
	}
	else
	{
		DrawDebugLine(GetWorld(), BulletStart, FireAtLocation, FColor::Red, false, 1.0f);
	}

	TimeSinceLastShot = 0.0f;
	RoundsRemaningInMagazine--;
	return true;
}

void UWeaponComponent::SetStats(const FWeaponStats NewWeaponStats)
{
	WeaponStats.Accuracy = NewWeaponStats.Accuracy;
	WeaponStats.FireRate = NewWeaponStats.FireRate;
	WeaponStats.BaseDamage = NewWeaponStats.BaseDamage;
	WeaponStats.MagazineSize = NewWeaponStats.MagazineSize;
	WeaponStats.ReloadTime = NewWeaponStats.ReloadTime;

	RoundsRemaningInMagazine = WeaponStats.MagazineSize;
}

void UWeaponComponent::Reload()
{
	TimeSinceLastReload = WeaponStats.ReloadTime;
	RoundsRemaningInMagazine = WeaponStats.MagazineSize;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastShot += DeltaTime;
	TimeSinceLastReload -= DeltaTime;
}

bool UWeaponComponent::OutOfBullet()
{
	return RoundsRemaningInMagazine <= 0;
}

