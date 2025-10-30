// TerraForge - Procedural World Generator
// Free Camera Pawn Implementation

#include "FreeCameraPawn.h"
#include "Components/InputComponent.h"

AFreeCameraPawn::AFreeCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = Camera;

	// Create movement component
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = BaseMovementSpeed;
	MovementComponent->Acceleration = 4000.0f;
	MovementComponent->Deceleration = 8000.0f;

	// Initialize state
	bIsSprinting = false;
	bIsMovingSlow = false;
	CurrentMovementInput = FVector::ZeroVector;
	CurrentRotationInput = FRotator::ZeroRotator;
	TargetRotation = FRotator::ZeroRotator;

	// Set this pawn to be controlled by the player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AFreeCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize target rotation to current rotation
	TargetRotation = GetActorRotation();
	
	// Set initial movement speed
	UpdateMovementSpeed();
}

void AFreeCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Apply smooth rotation if enabled
	if (bSmoothRotation)
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSmoothSpeed);
		SetActorRotation(NewRotation);
	}

	// Apply movement
	if (!CurrentMovementInput.IsNearlyZero())
	{
		FVector MovementDirection = GetActorRotation().RotateVector(CurrentMovementInput);
		AddMovementInput(MovementDirection);
	}
}

void AFreeCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AFreeCameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFreeCameraPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &AFreeCameraPawn::MoveUp);

	// Look bindings
	PlayerInputComponent->BindAxis("LookUp", this, &AFreeCameraPawn::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AFreeCameraPawn::LookRight);

	// Sprint bindings
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFreeCameraPawn::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFreeCameraPawn::StopSprint);

	// Slow move bindings
	PlayerInputComponent->BindAction("SlowMove", IE_Pressed, this, &AFreeCameraPawn::StartSlowMove);
	PlayerInputComponent->BindAction("SlowMove", IE_Released, this, &AFreeCameraPawn::StopSlowMove);
}

void AFreeCameraPawn::MoveForward(float Value)
{
	CurrentMovementInput.X = Value;
}

void AFreeCameraPawn::MoveRight(float Value)
{
	CurrentMovementInput.Y = Value;
}

void AFreeCameraPawn::MoveUp(float Value)
{
	CurrentMovementInput.Z = Value;
}

void AFreeCameraPawn::LookUp(float Value)
{
	if (Value != 0.0f)
	{
		float PitchChange = Value * LookSensitivity;
		
		if (bSmoothRotation)
		{
			TargetRotation.Pitch = FMath::Clamp(TargetRotation.Pitch + PitchChange, -89.0f, 89.0f);
		}
		else
		{
			AddControllerPitchInput(PitchChange);
		}
	}
}

void AFreeCameraPawn::LookRight(float Value)
{
	if (Value != 0.0f)
	{
		float YawChange = Value * LookSensitivity;
		
		if (bSmoothRotation)
		{
			TargetRotation.Yaw += YawChange;
		}
		else
		{
			AddControllerYawInput(YawChange);
		}
	}
}

void AFreeCameraPawn::StartSprint()
{
	bIsSprinting = true;
	UpdateMovementSpeed();
}

void AFreeCameraPawn::StopSprint()
{
	bIsSprinting = false;
	UpdateMovementSpeed();
}

void AFreeCameraPawn::StartSlowMove()
{
	bIsMovingSlow = true;
	UpdateMovementSpeed();
}

void AFreeCameraPawn::StopSlowMove()
{
	bIsMovingSlow = false;
	UpdateMovementSpeed();
}

void AFreeCameraPawn::UpdateMovementSpeed()
{
	if (!MovementComponent)
	{
		return;
	}

	float NewSpeed = BaseMovementSpeed;

	if (bIsSprinting)
	{
		NewSpeed *= SprintMultiplier;
	}
	else if (bIsMovingSlow)
	{
		NewSpeed *= SlowMultiplier;
	}

	MovementComponent->MaxSpeed = NewSpeed;
}
