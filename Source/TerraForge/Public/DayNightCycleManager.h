// TerraForge - Procedural World Generator
// Day Night Cycle Manager

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "DayNightCycleManager.generated.h"

/**
 * Manages the day/night cycle including sun position and atmospheric lighting
 */
UCLASS(Blueprintable)
class TERRAFORGE_API ADayNightCycleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ADayNightCycleManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Get the current time of day (0-24 hours) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TerraForge|DayNight")
	float GetTimeOfDay() const { return TimeOfDay; }

	/** Set the time of day (0-24 hours) */
	UFUNCTION(BlueprintCallable, Category = "TerraForge|DayNight")
	void SetTimeOfDay(float NewTime);

	/** Get normalized time of day (0-1) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TerraForge|DayNight")
	float GetNormalizedTimeOfDay() const { return TimeOfDay / 24.0f; }

	/** Directional light component (sun) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDirectionalLightComponent* SunLight;

	/** Reference to directional light actor in the level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	ADirectionalLight* DirectionalLightActor;

	// Day/Night cycle parameters

	/** Current time of day in hours (0-24) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight", meta = (ClampMin = "0.0", ClampMax = "24.0"))
	float TimeOfDay = 12.0f;

	/** Speed of day/night cycle (hours per real second) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float CycleSpeed = 1.0f;

	/** Enable automatic time progression */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	bool bAutoProgress = true;

	/** Sunrise time in hours */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight", meta = (ClampMin = "0.0", ClampMax = "24.0"))
	float SunriseTime = 6.0f;

	/** Sunset time in hours */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight", meta = (ClampMin = "0.0", ClampMax = "24.0"))
	float SunsetTime = 18.0f;

	/** Sun intensity during day */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float DayIntensity = 10.0f;

	/** Sun intensity during night */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float NightIntensity = 0.5f;

	/** Sun color during day */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	FLinearColor DayColor = FLinearColor(1.0f, 0.95f, 0.8f, 1.0f);

	/** Sun color during sunrise/sunset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	FLinearColor SunsetColor = FLinearColor(1.0f, 0.5f, 0.2f, 1.0f);

	/** Sun color during night */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	FLinearColor NightColor = FLinearColor(0.5f, 0.6f, 0.8f, 1.0f);

private:
	/** Update sun position based on time of day */
	void UpdateSunPosition();

	/** Update sun intensity and color based on time of day */
	void UpdateSunProperties();

	/** Calculate sun angle based on time of day */
	float CalculateSunAngle() const;

	/** Get interpolation factor for sunrise/sunset transitions */
	float GetTransitionFactor() const;
};
