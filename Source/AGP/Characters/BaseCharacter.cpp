// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "HealthComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletStartPosition = CreateDefaultSubobject<USceneComponent>("Bullet Start");
	BulletStartPosition->SetupAttachment(GetRootComponent());
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ABaseCharacter::Fire(const FVector& FireAtLocation)
{
	if (HasWeapon())
	{
		WeaponComponent->Fire(BulletStartPosition->GetComponentLocation(), FireAtLocation);
		return true;
	}

	return false;
}

void ABaseCharacter::Reload()
{
	if (HasWeapon())
	{
		WeaponComponent->Reload();
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

bool ABaseCharacter::HasWeapon()
{
	return WeaponComponent != nullptr;
}

void ABaseCharacter::EquipWeapon(FWeaponStats WeaponStats)
{
	if (!HasWeapon())
	{
		WeaponComponent = NewObject<UWeaponComponent>(this);
		WeaponComponent->RegisterComponent();
	}
	else 
	{
		// WeaponComponent->UnregisterComponent();
		// WeaponComponent = nullptr;
	}

	WeaponComponent->SetStats(WeaponStats);
} 

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

