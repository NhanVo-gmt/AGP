// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerCharacterHUD::SetHealthBar(float HealthPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercent);
	}
}

void UPlayerCharacterHUD::SetAmmoText(int32 RoundsRemaining, int32 MagazineSize)
{
	if (AmmoText)
	{
		AmmoText->SetText(FText::FromString(FString::FromInt(RoundsRemaining) + " / " + FString::FromInt(MagazineSize)));
	}
}

void UPlayerCharacterHUD::SetSpotBar(float SpotPercent)
{
	UE_LOG(LogTemp, Warning, TEXT("Spot Percent: %f"), SpotPercent);
	if (SpotBar)
	{
		SpotBar->SetPercent(SpotPercent);
	}
}
