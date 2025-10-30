// TerraForge - Procedural World Generator
// Procedural Terrain Generator Implementation

#include "ProceduralTerrainActor.h"
#include "KismetProceduralMeshLibrary.h"

AProceduralTerrainActor::AProceduralTerrainActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create procedural mesh component
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = ProceduralMesh;
	ProceduralMesh->bUseAsyncCooking = true;

	// Create noise generator
	NoiseGenerator = CreateDefaultSubobject<UNoiseGenerator>(TEXT("NoiseGenerator"));
}

void AProceduralTerrainActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (bAutoGenerate)
	{
		GenerateTerrain();
	}
}

void AProceduralTerrainActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (bAutoGenerate)
	{
		GenerateTerrain();
	}
}

void AProceduralTerrainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProceduralTerrainActor::GenerateTerrain()
{
	if (!NoiseGenerator || !ProceduralMesh)
	{
		return;
	}

	// Set the seed for reproducible generation
	NoiseGenerator->SetSeed(RandomSeed);

	// Clear existing mesh
	ProceduralMesh->ClearAllMeshSections();

	// Generate mesh data
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Generate vertices and UVs
	GenerateVertices(Vertices, Normals, UVs);

	// Generate triangles
	GenerateTriangles(Triangles);

	// Calculate proper normals
	CalculateNormals(Vertices, Triangles, Normals);

	// Generate vertex colors based on height
	VertexColors.SetNum(Vertices.Num());
	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		float HeightRatio = Vertices[i].Z / MaxHeight;
		uint8 ColorValue = FMath::Clamp(static_cast<uint8>(HeightRatio * 255), 0, 255);
		VertexColors[i] = FColor(ColorValue, ColorValue, ColorValue, 255);
	}

	// Create the mesh section
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);

	// Enable collision
	ProceduralMesh->ContainsPhysicsTriMeshData(true);
}

void AProceduralTerrainActor::ClearTerrain()
{
	if (ProceduralMesh)
	{
		ProceduralMesh->ClearAllMeshSections();
	}
}

void AProceduralTerrainActor::GenerateVertices(TArray<FVector>& Vertices, TArray<FVector>& Normals, TArray<FVector2D>& UVs)
{
	Vertices.Empty();
	Normals.Empty();
	UVs.Empty();

	const int32 NumVertices = (TerrainWidth + 1) * (TerrainHeight + 1);
	Vertices.Reserve(NumVertices);
	Normals.Reserve(NumVertices);
	UVs.Reserve(NumVertices);

	for (int32 Y = 0; Y <= TerrainHeight; Y++)
	{
		for (int32 X = 0; X <= TerrainWidth; X++)
		{
			// Calculate world position
			float WorldX = X * GridSize;
			float WorldY = Y * GridSize;

			// Generate height using noise
			float Height = 0.0f;
			if (bUseSimplexNoise)
			{
				Height = NoiseGenerator->GenerateSimplexNoise2D(WorldX, WorldY, NoiseScale);
				// Simplex noise returns -1 to 1, convert to 0 to 1
				Height = (Height + 1.0f) * 0.5f;
			}
			else
			{
				Height = NoiseGenerator->GeneratePerlinNoise2D(WorldX, WorldY, NoiseScale, Octaves, Persistence, Lacunarity);
			}

			// Apply height multiplier
			float WorldZ = Height * MaxHeight;

			// Add vertex
			Vertices.Add(FVector(WorldX, WorldY, WorldZ));

			// Add temporary normal (will be recalculated)
			Normals.Add(FVector::UpVector);

			// Add UV coordinates
			float U = static_cast<float>(X) / TerrainWidth;
			float V = static_cast<float>(Y) / TerrainHeight;
			UVs.Add(FVector2D(U, V));
		}
	}
}

void AProceduralTerrainActor::GenerateTriangles(TArray<int32>& Triangles)
{
	Triangles.Empty();

	const int32 NumQuads = TerrainWidth * TerrainHeight;
	Triangles.Reserve(NumQuads * 6); // 2 triangles per quad, 3 indices per triangle

	for (int32 Y = 0; Y < TerrainHeight; Y++)
	{
		for (int32 X = 0; X < TerrainWidth; X++)
		{
			// Calculate vertex indices for this quad
			int32 BottomLeft = Y * (TerrainWidth + 1) + X;
			int32 BottomRight = BottomLeft + 1;
			int32 TopLeft = (Y + 1) * (TerrainWidth + 1) + X;
			int32 TopRight = TopLeft + 1;

			// First triangle (bottom-left, top-left, top-right)
			Triangles.Add(BottomLeft);
			Triangles.Add(TopLeft);
			Triangles.Add(TopRight);

			// Second triangle (bottom-left, top-right, bottom-right)
			Triangles.Add(BottomLeft);
			Triangles.Add(TopRight);
			Triangles.Add(BottomRight);
		}
	}
}

void AProceduralTerrainActor::CalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& Normals)
{
	// Reset normals
	Normals.SetNum(Vertices.Num());
	for (int32 i = 0; i < Normals.Num(); i++)
	{
		Normals[i] = FVector::ZeroVector;
	}

	// Accumulate face normals
	for (int32 i = 0; i < Triangles.Num(); i += 3)
	{
		int32 Index0 = Triangles[i];
		int32 Index1 = Triangles[i + 1];
		int32 Index2 = Triangles[i + 2];

		FVector V0 = Vertices[Index0];
		FVector V1 = Vertices[Index1];
		FVector V2 = Vertices[Index2];

		// Calculate face normal
		FVector Edge1 = V1 - V0;
		FVector Edge2 = V2 - V0;
		FVector FaceNormal = FVector::CrossProduct(Edge1, Edge2).GetSafeNormal();

		// Accumulate to vertex normals
		Normals[Index0] += FaceNormal;
		Normals[Index1] += FaceNormal;
		Normals[Index2] += FaceNormal;
	}

	// Normalize accumulated normals
	for (int32 i = 0; i < Normals.Num(); i++)
	{
		Normals[i] = Normals[i].GetSafeNormal();
	}
}
