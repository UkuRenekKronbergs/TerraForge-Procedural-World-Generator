// TerraForge - Procedural World Generator
// Procedural Terrain Generator using noise functions

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "NoiseGenerator.h"
#include "ProceduralTerrainActor.generated.h"

/**
 * Actor that generates procedural terrain meshes using noise functions
 */
UCLASS(Blueprintable)
class TERRAFORGE_API AProceduralTerrainActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AProceduralTerrainActor();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Generate the terrain mesh */
	UFUNCTION(BlueprintCallable, Category = "TerraForge|Terrain")
	void GenerateTerrain();

	/** Clear the terrain mesh */
	UFUNCTION(BlueprintCallable, Category = "TerraForge|Terrain")
	void ClearTerrain();

	/** Procedural mesh component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProceduralMeshComponent* ProceduralMesh;

	/** Noise generator for terrain */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TerraForge|Terrain")
	UNoiseGenerator* NoiseGenerator;

	// Terrain generation parameters
	
	/** Width of the terrain in grid units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain", meta = (ClampMin = "1", ClampMax = "500"))
	int32 TerrainWidth = 100;

	/** Height of the terrain in grid units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain", meta = (ClampMin = "1", ClampMax = "500"))
	int32 TerrainHeight = 100;

	/** Size of each grid square in Unreal units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain", meta = (ClampMin = "1.0", ClampMax = "1000.0"))
	float GridSize = 100.0f;

	/** Maximum height of terrain features */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain", meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	float MaxHeight = 2000.0f;

	/** Noise scale for terrain generation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain", meta = (ClampMin = "0.1", ClampMax = "1000.0"))
	float NoiseScale = 100.0f;

	/** Number of octaves for noise detail */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain", meta = (ClampMin = "1", ClampMax = "8"))
	int32 Octaves = 4;

	/** Persistence of noise (amplitude multiplier per octave) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Persistence = 0.5f;

	/** Lacunarity of noise (frequency multiplier per octave) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain", meta = (ClampMin = "1.0", ClampMax = "4.0"))
	float Lacunarity = 2.0f;

	/** Random seed for terrain generation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain")
	int32 RandomSeed = 12345;

	/** Auto-generate terrain on construction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain")
	bool bAutoGenerate = true;

	/** Use Simplex noise instead of Perlin noise */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerraForge|Terrain")
	bool bUseSimplexNoise = false;

private:
	/** Generate vertices for the terrain mesh */
	void GenerateVertices(TArray<FVector>& Vertices, TArray<FVector>& Normals, TArray<FVector2D>& UVs);

	/** Generate triangles for the terrain mesh */
	void GenerateTriangles(TArray<int32>& Triangles);

	/** Calculate normals for smooth terrain shading */
	void CalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& Normals);
};
