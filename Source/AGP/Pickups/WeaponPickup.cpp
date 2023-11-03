// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

#include "../Characters/PlayerCharacter.h"

void AWeaponPickup::ShuffleAttributeTypes()
{
	if (WeaponStats.Types.Num() > 0)
	{
		int32 LastIndex = WeaponStats.Types.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				WeaponStats.Types.Swap(i, Index);
			}
		}
	}
}


void AWeaponPickup::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	//Super::OnPickupOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, HitInfo);
	UE_LOG(LogTemp, Display, TEXT("Overlap event occurred on WeaponPickup"))

	if (ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor))
	{

		Player->EquipWeapon(WeaponStats);
		Destroy();

	}
}

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	GenerateWeaponPickup();
	UpdateWeaponPickupMaterial();
}

void AWeaponPickup::GenerateWeaponPickup()
{
	int Chance = FMath::RandRange(0, 100);
	if (Chance < 5)
	{
		WeaponRarity = EWeaponRarity::Legendary;
	}
	else if (Chance < 20)
	{
		WeaponRarity = EWeaponRarity::Master;
	}
	else if (Chance < 50)
	{
		WeaponRarity = EWeaponRarity::Rare;
	}
	else
	{
		WeaponRarity = EWeaponRarity::Common;
	}

	GenerateStat();
}

void AWeaponPickup::GenerateStat()
{
	ShuffleAttributeTypes();

	int GoodAttributes = 0;
	if (WeaponRarity == EWeaponRarity::Legendary)
	{
		GoodAttributes = 4;
	}
	else if (WeaponRarity == EWeaponRarity::Master)
	{
		GoodAttributes = 3;
	}
	else if (WeaponRarity == EWeaponRarity::Rare)
	{
		GoodAttributes = 2;
	}

	for (int i = 0; i < WeaponStats.Types.Num(); i++)
	{
		EStatQuality StatQuality = EStatQuality::Bad;

		if (GoodAttributes > 0)
		{
			StatQuality = EStatQuality::Good;
			GoodAttributes--;
		}
		
		switch (WeaponStats.Types[i])
		{
			case EAttributeType::Accuracy:
				GenerateAccuracyStat(StatQuality);
				break;
			case EAttributeType::FireRate:
				GenerateFireRateStat(StatQuality);
				break;
			case EAttributeType::BaseDamage:
				GenerateBaseDamageStat(StatQuality);
				break;
			case EAttributeType::MagazineSize:
				GenerateMagazineStat(StatQuality);
				break;
			case EAttributeType::ReloadTime:
				GenerateReloadTimeStat(StatQuality);
				break;
			default:
				break;
		}
	}
}

void AWeaponPickup::GenerateAccuracyStat(EStatQuality StatQuality)
{
	if (StatQuality == EStatQuality::Good)
	{
		WeaponStats.Accuracy = FMath::RandRange(0.98f, 1.0f);
	}
	else
	{
		WeaponStats.Accuracy = FMath::RandRange(0.9f, 0.98f);
	}
}

void AWeaponPickup::GenerateFireRateStat(EStatQuality StatQuality)
{
	if (StatQuality == EStatQuality::Good)
	{
		WeaponStats.FireRate = FMath::RandRange(0.05f, 0.2f);
	}
	else
	{
		WeaponStats.FireRate = FMath::RandRange(0.2f, 1.0f);
	}
}

void AWeaponPickup::GenerateBaseDamageStat(EStatQuality StatQuality)
{
	if (StatQuality == EStatQuality::Good)
	{
		WeaponStats.BaseDamage = FMath::RandRange(15.0f, 30.0f);
	}
	else
	{
		WeaponStats.BaseDamage = FMath::RandRange(5.0f, 15.0f);
	}
}

void AWeaponPickup::GenerateMagazineStat(EStatQuality StatQuality)
{
	if (StatQuality == EStatQuality::Good)
	{
		WeaponStats.MagazineSize = FMath::RandRange(20, 100);
	}
	else
	{
		WeaponStats.MagazineSize = FMath::RandRange(1, 20);
	}
}

void AWeaponPickup::GenerateReloadTimeStat(EStatQuality StatQuality)
{
	if (StatQuality == EStatQuality::Good)
	{
		WeaponStats.ReloadTime = FMath::RandRange(0.1f, 1.0f);
	}
	else
	{
		WeaponStats.ReloadTime = FMath::RandRange(1.0f, 4.0f);
	}
}

