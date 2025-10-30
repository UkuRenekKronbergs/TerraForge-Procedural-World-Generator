// TerraForge - Procedural World Generator
// Dynamic Water Actor with custom shader support

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "DynamicWaterActor.generated.h"

/**
 * Actor that creates a dynamic water plane with custom shader effects
 */
UCLASS(Blueprintable)
class TERRAFORGE_API ADynamicWaterActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADynamicWaterActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Generate the water mesh */
	UFUNCTION(BlueprintCallable, Category = "TerraForge|Water")
	void GenerateWaterMesh();

	/** Procedural mesh component for water */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProceduralMeshComponent* WaterMesh;

	// Water parameters

	/** Width of water plane */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Water", meta = (ClampMin = "100.0", ClampMax = "100000.0"))
	float WaterWidth = 10000.0f;

	/** Length of water plane */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Water", meta = (ClampMin = "100.0", ClampMax = "100000.0"))
	float WaterLength = 10000.0f;

	/** Water plane height (Z position) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Water")
	float WaterLevel = 0.0f;

	/** Number of subdivisions for wave detail */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Water", meta = (ClampMin = "1", ClampMax = "100"))
	int32 Subdivisions = 20;

	/** Water material to apply */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Water")
	UMaterialInterface* WaterMaterial;

	/** Enable dynamic wave simulation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Water")
	bool bEnableWaves = true;

	/** Wave animation speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Water", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float WaveSpeed = 1.0f;

	/** Wave height multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Water", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float WaveHeight = 50.0f;

private:
	/** Generate vertices for water mesh */
	void GenerateWaterVertices(TArray<FVector>& Vertices, TArray<FVector>& Normals, TArray<FVector2D>& UVs);

	/** Generate triangles for water mesh */
	void GenerateWaterTriangles(TArray<int32>& Triangles);

	/** Current time for wave animation */
	float CurrentTime;
};
