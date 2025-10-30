// TerraForge - Procedural World Generator
// Day Night Cycle Manager Implementation

#include "DayNightCycleManager.h"
#include "Kismet/GameplayStatics.h"

ADayNightCycleManager::ADayNightCycleManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create default sun light component
	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
	RootComponent = SunLight;
	
	SunLight->SetIntensity(DayIntensity);
	SunLight->SetLightColor(DayColor);
	SunLight->SetCastShadows(true);
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
	
	// Initialize sun position and properties
	UpdateSunPosition();
	UpdateSunProperties();
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
}

void ADayNightCycleManager::UpdateSunProperties()
{
	if (!SunLight)
	{
		return;
	}

	float TransitionFactor = GetTransitionFactor();
	
	// Calculate intensity based on sun position
	float CurrentIntensity;
	if (TimeOfDay >= SunriseTime && TimeOfDay <= SunsetTime)
	{
		// Day time
		CurrentIntensity = DayIntensity;
		
		// Reduce intensity during sunrise/sunset
		if (TimeOfDay < SunriseTime + 1.0f)
		{
			// Sunrise transition
			float Factor = (TimeOfDay - SunriseTime) / 1.0f;
			CurrentIntensity = FMath::Lerp(NightIntensity, DayIntensity, Factor);
		}
		else if (TimeOfDay > SunsetTime - 1.0f)
		{
			// Sunset transition
			float Factor = (SunsetTime - TimeOfDay) / 1.0f;
			CurrentIntensity = FMath::Lerp(NightIntensity, DayIntensity, Factor);
		}
	}
	else
	{
		// Night time
		CurrentIntensity = NightIntensity;
	}
	
	// Calculate color based on time of day
	FLinearColor CurrentColor;
	if (TimeOfDay >= SunriseTime - 0.5f && TimeOfDay <= SunriseTime + 0.5f)
	{
		// Sunrise colors
		float Factor = (TimeOfDay - (SunriseTime - 0.5f)) / 1.0f;
		CurrentColor = FMath::Lerp(NightColor, SunsetColor, Factor);
	}
	else if (TimeOfDay >= SunriseTime + 0.5f && TimeOfDay <= SunsetTime - 0.5f)
	{
		// Day colors
		if (TimeOfDay < SunriseTime + 1.5f)
		{
			float Factor = (TimeOfDay - (SunriseTime + 0.5f)) / 1.0f;
			CurrentColor = FMath::Lerp(SunsetColor, DayColor, Factor);
		}
		else if (TimeOfDay > SunsetTime - 1.5f)
		{
			float Factor = (SunsetTime - 0.5f - TimeOfDay) / 1.0f;
			CurrentColor = FMath::Lerp(SunsetColor, DayColor, Factor);
		}
		else
		{
			CurrentColor = DayColor;
		}
	}
	else if (TimeOfDay >= SunsetTime - 0.5f && TimeOfDay <= SunsetTime + 0.5f)
	{
		// Sunset colors
		float Factor = (TimeOfDay - (SunsetTime - 0.5f)) / 1.0f;
		CurrentColor = FMath::Lerp(SunsetColor, NightColor, Factor);
	}
	else
	{
		// Night colors
		CurrentColor = NightColor;
	}
	
	// Apply properties to light
	SunLight->SetIntensity(CurrentIntensity);
	SunLight->SetLightColor(CurrentColor);
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
