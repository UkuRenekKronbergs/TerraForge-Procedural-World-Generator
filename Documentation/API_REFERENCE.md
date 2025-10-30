# TerraForge API Reference

## Core Classes

### NoiseGenerator

Provides noise generation algorithms for procedural content.

#### Public Methods

##### `float GeneratePerlinNoise2D(float X, float Y, float Scale, int32 Octaves, float Persistence, float Lacunarity)`

Generates 2D Perlin noise with multiple octaves for fractal detail.

**Parameters**:
- `X`: X coordinate in world space
- `Y`: Y coordinate in world space  
- `Scale`: Scale factor for noise frequency (default: 1.0)
- `Octaves`: Number of noise layers to combine (default: 4)
- `Persistence`: Amplitude reduction per octave (default: 0.5)
- `Lacunarity`: Frequency increase per octave (default: 2.0)

**Returns**: Float value between 0 and 1

**Example**:
```cpp
float Height = NoiseGenerator->GeneratePerlinNoise2D(
    100.0f,    // X
    200.0f,    // Y
    50.0f,     // Scale
    6,         // Octaves
    0.5f,      // Persistence
    2.0f       // Lacunarity
);
```

##### `float GeneratePerlinNoise3D(float X, float Y, float Z, float Scale)`

Generates 3D Perlin noise for volumetric effects.

**Parameters**:
- `X, Y, Z`: 3D coordinates in world space
- `Scale`: Scale factor for noise frequency

**Returns**: Float value between 0 and 1

##### `float GenerateSimplexNoise2D(float X, float Y, float Scale)`

Generates 2D Simplex noise (faster and more organic than Perlin).

**Parameters**:
- `X, Y`: 2D coordinates in world space
- `Scale`: Scale factor for noise frequency

**Returns**: Float value between -1 and 1

##### `void SetSeed(int32 NewSeed)`

Sets the random seed for reproducible noise generation.

**Parameters**:
- `NewSeed`: Integer seed value

---

### ProceduralTerrainActor

Actor that generates procedural terrain meshes using noise functions.

#### Public Properties

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `TerrainWidth` | int32 | 100 | Width of terrain in grid units |
| `TerrainHeight` | int32 | 100 | Height of terrain in grid units |
| `GridSize` | float | 100.0 | Size of each grid square in cm |
| `MaxHeight` | float | 2000.0 | Maximum terrain elevation in cm |
| `NoiseScale` | float | 100.0 | Scale of noise patterns |
| `Octaves` | int32 | 4 | Number of detail layers (1-8) |
| `Persistence` | float | 0.5 | Amplitude reduction per octave (0-1) |
| `Lacunarity` | float | 2.0 | Frequency increase per octave (1-4) |
| `RandomSeed` | int32 | 12345 | Seed for reproducible generation |
| `bAutoGenerate` | bool | true | Auto-generate on construction |
| `bUseSimplexNoise` | bool | false | Use Simplex instead of Perlin |

#### Public Methods

##### `void GenerateTerrain()`

Generates or regenerates the terrain mesh based on current parameters.

**Example**:
```cpp
AProceduralTerrainActor* Terrain = GetWorld()->SpawnActor<AProceduralTerrainActor>();
Terrain->TerrainWidth = 200;
Terrain->MaxHeight = 5000.0f;
Terrain->RandomSeed = FMath::Rand();
Terrain->GenerateTerrain();
```

##### `void ClearTerrain()`

Clears the terrain mesh.

---

### DayNightCycleManager

Manages the day/night cycle and sun lighting.

#### Public Properties

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `TimeOfDay` | float | 12.0 | Current time in hours (0-24) |
| `CycleSpeed` | float | 1.0 | Hours per real second |
| `bAutoProgress` | bool | true | Enable automatic time progression |
| `SunriseTime` | float | 6.0 | Sunrise hour (0-24) |
| `SunsetTime` | float | 18.0 | Sunset hour (0-24) |
| `DayIntensity` | float | 10.0 | Sun intensity during day |
| `NightIntensity` | float | 0.5 | Sun intensity during night |
| `DayColor` | FLinearColor | (1, 0.95, 0.8) | Sun color during day |
| `SunsetColor` | FLinearColor | (1, 0.5, 0.2) | Sun color at sunrise/sunset |
| `NightColor` | FLinearColor | (0.5, 0.6, 0.8) | Sun color during night |

#### Public Methods

##### `float GetTimeOfDay() const`

Returns the current time of day.

**Returns**: Float value between 0 and 24

##### `void SetTimeOfDay(float NewTime)`

Sets the time of day manually.

**Parameters**:
- `NewTime`: Time in hours (0-24)

**Example**:
```cpp
// Set to sunset
DayNightManager->SetTimeOfDay(18.0f);

// Set to midnight
DayNightManager->SetTimeOfDay(0.0f);
```

##### `float GetNormalizedTimeOfDay() const`

Returns normalized time of day.

**Returns**: Float value between 0 and 1

---

### DynamicWaterActor

Creates a dynamic water plane with custom shader support.

