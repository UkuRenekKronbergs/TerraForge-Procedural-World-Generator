# TerraForge Examples

This document provides practical examples for using TerraForge to create different types of procedural worlds.

## Table of Contents

1. [Basic Terrain Setup](#basic-terrain-setup)
2. [Terrain Presets](#terrain-presets)
3. [Water Configurations](#water-configurations)
4. [Day/Night Cycle Setup](#daynight-cycle-setup)
5. [Custom Camera Configurations](#custom-camera-configurations)
6. [Complete World Examples](#complete-world-examples)
7. [C++ Code Examples](#c-code-examples)
8. [Blueprint Examples](#blueprint-examples)

---

## Basic Terrain Setup

### Simple Flat Terrain with Gentle Hills

```cpp
// In Level Blueprint or C++
AProceduralTerrainActor* Terrain = GetWorld()->SpawnActor<AProceduralTerrainActor>();
Terrain->SetActorLocation(FVector(0, 0, 0));
Terrain->TerrainWidth = 100;
Terrain->TerrainHeight = 100;
Terrain->GridSize = 100.0f;
Terrain->MaxHeight = 500.0f;
Terrain->NoiseScale = 200.0f;
Terrain->Octaves = 2;
Terrain->Persistence = 0.3f;
Terrain->Lacunarity = 2.0f;
Terrain->RandomSeed = 12345;
Terrain->bUseSimplexNoise = false;
Terrain->GenerateTerrain();
```

---

## Terrain Presets

### 1. Rolling Hills
Perfect for pastoral landscapes and gentle exploration.

**Parameters**:
```
TerrainWidth: 100
TerrainHeight: 100
GridSize: 100.0
MaxHeight: 800.0
NoiseScale: 150.0
Octaves: 3
Persistence: 0.45
Lacunarity: 2.0
bUseSimplexNoise: true
```

### 2. Mountain Range
Dramatic peaks and valleys for adventure settings.

**Parameters**:
```
TerrainWidth: 150
TerrainHeight: 150
GridSize: 100.0
MaxHeight: 5000.0
NoiseScale: 80.0
Octaves: 6
Persistence: 0.6
Lacunarity: 2.5
bUseSimplexNoise: false
```

### 3. Desert Dunes
Smooth, flowing sand formations.

**Parameters**:
```
TerrainWidth: 120
TerrainHeight: 120
GridSize: 100.0
MaxHeight: 400.0
NoiseScale: 250.0
Octaves: 2
Persistence: 0.25
Lacunarity: 1.8
bUseSimplexNoise: true
```

### 4. Rocky Badlands
Rugged, irregular terrain with sharp features.

**Parameters**:
```
TerrainWidth: 100
TerrainHeight: 100
GridSize: 100.0
MaxHeight: 1500.0
NoiseScale: 60.0
Octaves: 7
Persistence: 0.7
Lacunarity: 3.0
bUseSimplexNoise: false
```

### 5. Volcanic Landscape
Jagged peaks with steep slopes.

**Parameters**:
```
TerrainWidth: 80
TerrainHeight: 80
GridSize: 100.0
MaxHeight: 3500.0
NoiseScale: 50.0
Octaves: 8
Persistence: 0.65
Lacunarity: 2.8
bUseSimplexNoise: false
```

---

## Water Configurations

### Calm Lake

```cpp
ADynamicWaterActor* Water = GetWorld()->SpawnActor<ADynamicWaterActor>();
Water->SetActorLocation(FVector(5000, 5000, 0));
Water->WaterWidth = 10000.0f;
Water->WaterLength = 10000.0f;
Water->WaterLevel = 0.0f;
Water->Subdivisions = 20;
Water->bEnableWaves = true;
Water->WaveSpeed = 0.3f;
Water->WaveHeight = 10.0f;
Water->GenerateWaterMesh();
```

**Material Parameters**:
```
ShallowWaterColor: (0.1, 0.7, 0.8)
DeepWaterColor: (0.0, 0.3, 0.5)
Opacity: 0.9
```

### Ocean Waves

```cpp
Water->WaterWidth = 50000.0f;
Water->WaterLength = 50000.0f;
Water->WaterLevel = 0.0f;
Water->Subdivisions = 40;
Water->WaveSpeed = 1.5f;
Water->WaveHeight = 80.0f;
```

**Material Parameters**:
```
ShallowWaterColor: (0.0, 0.6, 0.7)
DeepWaterColor: (0.0, 0.1, 0.3)
Opacity: 0.8
```

### Tropical Lagoon

```cpp
Water->WaterWidth = 8000.0f;
Water->WaterLength = 8000.0f;
Water->WaterLevel = 0.0f;
Water->Subdivisions = 25;
Water->WaveSpeed = 0.5f;
Water->WaveHeight = 20.0f;
```

**Material Parameters**:
```
ShallowWaterColor: (0.2, 0.9, 0.95)
DeepWaterColor: (0.0, 0.4, 0.6)
Opacity: 0.85
```

### Stormy Sea

```cpp
Water->WaterWidth = 30000.0f;
Water->WaterLength = 30000.0f;
Water->WaterLevel = 0.0f;
Water->Subdivisions = 50;
Water->WaveSpeed = 2.5f;
Water->WaveHeight = 150.0f;
```

**Material Parameters**:
```
ShallowWaterColor: (0.3, 0.4, 0.5)
DeepWaterColor: (0.1, 0.2, 0.3)
Opacity: 0.75
```

---

## Day/Night Cycle Setup

### Normal Day/Night Cycle

24-hour cycle completed in 24 real minutes.

```cpp
ADayNightCycleManager* DayNight = GetWorld()->SpawnActor<ADayNightCycleManager>();
DayNight->TimeOfDay = 12.0f;  // Start at noon
DayNight->CycleSpeed = 1.0f;   // 1 hour per second
DayNight->bAutoProgress = true;
DayNight->SunriseTime = 6.0f;
DayNight->SunsetTime = 18.0f;
DayNight->DayIntensity = 10.0f;
DayNight->NightIntensity = 0.5f;
```

### Fast Time-Lapse

Complete day in 2 real minutes.

```cpp
DayNight->CycleSpeed = 12.0f;  // 12 hours per second
```

### Eternal Sunset

Frozen at sunset for dramatic lighting.

```cpp
DayNight->SetTimeOfDay(18.0f);
DayNight->bAutoProgress = false;
```

### Long Days, Short Nights

Arctic summer simulation.

```cpp
DayNight->SunriseTime = 4.0f;   // Early sunrise
DayNight->SunsetTime = 22.0f;   // Late sunset
DayNight->CycleSpeed = 1.0f;
```

### Custom Colors

Fantasy world with purple sunset.

```cpp
DayNight->DayColor = FLinearColor(1.0f, 0.95f, 0.8f);
DayNight->SunsetColor = FLinearColor(0.8f, 0.2f, 0.9f);  // Purple
DayNight->NightColor = FLinearColor(0.3f, 0.3f, 0.6f);
```

---

## Custom Camera Configurations

### Slow Exploration Camera

For detailed world examination.

```cpp
AFreeCameraPawn* Camera = GetWorld()->SpawnActor<AFreeCameraPawn>();
Camera->BaseMovementSpeed = 500.0f;
Camera->SprintMultiplier = 2.0f;
Camera->SlowMultiplier = 0.2f;
Camera->LookSensitivity = 0.5f;
Camera->bSmoothRotation = true;
Camera->RotationSmoothSpeed = 5.0f;
```

### Fast Flythrough Camera

For quick world traversal.

```cpp
Camera->BaseMovementSpeed = 3000.0f;
Camera->SprintMultiplier = 5.0f;
Camera->SlowMultiplier = 0.5f;
Camera->LookSensitivity = 2.0f;
Camera->bSmoothRotation = true;
Camera->RotationSmoothSpeed = 15.0f;
```

### Cinematic Camera

Smooth, dramatic movements.

```cpp
Camera->BaseMovementSpeed = 800.0f;
Camera->SprintMultiplier = 1.5f;
Camera->SlowMultiplier = 0.3f;
Camera->LookSensitivity = 0.3f;
Camera->bSmoothRotation = true;
Camera->RotationSmoothSpeed = 3.0f;
```

---

## Complete World Examples

### Example 1: Island Paradise

```cpp
void SetupIslandWorld()
{
    // Terrain: Gentle island with hills
    AProceduralTerrainActor* Terrain = SpawnActor<AProceduralTerrainActor>();
    Terrain->SetActorLocation(FVector(0, 0, -200));
    Terrain->TerrainWidth = 150;
    Terrain->TerrainHeight = 150;
    Terrain->GridSize = 100.0f;
    Terrain->MaxHeight = 1500.0f;
    Terrain->NoiseScale = 120.0f;
    Terrain->Octaves = 4;
    Terrain->Persistence = 0.5f;
    Terrain->Lacunarity = 2.2f;
    Terrain->RandomSeed = 54321;
    Terrain->GenerateTerrain();
    
    // Water: Tropical lagoon
    ADynamicWaterActor* Water = SpawnActor<ADynamicWaterActor>();
    Water->SetActorLocation(FVector(7500, 7500, 0));
    Water->WaterWidth = 15000.0f;
    Water->WaterLength = 15000.0f;
    Water->WaterLevel = 0.0f;
    Water->Subdivisions = 30;
    Water->WaveSpeed = 0.6f;
    Water->WaveHeight = 25.0f;
    Water->GenerateWaterMesh();
    
    // Day/Night: Tropical cycle
    ADayNightCycleManager* DayNight = SpawnActor<ADayNightCycleManager>();
    DayNight->TimeOfDay = 14.0f;  // Afternoon
    DayNight->CycleSpeed = 0.5f;   // Slow cycle
    DayNight->bAutoProgress = true;
    DayNight->SunriseTime = 6.0f;
    DayNight->SunsetTime = 19.0f;
    DayNight->DayIntensity = 12.0f;  // Bright sun
}
```

### Example 2: Mountain Valley

```cpp
void SetupMountainValley()
{
    // Terrain: Dramatic mountains
    AProceduralTerrainActor* Terrain = SpawnActor<AProceduralTerrainActor>();
    Terrain->SetActorLocation(FVector(0, 0, 0));
    Terrain->TerrainWidth = 200;
    Terrain->TerrainHeight = 200;
    Terrain->GridSize = 100.0f;
    Terrain->MaxHeight = 6000.0f;
    Terrain->NoiseScale = 70.0f;
    Terrain->Octaves = 7;
    Terrain->Persistence = 0.6f;
    Terrain->Lacunarity = 2.5f;
    Terrain->RandomSeed = 99999;
    Terrain->GenerateTerrain();
    
    // Water: Mountain lake
    ADynamicWaterActor* Water = SpawnActor<ADynamicWaterActor>();
    Water->SetActorLocation(FVector(10000, 10000, 500));
    Water->WaterWidth = 5000.0f;
    Water->WaterLength = 5000.0f;
    Water->WaterLevel = 500.0f;
    Water->Subdivisions = 20;
    Water->WaveSpeed = 0.2f;
    Water->WaveHeight = 8.0f;
    Water->GenerateWaterMesh();
    
    // Day/Night: Alpine lighting
    ADayNightCycleManager* DayNight = SpawnActor<ADayNightCycleManager>();
    DayNight->TimeOfDay = 8.0f;  // Morning
    DayNight->CycleSpeed = 1.0f;
    DayNight->bAutoProgress = true;
    DayNight->DayColor = FLinearColor(1.0f, 0.98f, 0.85f);  // Cool daylight
}
```

### Example 3: Alien Planet

```cpp
void SetupAlienWorld()
{
    // Terrain: Strange formations
    AProceduralTerrainActor* Terrain = SpawnActor<AProceduralTerrainActor>();
    Terrain->TerrainWidth = 180;
    Terrain->TerrainHeight = 180;
    Terrain->MaxHeight = 4000.0f;
    Terrain->NoiseScale = 45.0f;
    Terrain->Octaves = 8;
    Terrain->Persistence = 0.75f;
    Terrain->Lacunarity = 3.5f;
    Terrain->RandomSeed = 77777;
    Terrain->bUseSimplexNoise = true;
    Terrain->GenerateTerrain();
    
    // Water: Alien liquid (adjust material colors)
    ADynamicWaterActor* Water = SpawnActor<ADynamicWaterActor>();
    Water->WaterWidth = 20000.0f;
    Water->WaterLength = 20000.0f;
    Water->WaterLevel = -100.0f;
    Water->Subdivisions = 35;
    Water->WaveSpeed = 1.2f;
    Water->WaveHeight = 60.0f;
    // Set alien colors in material: green/purple tint
    
    // Day/Night: Strange lighting cycle
    ADayNightCycleManager* DayNight = SpawnActor<ADayNightCycleManager>();
    DayNight->CycleSpeed = 2.0f;  // Fast cycle
    DayNight->DayColor = FLinearColor(0.9f, 0.7f, 1.0f);  // Purple tint
    DayNight->SunsetColor = FLinearColor(1.0f, 0.3f, 0.5f);  // Pink
    DayNight->NightColor = FLinearColor(0.2f, 0.5f, 0.8f);  // Blue
}
```

---

## C++ Code Examples

### Random Seed Generation

```cpp
void GenerateRandomWorld()
{
    int32 RandomSeed = FMath::Rand();
    
    AProceduralTerrainActor* Terrain = GetWorld()->SpawnActor<AProceduralTerrainActor>();
    Terrain->RandomSeed = RandomSeed;
    Terrain->GenerateTerrain();
    
    UE_LOG(LogTemp, Log, TEXT("Generated world with seed: %d"), RandomSeed);
}
```

### Dynamic Parameter Updates

```cpp
void UpdateTerrainHeight(float NewMaxHeight)
{
    if (TerrainActor)
    {
        TerrainActor->MaxHeight = NewMaxHeight;
        TerrainActor->GenerateTerrain();
    }
}

void UpdateWaterLevel(float NewLevel)
{
    if (WaterActor)
    {
        WaterActor->WaterLevel = NewLevel;
        WaterActor->GenerateWaterMesh();
    }
}
```

### Time Control

```cpp
void SetTimeOfDay(float Hour)
{
    if (DayNightManager)
    {
        DayNightManager->SetTimeOfDay(FMath::Clamp(Hour, 0.0f, 24.0f));
    }
}

void ToggleTimePause()
{
    if (DayNightManager)
    {
        DayNightManager->bAutoProgress = !DayNightManager->bAutoProgress;
    }
}
```

---

## Blueprint Examples

### Terrain Generation on Key Press

**Event Graph**:
1. Event: `Key Press (G)`
2. Call: `Generate Terrain` on TerrainActor reference
3. Print: "Terrain regenerated!"

### Water Level Slider

**Widget Blueprint**:
1. Add Slider widget (0 to 1000 range)
2. On Value Changed:
   - Get WaterActor reference
   - Set Water Level to slider value
   - Call Generate Water Mesh

### Time of Day Display

**Widget Blueprint**:
1. Add Text widget
2. On Tick:
   - Get DayNightManager reference
   - Get Time Of Day
   - Format as "HH:MM"
   - Set Text

### Procedural World Generator Menu

**UI Elements**:
- Terrain Width slider
- Terrain Height slider
- Max Height slider
- Noise Scale slider
- Random Seed input
- Generate button

**Generate Button Logic**:
1. Get all slider values
2. Set TerrainActor properties
3. Call GenerateTerrain()
4. Close menu

---

For more information, see:
- [README.md](../README.md) for general usage
- [API_REFERENCE.md](API_REFERENCE.md) for detailed API
- [SETUP_GUIDE.md](SETUP_GUIDE.md) for installation

Happy world building! 🌍
