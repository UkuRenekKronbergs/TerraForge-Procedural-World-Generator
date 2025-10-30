// TerraForge - Procedural World Generator
// Free Camera Pawn for world exploration

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "FreeCameraPawn.generated.h"

/**
 * Free-flying camera pawn for exploring the procedural world
 */
UCLASS(Blueprintable)
class TERRAFORGE_API AFreeCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AFreeCameraPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

	/** Movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* MovementComponent;

	// Camera movement parameters

	/** Base movement speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Camera", meta = (ClampMin = "100.0", ClampMax = "10000.0"))
	float BaseMovementSpeed = 1000.0f;

	/** Speed multiplier when sprinting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Camera", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	float SprintMultiplier = 3.0f;

	/** Speed multiplier when moving slowly */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Camera", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float SlowMultiplier = 0.3f;

	/** Mouse sensitivity for looking around */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Camera", meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float LookSensitivity = 1.0f;

	/** Smooth camera rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Camera")
	bool bSmoothRotation = true;

	/** Rotation interpolation speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Camera", meta = (ClampMin = "1.0", ClampMax = "20.0"))
	float RotationSmoothSpeed = 10.0f;

private:
	// Input handling functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	void StartSprint();
	void StopSprint();
	void StartSlowMove();
	void StopSlowMove();

	// Current movement direction
	FVector CurrentMovementInput;
	
	// Current rotation input
	FRotator CurrentRotationInput;
	FRotator TargetRotation;
	
	// Sprint state
	bool bIsSprinting;
	bool bIsMovingSlow;

	// Update movement speed based on modifiers
	void UpdateMovementSpeed();
};
