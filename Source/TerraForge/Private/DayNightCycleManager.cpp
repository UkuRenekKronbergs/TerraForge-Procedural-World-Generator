// TerraForge - Procedural World Generator
// Day Night Cycle Manager Implementation

#include "DayNightCycleManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"

ADayNightCycleManager::ADayNightCycleManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create default sun light component
	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
	RootComponent = SunLight;
	
	SunLight->SetIntensity(DayIntensity);
	SunLight->SetLightColor(DayColor);
	SunLight->SetCastShadows(true);

	// Create default moon light component
	MoonLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MoonLight"));
	MoonLight->SetupAttachment(RootComponent);
	MoonLight->SetIntensity(0.0f);
	MoonLight->SetLightColor(MoonColor);
	MoonLight->SetCastShadows(false);
}

void ADayNightCycleManager::BeginPlay()
{
	Super::BeginPlay();
	
	// If a directional light actor is specified, use its directional light component when available
	if (DirectionalLightActor)
	{
		if (UDirectionalLightComponent* DirectionalComponent = Cast<UDirectionalLightComponent>(DirectionalLightActor->GetLightComponent()))
		{
			SunLight = DirectionalComponent;
			RootComponent = SunLight;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DayNightCycleManager: DirectionalLightActor missing UDirectionalLightComponent."));
		}
	}

	if (MoonDirectionalLightActor)
	{
		if (UDirectionalLightComponent* DirectionalComponent = Cast<UDirectionalLightComponent>(MoonDirectionalLightActor->GetLightComponent()))
		{
			MoonLight = DirectionalComponent;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DayNightCycleManager: MoonDirectionalLightActor missing UDirectionalLightComponent."));
		}
	}
	
	// Initialize sun position and properties
	UpdateSunPosition();
	UpdateSunProperties();
	UpdateClockDisplay(0.0f);
}

void ADayNightCycleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAutoProgress)
	{
		// Progress time
		TimeOfDay += CycleSpeed * DeltaTime;
		
		// Wrap around after 24 hours
		if (TimeOfDay >= 24.0f)
		{
			TimeOfDay = TimeOfDay - 24.0f;
		}
	}

	// Update sun position and properties
	UpdateSunPosition();
	UpdateSunProperties();
	UpdateClockDisplay(DeltaTime);
}

void ADayNightCycleManager::SetTimeOfDay(float NewTime)
{
	TimeOfDay = FMath::Clamp(NewTime, 0.0f, 24.0f);
	UpdateSunPosition();
	UpdateSunProperties();
}

void ADayNightCycleManager::UpdateSunPosition()
{
	if (!SunLight)
	{
		return;
	}

	// Calculate sun angle (0 degrees at noon, pointing down; 180 degrees at midnight, pointing up)
	float SunAngle = CalculateSunAngle();
	
	// Create rotation for the sun
	// Pitch controls the sun's elevation, Yaw rotates around the horizon
	FRotator SunRotation;
	SunRotation.Pitch = SunAngle;
	SunRotation.Yaw = 0.0f; // Can be modified for east-west sun movement
	SunRotation.Roll = 0.0f;
	
	// Apply rotation to the light
	SetActorRotation(SunRotation);
	if (SunLight)
	{
		SunLight->SetWorldRotation(SunRotation);
	}

	if (MoonLight && bEnableMoonLight)
	{
		FRotator MoonRotation;
		MoonRotation.Pitch = CalculateMoonAngle();
		MoonRotation.Yaw = 0.0f;
		MoonRotation.Roll = 0.0f;
		MoonLight->SetWorldRotation(MoonRotation);
	}
}

