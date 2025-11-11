# TerraForge - Procedural World Generator

An interactive procedural world generator built with Unreal Engine that creates unique 3D landscapes on the fly. Explore dynamically generated terrain with realistic water and a real-time day/night cycle.

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.6-blue)
![C++](https://img.shields.io/badge/C++-17-brightgreen)
![License](https://img.shields.io/badge/license-MIT-orange)

## Features

### 🏔️ Procedural Terrain Generation
The terrain uses Perlin and Simplex noise to generate heightmaps. You can tweak height, scale, octaves, persistence, and lacunarity to get different landscapes. Real-time mesh generation with smooth normals and proper UV mapping is in progress. Seed-based generation for reproducible worlds and Blueprint-exposed functions are planned.

### 🌊 Dynamic Water System
A custom HLSL water shader is planned with animated waves using fractal Brownian motion, Fresnel reflections, depth-based color transitions from shallow to deep water, and normal mapping for surface detail. The procedurally generated water mesh will have adjustable subdivision, and wave parameters will be controllable at runtime.

### 🌅 Day/Night Cycle
Time progresses automatically at whatever speed you set, though it still needs some bug fixes. The sun moves based on the time of day, and colors transition smoothly between day, sunrise/sunset, and night. You can adjust the intensity for different times. There's a realistic sky for the skybox, but it needs to be made dynamic.

### 🎮 Free Camera Controls
Standard WASD movement with smooth interpolation, inverted mouse look, E/Q for up/down, Shift to sprint, Ctrl to slow down. Speed and sensitivity are all configurable.  

## Installation

### Option 1: Clone and Build

First, clone the repo:
```bash
git clone https://github.com/UkuRenekKronbergs/TerraForge-Procedural-World-Generator.git
cd TerraForge-Procedural-World-Generator
```

Then generate project files:
- **Windows**: Right-click `TerraForge.uproject` → Generate Visual Studio project files
- **Mac**: Right-click `TerraForge.uproject` → Generate Xcode project files
- **Linux**: Run `UnrealEngine/Engine/Build/BatchFiles/Linux/GenerateProjectFiles.sh TerraForge.uproject`

Build it:
- **Windows**: Open `TerraForge.sln` in Visual Studio 2022, set build configuration to `Development Editor`, and build
- **Mac**: Open `TerraForge.xcworkspace` in Xcode and build
- **Linux**: Run `make TerraForgeEditor`

Finally, open `TerraForge.uproject` with Unreal Engine.

### Option 2: Pre-built .exe folder (for testing the game)

If there's a pre-built version, grab it from the [Releases](https://github.com/UkuRenekKronbergs/TerraForge-Procedural-World-Generator/releases) page.

## Usage

### Quick Start

Open the project in Unreal Engine, then create a new level or use an existing one.

To add terrain, drag `ProceduralTerrainActor` from the Content Browser into the level. Adjust the parameters in the Details panel (size, noise scale, octaves, etc.) and the terrain will generate automatically.

For water, drag `DynamicWaterActor` into the level and position it where you want the water level. Assign the water material if you have one.

To add the day/night cycle, drag `DayNightCycleManager` into the level. Configure the time of day, cycle speed, and sun properties. You can optionally link it to an existing Directional Light.

Hit Play in the editor to test it out. Use **WASD** to move, **Mouse** to look around, **E/Q** for vertical movement, **Shift** to sprint, and **Ctrl** to move slowly.

### C++ Classes

#### ProceduralTerrainActor
This generates terrain meshes using noise functions. Key properties:
- `TerrainWidth/Height`: Grid dimensions
- `GridSize`: Size of each grid square
- `MaxHeight`: Maximum terrain elevation
- `NoiseScale`: Scale of noise patterns
- `Octaves`: Detail level (more = more detail)
- `Persistence`: Amplitude reduction per octave
- `Lacunarity`: Frequency increase per octave
- `RandomSeed`: Seed for reproducible generation
- `bUseSimplexNoise`: Toggle between Perlin and Simplex noise

Example usage:
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
Handles Perlin and Simplex noise generation. Functions:
- `GeneratePerlinNoise2D()`: Multi-octave Perlin noise
- `GenerateSimplexNoise2D()`: Simplex noise (faster, more organic)
- `GeneratePerlinNoise3D()`: 3D Perlin noise for volumetric effects
- `SetSeed()`: Set random seed

#### DayNightCycleManager
Manages the day/night cycle and sun lighting. Key properties:
- `TimeOfDay`: Current time (0-24 hours)
- `CycleSpeed`: Hours per real-world second
- `bAutoProgress`: Enable automatic time progression
- `SunriseTime/SunsetTime`: Times for day/night transitions
- `DayIntensity/NightIntensity`: Light intensity values
- `DayColor/NightColor/SunsetColor`: Light colors for different times

Blueprint functions:
- `GetTimeOfDay()`: Returns current time
- `SetTimeOfDay(float)`: Manually set time
- `GetNormalizedTimeOfDay()`: Returns 0-1 value for time

#### DynamicWaterActor
Creates a water plane with custom shader support. Key properties:
- `WaterWidth/Length`: Dimensions of water plane
- `WaterLevel`: Height (Z position)
- `Subdivisions`: Mesh detail level
- `bEnableWaves`: Toggle wave animation
- `WaveSpeed/WaveHeight`: Wave parameters
- `WaterMaterial`: Material to apply

#### FreeCameraPawn
Free-flying camera for exploring the world. Key properties:
- `BaseMovementSpeed`: Default movement speed
- `SprintMultiplier`: Speed boost when sprinting
- `SlowMultiplier`: Speed reduction for slow movement
- `LookSensitivity`: Mouse sensitivity
- `bSmoothRotation`: Enable smooth camera rotation
- `RotationSmoothSpeed`: Interpolation speed for rotation

### Blueprint Integration

All classes are Blueprint-accessible and can be extended. To create a Blueprint from a C++ class, right-click in the Content Browser → Blueprint Class, choose your parent class (e.g., `ProceduralTerrainActor`), and customize it in the Blueprint editor.

Any `UPROPERTY` marked with `BlueprintReadWrite` can be modified, and any `UFUNCTION` marked with `BlueprintCallable` can be called.

To create custom terrain algorithms, extend `ProceduralTerrainActor` in Blueprint, use the Event Graph to modify terrain generation logic, and access `NoiseGenerator` functions for custom height calculations.


## Customization

### Creating Custom Terrain Types

Here are some preset configurations for different terrain types:

**Desert terrain**:
```cpp
TerrainWidth = 200;
TerrainHeight = 200;
NoiseScale = 200.0f;
MaxHeight = 500.0f;
Octaves = 2;
Persistence = 0.3f;
```

**Mountain terrain**:
```cpp
TerrainWidth = 150;
TerrainHeight = 150;
NoiseScale = 80.0f;
MaxHeight = 5000.0f;
Octaves = 6;
Persistence = 0.6f;
Lacunarity = 2.5f;
```

**Rolling hills**:
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

You can add biomes, vegetation, or weather. Here's an example for biome-based terrain coloring:

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
