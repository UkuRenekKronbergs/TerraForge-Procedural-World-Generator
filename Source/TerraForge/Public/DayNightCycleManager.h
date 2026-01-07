// TerraForge - Procedural World Generator
// Day Night Cycle Manager

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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

	/** Get the current real-world clock time as text (HH:MM:SS) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TerraForge|DayNight")
	FText GetRealWorldTimeText() const;

	/** Directional light component (sun) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDirectionalLightComponent* SunLight;

	/** Directional light component (moon) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDirectionalLightComponent* MoonLight;

	/** Reference to directional light actor in the level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	ADirectionalLight* DirectionalLightActor;

	/** Reference to a directional light actor to drive the moon (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	ADirectionalLight* MoonDirectionalLightActor;

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

	/** Moon intensity at full night */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight", meta = (ClampMin = "0.0", ClampMax = "50.0"))
	float MoonIntensity = 1.5f;

	/** Sun color during day */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	FLinearColor DayColor = FLinearColor(1.0f, 0.95f, 0.8f, 1.0f);

	/** Sun color during sunrise/sunset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	FLinearColor SunsetColor = FLinearColor(1.0f, 0.5f, 0.2f, 1.0f);

	/** Sun color during night */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	FLinearColor NightColor = FLinearColor(0.5f, 0.6f, 0.8f, 1.0f);

	/** Moon light color */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	FLinearColor MoonColor = FLinearColor(0.65f, 0.73f, 0.9f, 1.0f);

	/** Enable moon lighting during night */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight")
	bool bEnableMoonLight = true;

	/** Length of dawn/dusk blend in hours to avoid harsh jumps */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|DayNight", meta = (ClampMin = "0.1", ClampMax = "4.0"))
	float TwilightLength = 1.5f;

	/** Optional UI widget blueprint (should contain a TextBlock named ClockText) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|UI")
	TSubclassOf<UUserWidget> ClockWidgetClass;

	/** Show a simple debug clock using on-screen messages when no widget is provided */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|UI")
	bool bShowDebugClock = true;

	/** Update interval (seconds) for the clock UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|UI", meta = (ClampMin = "0.05", ClampMax = "5.0"))
	float ClockUpdateInterval = 0.25f;

private:
	/** Update sun position based on time of day */
	void UpdateSunPosition();

	/** Update sun intensity and color based on time of day */
	void UpdateSunProperties();

	/** Calculate sun angle based on time of day */
	float CalculateSunAngle() const;

	/** Calculate moon angle (opposite the sun) */
	float CalculateMoonAngle() const;

	/** Get interpolation factor for sunrise/sunset transitions */
	float GetTransitionFactor() const;

	/** Update or create the on-screen clock widget */
	void UpdateClockDisplay(float DeltaTime);

	/** Format time of day as HH:MM */
	FText GetFormattedTimeText() const;

	/** Cached clock widget instance */
	UPROPERTY(Transient)
	UUserWidget* ClockWidget = nullptr;

	/** Cached pointer to the text block inside the widget */
	UPROPERTY(Transient)
	UTextBlock* ClockTextBlock = nullptr;

	/** Accumulator for controlling clock refresh cadence */
	float ClockAccumulatedTime = 0.0f;

	/** Key used for debug on-screen messages to avoid spam */
	int32 ClockDebugMessageKey = 12345;
};