void ADayNightCycleManager::UpdateSunProperties()
{
	if (!SunLight)
	{
		return;
	}

	const float SunAngleDeg = CalculateSunAngle();
	const float SunAngleRad = FMath::DegreesToRadians(SunAngleDeg);
	const float SunHeight = FMath::Sin(SunAngleRad); // -1 (midnight) to 1 (noon)

	// Smooth transition around horizon using twilight length
	const float TwilightHalf = TwilightLength * 0.5f;
	const float DawnStart = SunriseTime - TwilightHalf;
	const float DawnEnd = SunriseTime + TwilightHalf;
	const float DuskStart = SunsetTime - TwilightHalf;
	const float DuskEnd = SunsetTime + TwilightHalf;

	auto SmoothStepRange = [](float Value, float Edge0, float Edge1)
	{
		const float Clamped = FMath::Clamp((Value - Edge0) / (Edge1 - Edge0), 0.0f, 1.0f);
		return Clamped * Clamped * (3.0f - 2.0f * Clamped);
	};

	float DayFactor = 0.0f;
	if (TimeOfDay >= DawnEnd && TimeOfDay <= DuskStart)
	{
		DayFactor = 1.0f;
	}
	else if (TimeOfDay >= DawnStart && TimeOfDay < DawnEnd)
	{
		DayFactor = SmoothStepRange(TimeOfDay, DawnStart, DawnEnd);
	}
	else if (TimeOfDay > DuskStart && TimeOfDay <= DuskEnd)
	{
		DayFactor = 1.0f - SmoothStepRange(TimeOfDay, DuskStart, DuskEnd); // reverse fade out
	}

	const float CurrentIntensity = FMath::Lerp(NightIntensity, DayIntensity, DayFactor);

	FLinearColor CurrentColor;
	if (DayFactor <= 0.01f)
	{
		CurrentColor = NightColor;
	}
	else if (DayFactor >= 0.99f)
	{
		CurrentColor = DayColor;
	}
	else
	{
		// Blend night -> sunrise/sunset -> day based on factor
		FLinearColor MidColor = SunsetColor;
		CurrentColor = FMath::Lerp(NightColor, MidColor, FMath::Clamp(DayFactor * 2.0f, 0.0f, 1.0f));
		CurrentColor = FMath::Lerp(CurrentColor, DayColor, FMath::Clamp((DayFactor - 0.5f) * 2.0f, 0.0f, 1.0f));
	}

	SunLight->SetIntensity(CurrentIntensity);
	SunLight->SetLightColor(CurrentColor);

	if (MoonLight)
	{
		const float MoonFactor = bEnableMoonLight ? (1.0f - DayFactor) : 0.0f;
		MoonLight->SetIntensity(FMath::Max(0.0f, MoonIntensity * MoonFactor));
		MoonLight->SetLightColor(MoonColor);
		MoonLight->SetVisibility(MoonFactor > 0.01f);
	}
}

float ADayNightCycleManager::CalculateSunAngle() const
{
	// Map 24-hour time to 360-degree rotation
	// 0:00 (midnight) = 0 degrees (sun below horizon)
	// 6:00 (sunrise) = -90 degrees
	// 12:00 (noon) = -180 degrees (sun directly overhead when rotated)
	// 18:00 (sunset) = -270 degrees
	// 24:00 (midnight) = -360 degrees (same as 0)
	
	float NormalizedTime = TimeOfDay / 24.0f; // 0 to 1
	float Angle = (NormalizedTime * 360.0f) - 90.0f; // Offset so sun rises in the east
	
	return Angle;
}

float ADayNightCycleManager::CalculateMoonAngle() const
{
	// Moon opposite to sun: add 180 degrees
	return CalculateSunAngle() + 180.0f;
}

float ADayNightCycleManager::GetTransitionFactor() const
{
	// Returns a value from 0 (night) to 1 (day) for smooth transitions
	if (TimeOfDay >= SunriseTime && TimeOfDay <= SunsetTime)
	{
		// During day
		if (TimeOfDay < SunriseTime + 1.0f)
		{
			// Sunrise transition
			return (TimeOfDay - SunriseTime) / 1.0f;
		}
		else if (TimeOfDay > SunsetTime - 1.0f)
		{
			// Sunset transition
			return (SunsetTime - TimeOfDay) / 1.0f;
		}
		else
		{
			// Full day
			return 1.0f;
		}
	}
	else
	{
		// Night
		return 0.0f;
	}
}

FText ADayNightCycleManager::GetRealWorldTimeText() const
{
	const FDateTime Now = FDateTime::Now();
	return FText::FromString(Now.ToString(TEXT("%H:%M:%S")));
}

void ADayNightCycleManager::UpdateClockDisplay(float DeltaTime)
{
	ClockAccumulatedTime += DeltaTime;
	if (ClockAccumulatedTime < ClockUpdateInterval)
	{
		return;
	}
	ClockAccumulatedTime = 0.0f;

	if (!ClockWidget && ClockWidgetClass)
	{
		ClockWidget = CreateWidget<UUserWidget>(GetWorld(), ClockWidgetClass);
		if (ClockWidget)
		{
			ClockWidget->AddToViewport();
			ClockTextBlock = Cast<UTextBlock>(ClockWidget->GetWidgetFromName(TEXT("ClockText")));
		}
	}

	const FText TimeText = GetFormattedTimeText();

	if (ClockTextBlock)
	{
		ClockTextBlock->SetText(TimeText);
	}
	else if (bShowDebugClock && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(ClockDebugMessageKey, ClockUpdateInterval * 1.1f, FColor::Cyan, TimeText.ToString());
	}
}

FText ADayNightCycleManager::GetFormattedTimeText() const
{
	const int32 TotalMinutes = FMath::RoundToInt(TimeOfDay * 60.0f);
	const int32 Hours = (TotalMinutes / 60) % 24;
	const int32 Minutes = TotalMinutes % 60;

	return FText::FromString(FString::Printf(TEXT("%02d:%02d"), Hours, Minutes));
}