#### Public Properties

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `WaterWidth` | float | 10000.0 | Width of water plane |
| `WaterLength` | float | 10000.0 | Length of water plane |
| `WaterLevel` | float | 0.0 | Water surface height (Z) |
| `Subdivisions` | int32 | 20 | Mesh detail level (1-100) |
| `WaterMaterial` | UMaterialInterface* | nullptr | Water material reference |
| `bEnableWaves` | bool | true | Enable wave animation |
| `WaveSpeed` | float | 1.0 | Wave animation speed |
| `WaveHeight` | float | 50.0 | Wave height multiplier |

#### Public Methods

##### `void GenerateWaterMesh()`

Generates or regenerates the water mesh.

---

### FreeCameraPawn

Free-flying camera pawn for world exploration.

#### Public Properties

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `BaseMovementSpeed` | float | 1000.0 | Base movement speed |
| `SprintMultiplier` | float | 3.0 | Sprint speed multiplier |
| `SlowMultiplier` | float | 0.3 | Slow movement multiplier |
| `LookSensitivity` | float | 1.0 | Mouse look sensitivity |
| `bSmoothRotation` | bool | true | Enable smooth rotation |
| `RotationSmoothSpeed` | float | 10.0 | Rotation interpolation speed |

---

## Blueprint Usage

### Creating a Terrain in Blueprint

1. Add `ProceduralTerrainActor` to level
2. In Event Graph, call `Generate Terrain` when ready
3. Access properties using Get/Set nodes

**Example Blueprint**:
```
Event BeginPlay
  → Get Procedural Terrain Actor
  → Set Terrain Width (150)
  → Set Max Height (3000.0)
  → Set Random Seed (Random Integer)
  → Generate Terrain
```

### Controlling Day/Night Cycle

**Blueprint snippet to advance time**:
```
Event Tick
  → Get Day Night Cycle Manager
  → Get Time Of Day
  → Add (Delta Seconds * Cycle Speed)
  → Set Time Of Day
```

**Blueprint snippet for time-based events**:
```
Event Tick
  → Get Day Night Cycle Manager
  → Get Normalized Time Of Day
  → Branch (< 0.25 or > 0.75)
    True: // Night time logic
    False: // Day time logic
```

---

## Material Parameters

### Water Material Parameters

When creating a material for `DynamicWaterActor`, expose these parameters:

| Parameter Name | Type | Description |
|----------------|------|-------------|
| `Time` | Scalar | Current animation time |
| `WaveHeight` | Scalar | Height of waves |
| `WaveSpeed` | Scalar | Animation speed |
| `WaveScale` | Scalar | Scale of wave patterns |
| `ShallowWaterColor` | Vector | Color for shallow water |
| `DeepWaterColor` | Vector | Color for deep water |

---

## Events and Delegates

Currently, the system does not expose custom events or delegates. This could be extended in future versions to support:
- OnTerrainGenerated
- OnTimeOfDayChanged
- OnDayNightTransition

---

## Performance Considerations

### Terrain Generation
- Keep `TerrainWidth` and `TerrainHeight` under 200 for real-time generation
- Higher `Octaves` values (6+) increase computation time
- Use async cooking for large terrains

### Water Rendering
- Keep `Subdivisions` under 50 for good performance
- Higher subdivisions improve wave quality but increase vertex count
- Use LOD system for large water bodies

### Day/Night Cycle
- Very lightweight, minimal performance impact
- Update frequency is per-frame but calculations are simple

---

## Shader Integration

### Using Custom Shaders

The water shader (`WaterShader.usf`) can be integrated into Unreal materials:

1. Create a Material in Content Browser
2. Create a Custom node
3. Reference shader functions
4. Connect parameters from material

**Example Custom node setup**:
- Code: `return GetVertexOffset(WorldPosition, Time);`
- Output Type: Float3
- Inputs: WorldPosition (Float3), Time (Float1)

---

## Extending the System

### Adding New Noise Types

Extend `NoiseGenerator` to add new noise algorithms:

```cpp
// In NoiseGenerator.h
UFUNCTION(BlueprintCallable, Category = "TerraForge|Noise")
float GenerateVoronoiNoise2D(float X, float Y, float Scale);

// In NoiseGenerator.cpp
float UNoiseGenerator::GenerateVoronoiNoise2D(float X, float Y, float Scale)
{
    // Implement Voronoi/Worley noise
    // ...
}
```

### Creating Custom Terrain Features

Override `ProceduralTerrainActor` to add features:

```cpp
class AAdvancedTerrainActor : public AProceduralTerrainActor
{
    virtual void GenerateVertices(...) override
    {
        Super::GenerateVertices(...);
        // Add caves, overhangs, etc.
    }
};
```

---

## Troubleshooting

### Common Issues

**Q: Terrain appears flat**
- Check `MaxHeight` is not zero
- Verify `NoiseScale` is reasonable (50-200)
- Ensure `Octaves` is at least 2

**Q: Water not animating**
- Check `bEnableWaves` is true
- Verify `WaveSpeed` is greater than zero
- Ensure material has Time parameter connected

**Q: Camera controls not working**
- Check Input settings in Project Settings
- Verify `FreeCameraPawn` is set as default pawn
- Ensure player controller is valid

---

For more information, see the main [README.md](../README.md).
