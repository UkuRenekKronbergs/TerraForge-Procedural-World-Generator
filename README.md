# TerraForge - Procedural World Generator

An interactive procedural world generator built with Unreal Engine that creates unique 3D landscapes on the fly. Explore dynamically generated terrain with realistic water and a real-time day/night cycle.

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.6-blue)
![C++](https://img.shields.io/badge/C++-17-brightgreen)
![License](https://img.shields.io/badge/license-MIT-orange)

## Features

### 🏔️ Procedural Terrain Generation
- **Noise-based terrain generation** using Perlin and Simplex noise algorithms
- **Customizable parameters**: height, scale, octaves, persistence, and lacunarity
- **Real-time mesh generation** with smooth normals and proper UV mapping
- **Seed-based generation** for reproducible worlds
- **Blueprint-exposed functions** for easy customization

### 🌊 Dynamic Water System
- **Custom HLSL water shader** with:
  - Animated waves using fractal Brownian motion
  - Fresnel reflections for realistic water appearance
  - Depth-based color transitions (shallow to deep water)
  - Normal mapping for surface detail
- **Procedurally generated water mesh** with adjustable subdivision
- **Real-time wave parameters** controllable at runtime

### 🌅 Day/Night Cycle
- **Automatic time progression** with configurable speed
- **Dynamic sun positioning** based on time of day
- **Smooth color transitions** between day, sunrise/sunset, and night
- **Adjustable intensity** for different times of day
- **Blueprint integration** for easy time control

### 🎮 Free Camera Controls
- **WASD movement** with smooth interpolation
- **Mouse look** for camera orientation
- **E/Q for vertical movement** (ascend/descend)
- **Shift to sprint**, Ctrl to move slowly
- **Customizable speed** and sensitivity settings

## Installation

### Option 1: Clone and Build

1. **Clone the repository**:
   ```bash
   git clone https://github.com/UkuRenekKronbergs/TerraForge-Procedural-World-Generator.git
   cd TerraForge-Procedural-World-Generator
   ```

2. **Generate project files**:
   - **Windows**: Right-click `TerraForge.uproject` → Generate Visual Studio project files
   - **Mac**: Right-click `TerraForge.uproject` → Generate Xcode project files
   - **Linux**: Run `UnrealEngine/Engine/Build/BatchFiles/Linux/GenerateProjectFiles.sh TerraForge.uproject`

3. **Build the project**:
   - **Windows**: Open `TerraForge.sln` in Visual Studio 2022, set build configuration to `Development Editor`, and build
   - **Mac**: Open `TerraForge.xcworkspace` in Xcode and build
   - **Linux**: Run `make TerraForgeEditor`

4. **Launch the editor**:
   - Open `TerraForge.uproject` with Unreal Engine

### Option 2: Pre-built Binary (If Available)

Download the latest release from the [Releases](https://github.com/UkuRenekKronbergs/TerraForge-Procedural-World-Generator/releases) page.

## Usage

### Quick Start

1. **Open the project** in Unreal Engine
2. **Create a new level** or open an existing one
3. **Add terrain**:
   - Drag `ProceduralTerrainActor` from the Content Browser into the level
   - Adjust parameters in the Details panel (size, noise scale, octaves, etc.)
   - The terrain will generate automatically

4. **Add water**:
   - Drag `DynamicWaterActor` into the level
   - Position it at your desired water level
   - Assign the water material (if available)

5. **Add day/night cycle**:
   - Drag `DayNightCycleManager` into the level
   - Configure time of day, cycle speed, and sun properties
   - Optionally link it to an existing Directional Light

6. **Test in Play mode**:
   - Press Play in the editor
   - Use **WASD** to move, **Mouse** to look around
   - Use **E/Q** for vertical movement
   - Hold **Shift** to sprint, **Ctrl** to move slowly

### C++ Classes

#### ProceduralTerrainActor
Generates terrain meshes using noise functions.

**Key Properties**:
- `TerrainWidth/Height`: Grid dimensions
- `GridSize`: Size of each grid square
- `MaxHeight`: Maximum terrain elevation
- `NoiseScale`: Scale of noise patterns
- `Octaves`: Detail level (more = more detail)
- `Persistence`: Amplitude reduction per octave
- `Lacunarity`: Frequency increase per octave
- `RandomSeed`: Seed for reproducible generation
- `bUseSimplexNoise`: Toggle between Perlin and Simplex noise

**Example Usage**:
```cpp
// Spawn terrain in C++
AProceduralTerrainActor* Terrain = GetWorld()->SpawnActor<AProceduralTerrainActor>();
Terrain->TerrainWidth = 150;
Terrain->TerrainHeight = 150;
Terrain->MaxHeight = 3000.0f;
Terrain->RandomSeed = 54321;
Terrain->GenerateTerrain();
```

#### NoiseGenerator
Provides Perlin and Simplex noise generation functions.

**Functions**:
- `GeneratePerlinNoise2D()`: Multi-octave Perlin noise
- `GenerateSimplexNoise2D()`: Simplex noise (faster, more organic)
- `GeneratePerlinNoise3D()`: 3D Perlin noise for volumetric effects
- `SetSeed()`: Set random seed

#### DayNightCycleManager
Manages the day/night cycle and sun lighting.

**Key Properties**:
- `TimeOfDay`: Current time (0-24 hours)
- `CycleSpeed`: Hours per real-world second
- `bAutoProgress`: Enable automatic time progression
- `SunriseTime/SunsetTime`: Times for day/night transitions
- `DayIntensity/NightIntensity`: Light intensity values
- `DayColor/NightColor/SunsetColor`: Light colors for different times

**Blueprint Functions**:
- `GetTimeOfDay()`: Returns current time
- `SetTimeOfDay(float)`: Manually set time
- `GetNormalizedTimeOfDay()`: Returns 0-1 value for time

#### DynamicWaterActor
Creates a water plane with custom shader support.

**Key Properties**:
- `WaterWidth/Length`: Dimensions of water plane
- `WaterLevel`: Height (Z position)
- `Subdivisions`: Mesh detail level
- `bEnableWaves`: Toggle wave animation
- `WaveSpeed/WaveHeight`: Wave parameters
- `WaterMaterial`: Material to apply

#### FreeCameraPawn
Free-flying camera for world exploration.

**Key Properties**:
- `BaseMovementSpeed`: Default movement speed
- `SprintMultiplier`: Speed boost when sprinting
- `SlowMultiplier`: Speed reduction for slow movement
- `LookSensitivity`: Mouse sensitivity
- `bSmoothRotation`: Enable smooth camera rotation
- `RotationSmoothSpeed`: Interpolation speed for rotation

### Blueprint Integration

All classes are Blueprint-accessible and can be extended:

1. **Create Blueprint from C++ class**:
   - Right-click in Content Browser → Blueprint Class
   - Choose your parent class (e.g., `ProceduralTerrainActor`)
   - Customize in Blueprint editor

2. **Expose custom parameters**:
   - All `UPROPERTY` marked with `BlueprintReadWrite` can be modified
   - All `UFUNCTION` marked with `BlueprintCallable` can be called

3. **Create custom terrain algorithms**:
   - Extend `ProceduralTerrainActor` in Blueprint
   - Use Event Graph to modify terrain generation logic
   - Access `NoiseGenerator` functions for custom height calculations


## Customization

### Creating Custom Terrain Types

1. **Desert terrain**:
   ```cpp
   TerrainWidth = 200;
   TerrainHeight = 200;
   NoiseScale = 200.0f;
   MaxHeight = 500.0f;
   Octaves = 2;
   Persistence = 0.3f;
   ```

2. **Mountain terrain**:
   ```cpp
   TerrainWidth = 150;
   TerrainHeight = 150;
   NoiseScale = 80.0f;
   MaxHeight = 5000.0f;
   Octaves = 6;
   Persistence = 0.6f;
   Lacunarity = 2.5f;
   ```

3. **Rolling hills**:
   ```cpp
   TerrainWidth = 100;
   TerrainHeight = 100;
   NoiseScale = 150.0f;
   MaxHeight = 1500.0f;
   Octaves = 3;
   Persistence = 0.5f;
   bUseSimplexNoise = true;
   ```

### Extending the System

Add biomes, vegetation, or weather:

```cpp
// Example: Biome-based terrain coloring
void AProceduralTerrainActor::ApplyBiomeColors()
{
    for (int32 i = 0; i < Vertices.Num(); i++)
    {
        float Height = Vertices[i].Z;
        if (Height < 100.0f) {
            // Beach/Sand
            VertexColors[i] = FColor::Yellow;
        } else if (Height < 1000.0f) {
            // Grass
            VertexColors[i] = FColor::Green;
        } else {
            // Snow
            VertexColors[i] = FColor::White;
        }
    }
}
```

## Troubleshooting

### Terrain not generating
- Ensure `bAutoGenerate` is enabled
- Check that `NoiseScale` is not zero
- Verify `TerrainWidth` and `TerrainHeight` are positive

### Water not visible
- Check that `WaterLevel` is within view
- Ensure `WaterMaterial` is assigned
- Verify the water actor is not below the terrain

### Day/night cycle not working
- Ensure `bAutoProgress` is enabled
- Check that `CycleSpeed` is greater than zero
- Verify the `DirectionalLightActor` reference is set (if using external light)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
