// TerraForge - Procedural World Generator
// Noise Generator Implementation

#include "NoiseGenerator.h"
#include "Math/UnrealMathUtility.h"

UNoiseGenerator::UNoiseGenerator()
	: Seed(0)
{
	InitializePermutation();
}

void UNoiseGenerator::InitializePermutation()
{
	// Standard Perlin noise permutation table
	static const int32 StandardPermutation[256] = {
		151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
		8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
		35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
		134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
		55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,
		18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
		250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
		189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
		172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
		228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
		107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
	};

	Permutation.SetNum(512);
	for (int32 i = 0; i < 256; i++)
	{
		Permutation[i] = StandardPermutation[i];
		Permutation[256 + i] = StandardPermutation[i];
	}
}

void UNoiseGenerator::SetSeed(int32 NewSeed)
{
	Seed = NewSeed;
	
	// Reinitialize permutation with seed
	FRandomStream RandomStream(Seed);
	TArray<int32> TempPerm;
	TempPerm.SetNum(256);
	
	for (int32 i = 0; i < 256; i++)
	{
		TempPerm[i] = i;
	}
	
	// Shuffle using Fisher-Yates algorithm
	for (int32 i = 255; i > 0; i--)
	{
		int32 j = RandomStream.RandRange(0, i);
		int32 Temp = TempPerm[i];
		TempPerm[i] = TempPerm[j];
		TempPerm[j] = Temp;
	}
	
	// Duplicate for wrapping
	Permutation.SetNum(512);
	for (int32 i = 0; i < 256; i++)
	{
		Permutation[i] = TempPerm[i];
		Permutation[256 + i] = TempPerm[i];
	}
}

float UNoiseGenerator::Fade(float T) const
{
	// 6t^5 - 15t^4 + 10t^3
	return T * T * T * (T * (T * 6.0f - 15.0f) + 10.0f);
}

float UNoiseGenerator::Lerp(float T, float A, float B) const
{
	return A + T * (B - A);
}

float UNoiseGenerator::Gradient(int32 Hash, float X, float Y) const
{
	int32 H = Hash & 3;
	float U = (H < 2) ? X : Y;
	float V = (H < 2) ? Y : X;
	return ((H & 1) ? -U : U) + ((H & 2) ? -V : V);
}

float UNoiseGenerator::Gradient3D(int32 Hash, float X, float Y, float Z) const
{
	int32 H = Hash & 15;
	float U = (H < 8) ? X : Y;
	float V = (H < 4) ? Y : ((H == 12 || H == 14) ? X : Z);
	return ((H & 1) ? -U : U) + ((H & 2) ? -V : V);
}

float UNoiseGenerator::GeneratePerlinNoise2D(float X, float Y, float Scale, int32 Octaves, float Persistence, float Lacunarity)
{
	if (Scale <= 0.0f) Scale = 0.0001f;
	
	float Total = 0.0f;
	float Frequency = 1.0f;
	float Amplitude = 1.0f;
	float MaxValue = 0.0f;
	
	for (int32 i = 0; i < Octaves; i++)
	{
		float SampleX = (X / Scale) * Frequency;
		float SampleY = (Y / Scale) * Frequency;
		
		// Calculate grid cell coordinates
		int32 Xi = FMath::FloorToInt(SampleX) & 255;
		int32 Yi = FMath::FloorToInt(SampleY) & 255;
		
		// Calculate relative position within grid cell
		float Xf = SampleX - FMath::FloorToFloat(SampleX);
		float Yf = SampleY - FMath::FloorToFloat(SampleY);
		
		// Fade curves
		float U = Fade(Xf);
		float V = Fade(Yf);
		
		// Hash coordinates of the 4 square corners
		int32 AA = Permutation[Permutation[Xi] + Yi];
		int32 AB = Permutation[Permutation[Xi] + Yi + 1];
		int32 BA = Permutation[Permutation[Xi + 1] + Yi];
		int32 BB = Permutation[Permutation[Xi + 1] + Yi + 1];
		
		// Blend results from the 4 corners
		float X1 = Lerp(U, Gradient(AA, Xf, Yf), Gradient(BA, Xf - 1.0f, Yf));
		float X2 = Lerp(U, Gradient(AB, Xf, Yf - 1.0f), Gradient(BB, Xf - 1.0f, Yf - 1.0f));
		float NoiseValue = Lerp(V, X1, X2);
		
		Total += NoiseValue * Amplitude;
		MaxValue += Amplitude;
		
		Amplitude *= Persistence;
		Frequency *= Lacunarity;
	}
	
	// Normalize to 0-1 range
	return (Total / MaxValue + 1.0f) * 0.5f;
}

