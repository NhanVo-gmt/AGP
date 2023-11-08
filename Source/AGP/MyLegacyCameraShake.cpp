// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLegacyCameraShake.h"

UMyLegacyCameraShake::UMyLegacyCameraShake()
{
	OscillationDuration = 0.4f;
	OscillationBlendInTime = 0.3f;
	OscillationBlendOutTime = 0.25f;

	RotOscillation.Pitch.Amplitude = 25.0f;
	RotOscillation.Pitch.Frequency = 5.5f;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	RotOscillation.Pitch.Waveform = EOscillatorWaveform::PerlinNoise;

	LocOscillation.Y.Amplitude = 20.0f;
	LocOscillation.Y.Frequency = 1.2f;
	LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	LocOscillation.Y.Waveform = EOscillatorWaveform::SineWave;

	LocOscillation.Z.Amplitude = 20.0f;
	LocOscillation.Z.Frequency = 1.2f;
	LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	LocOscillation.Z.Waveform = EOscillatorWaveform::SineWave;
}
