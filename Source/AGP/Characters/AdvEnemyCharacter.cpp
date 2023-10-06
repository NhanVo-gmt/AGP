// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvEnemyCharacter.h"

// Sets default values
AAdvEnemyCharacter::AAdvEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAdvEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAdvEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAdvEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