float UNoiseGenerator::GeneratePerlinNoise3D(float X, float Y, float Z, float Scale)
{
	if (Scale <= 0.0f) Scale = 0.0001f;
	
	float SampleX = X / Scale;
	float SampleY = Y / Scale;
	float SampleZ = Z / Scale;
	
	// Calculate grid cell coordinates
	int32 Xi = FMath::FloorToInt(SampleX) & 255;
	int32 Yi = FMath::FloorToInt(SampleY) & 255;
	int32 Zi = FMath::FloorToInt(SampleZ) & 255;
	
	// Calculate relative position within grid cell
	float Xf = SampleX - FMath::FloorToFloat(SampleX);
	float Yf = SampleY - FMath::FloorToFloat(SampleY);
	float Zf = SampleZ - FMath::FloorToFloat(SampleZ);
	
	// Fade curves
	float U = Fade(Xf);
	float V = Fade(Yf);
	float W = Fade(Zf);
	
	// Hash coordinates of the 8 cube corners
	int32 A = Permutation[Xi] + Yi;
	int32 AA = Permutation[A] + Zi;
	int32 AB = Permutation[A + 1] + Zi;
	int32 B = Permutation[Xi + 1] + Yi;
	int32 BA = Permutation[B] + Zi;
	int32 BB = Permutation[B + 1] + Zi;
	
	// Blend results from the 8 corners
	float X1 = Lerp(U, Gradient3D(Permutation[AA], Xf, Yf, Zf), 
					   Gradient3D(Permutation[BA], Xf - 1.0f, Yf, Zf));
	float X2 = Lerp(U, Gradient3D(Permutation[AB], Xf, Yf - 1.0f, Zf),
					   Gradient3D(Permutation[BB], Xf - 1.0f, Yf - 1.0f, Zf));
	float Y1 = Lerp(V, X1, X2);
	
	X1 = Lerp(U, Gradient3D(Permutation[AA + 1], Xf, Yf, Zf - 1.0f),
				 Gradient3D(Permutation[BA + 1], Xf - 1.0f, Yf, Zf - 1.0f));
	X2 = Lerp(U, Gradient3D(Permutation[AB + 1], Xf, Yf - 1.0f, Zf - 1.0f),
				 Gradient3D(Permutation[BB + 1], Xf - 1.0f, Yf - 1.0f, Zf - 1.0f));
	float Y2 = Lerp(V, X1, X2);
	
	float NoiseValue = Lerp(W, Y1, Y2);
	
	// Normalize to 0-1 range
	return (NoiseValue + 1.0f) * 0.5f;
}

float UNoiseGenerator::SimplexGradient(int32 Hash, float X, float Y) const
{
	int32 H = Hash & 7;
	float U = (H < 4) ? X : Y;
	float V = (H < 4) ? Y : X;
	return ((H & 1) ? -U : U) + ((H & 2) ? -2.0f * V : 2.0f * V);
}

float UNoiseGenerator::GenerateSimplexNoise2D(float X, float Y, float Scale)
{
	if (Scale <= 0.0f) Scale = 0.0001f;
	
	float SampleX = X / Scale;
	float SampleY = Y / Scale;
	
	// Skew factors for 2D
	const float F2 = 0.366025403f; // (sqrt(3) - 1) / 2
	const float G2 = 0.211324865f; // (3 - sqrt(3)) / 6
	
	// Skew the input space
	float S = (SampleX + SampleY) * F2;
	int32 I = FMath::FloorToInt(SampleX + S);
	int32 J = FMath::FloorToInt(SampleY + S);
	
	float T = (I + J) * G2;
	float X0 = SampleX - (I - T);
	float Y0 = SampleY - (J - T);
	
	// Determine which simplex we're in
	int32 I1, J1;
	if (X0 > Y0)
	{
		I1 = 1;
		J1 = 0;
	}
	else
	{
		I1 = 0;
		J1 = 1;
	}
	
	// Offsets for middle and last corners
	float X1 = X0 - I1 + G2;
	float Y1 = Y0 - J1 + G2;
	float X2 = X0 - 1.0f + 2.0f * G2;
	float Y2 = Y0 - 1.0f + 2.0f * G2;
	
	// Work with wrapped indices
	int32 Ii = I & 255;
	int32 Jj = J & 255;
	
	// Calculate contribution from three corners
	float N0, N1, N2;
	
	float T0 = 0.5f - X0 * X0 - Y0 * Y0;
	if (T0 < 0.0f)
	{
		N0 = 0.0f;
	}
	else
	{
		T0 *= T0;
		N0 = T0 * T0 * SimplexGradient(Permutation[Ii + Permutation[Jj]], X0, Y0);
	}
	
	float T1 = 0.5f - X1 * X1 - Y1 * Y1;
	if (T1 < 0.0f)
	{
		N1 = 0.0f;
	}
	else
	{
		T1 *= T1;
		N1 = T1 * T1 * SimplexGradient(Permutation[Ii + I1 + Permutation[Jj + J1]], X1, Y1);
	}
	
	float T2 = 0.5f - X2 * X2 - Y2 * Y2;
	if (T2 < 0.0f)
	{
		N2 = 0.0f;
	}
	else
	{
		T2 *= T2;
		N2 = T2 * T2 * SimplexGradient(Permutation[Ii + 1 + Permutation[Jj + 1]], X2, Y2);
	}
	
	// Sum contributions and scale to [-1, 1]
	return 70.0f * (N0 + N1 + N2);
}
