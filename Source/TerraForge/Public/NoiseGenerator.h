// TerraForge - Procedural World Generator
// Noise Generator for terrain generation using Perlin and Simplex noise

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NoiseGenerator.generated.h"

/**
 * Noise Generator class for procedural terrain generation
 * Implements Perlin and Simplex noise algorithms
 */
UCLASS(Blueprintable, BlueprintType)
class TERRAFORGE_API UNoiseGenerator : public UObject
{
	GENERATED_BODY()

public:
	UNoiseGenerator();

	/**
	 * Generate 2D Perlin noise value at given coordinates
	 * @param X - X coordinate
	 * @param Y - Y coordinate
	 * @param Scale - Scale of the noise
	 * @param Octaves - Number of noise octaves for detail
	 * @param Persistence - Amplitude multiplier for each octave
	 * @param Lacunarity - Frequency multiplier for each octave
	 * @return Noise value between 0 and 1
	 */
	UFUNCTION(BlueprintCallable, Category = "TerraForge|Noise")
	float GeneratePerlinNoise2D(float X, float Y, float Scale = 1.0f, int32 Octaves = 4, float Persistence = 0.5f, float Lacunarity = 2.0f);

	/**
	 * Generate 3D Perlin noise value at given coordinates
	 * @param X - X coordinate
	 * @param Y - Y coordinate
	 * @param Z - Z coordinate
	 * @param Scale - Scale of the noise
	 * @return Noise value between 0 and 1
	 */
	UFUNCTION(BlueprintCallable, Category = "TerraForge|Noise")
	float GeneratePerlinNoise3D(float X, float Y, float Z, float Scale = 1.0f);

	/**
	 * Generate Simplex noise value at given coordinates
	 * @param X - X coordinate
	 * @param Y - Y coordinate
	 * @param Scale - Scale of the noise
	 * @return Noise value between -1 and 1
	 */
	UFUNCTION(BlueprintCallable, Category = "TerraForge|Noise")
	float GenerateSimplexNoise2D(float X, float Y, float Scale = 1.0f);

	/**
	 * Set the random seed for noise generation
	 * @param NewSeed - Seed value for reproducible noise
	 */
	UFUNCTION(BlueprintCallable, Category = "TerraForge|Noise")
	void SetSeed(int32 NewSeed);

private:
	// Permutation table for Perlin noise
	TArray<int32> Permutation;
	
	// Random seed
	int32 Seed;

	// Helper functions for Perlin noise
	float Fade(float T) const;
	float Lerp(float T, float A, float B) const;
	float Gradient(int32 Hash, float X, float Y) const;
	float Gradient3D(int32 Hash, float X, float Y, float Z) const;
	
	// Initialize permutation table
	void InitializePermutation();
	
	// Simplex noise helpers
	float SimplexGradient(int32 Hash, float X, float Y) const;
};
