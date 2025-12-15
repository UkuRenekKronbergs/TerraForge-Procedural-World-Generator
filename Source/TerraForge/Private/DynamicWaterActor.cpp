// TerraForge - Procedural World Generator
// Dynamic Water Actor Implementation

#include "DynamicWaterActor.h"
#include "Materials/MaterialInstanceDynamic.h"

ADynamicWaterActor::ADynamicWaterActor()
{
	PrimaryActorTick.bCanEverTick = true;

	WaterMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("WaterMesh"));
	RootComponent = WaterMesh;
	WaterMesh->bUseAsyncCooking = true;
	WaterMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WaterMesh->SetCollisionResponseToAllChannels(ECR_Block);

	CurrentTime = 0.0f;
}

void ADynamicWaterActor::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateWaterMesh();
}

void ADynamicWaterActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GenerateWaterMesh();
}

void ADynamicWaterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bEnableWaves)
	{
		CurrentTime += DeltaTime * WaveSpeed;
		
		// Update material parameters for wave animation using a single shared instance
		if (DynamicMaterialInstance)
		{
			DynamicMaterialInstance->SetScalarParameterValue(FName("Time"), CurrentTime);
			DynamicMaterialInstance->SetScalarParameterValue(FName("WaveHeight"), WaveHeight);
			DynamicMaterialInstance->SetScalarParameterValue(FName("WaveSpeed"), WaveSpeed);
		}
	}
}

void ADynamicWaterActor::GenerateWaterMesh()
{
	if (!WaterMesh)
	{
		return;
	}

	// Clear existing mesh
	WaterMesh->ClearAllMeshSections();

	// Generate mesh data
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Generate vertices
	GenerateWaterVertices(Vertices, Normals, UVs);

	// Generate triangles
	GenerateWaterTriangles(Triangles);

	// Set vertex colors to white
	VertexColors.SetNum(Vertices.Num());
	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		VertexColors[i] = FColor::White;
	}

	// Create the mesh section
	WaterMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, false);

	// Apply material if set and keep a dynamic instance for runtime updates
	if (WaterMaterial)
	{
		WaterMesh->SetMaterial(0, WaterMaterial);
		DynamicMaterialInstance = WaterMesh->CreateDynamicMaterialInstance(0, WaterMaterial);
		if (DynamicMaterialInstance)
		{
			DynamicMaterialInstance->SetScalarParameterValue(FName("WaveHeight"), WaveHeight);
			DynamicMaterialInstance->SetScalarParameterValue(FName("WaveSpeed"), WaveSpeed);
		}
	}
}

void ADynamicWaterActor::GenerateWaterVertices(TArray<FVector>& Vertices, TArray<FVector>& Normals, TArray<FVector2D>& UVs)
{
	Vertices.Empty();
	Normals.Empty();
	UVs.Empty();

	const int32 VertsPerSide = Subdivisions + 1;
	const int32 NumVertices = VertsPerSide * VertsPerSide;
	
	Vertices.Reserve(NumVertices);
	Normals.Reserve(NumVertices);
	UVs.Reserve(NumVertices);

	const float StepX = WaterWidth / Subdivisions;
	const float StepY = WaterLength / Subdivisions;
	const float OffsetX = -WaterWidth * 0.5f;
	const float OffsetY = -WaterLength * 0.5f;

	for (int32 Y = 0; Y <= Subdivisions; Y++)
	{
		for (int32 X = 0; X <= Subdivisions; X++)
		{
			float WorldX = OffsetX + (X * StepX);
			float WorldY = OffsetY + (Y * StepY);
			float WorldZ = WaterLevel;

			// Add vertex
			Vertices.Add(FVector(WorldX, WorldY, WorldZ));

			// Add normal (pointing up)
			Normals.Add(FVector::UpVector);

			// Add UV coordinates
			float U = static_cast<float>(X) / Subdivisions;
			float V = static_cast<float>(Y) / Subdivisions;
			UVs.Add(FVector2D(U, V));
		}
	}
}

void ADynamicWaterActor::GenerateWaterTriangles(TArray<int32>& Triangles)
{
	Triangles.Empty();

	const int32 NumQuads = Subdivisions * Subdivisions;
	Triangles.Reserve(NumQuads * 6);

	const int32 VertsPerSide = Subdivisions + 1;

	for (int32 Y = 0; Y < Subdivisions; Y++)
	{
		for (int32 X = 0; X < Subdivisions; X++)
		{
			int32 BottomLeft = Y * VertsPerSide + X;
			int32 BottomRight = BottomLeft + 1;
			int32 TopLeft = (Y + 1) * VertsPerSide + X;
			int32 TopRight = TopLeft + 1;

			// First triangle
			Triangles.Add(BottomLeft);
			Triangles.Add(TopLeft);
			Triangles.Add(TopRight);

			// Second triangle
			Triangles.Add(BottomLeft);
			Triangles.Add(TopRight);
			Triangles.Add(BottomRight);
		}
	}
}
