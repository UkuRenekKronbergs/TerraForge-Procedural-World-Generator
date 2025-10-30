# Water Material Setup Guide

This guide explains how to create and configure the water material for TerraForge's dynamic water system.

## Overview

The water shader (`WaterShader.usf`) provides:
- Animated wave surfaces using fractal Brownian motion (FBM)
- Fresnel reflections for realistic water appearance
- Depth-based color transitions
- Dynamic normals for proper lighting

## Creating the Water Material

### Step 1: Create a New Material

1. In Content Browser, navigate to `Content/Materials/`
2. Right-click → Material → Name it "M_DynamicWater"
3. Double-click to open Material Editor

### Step 2: Configure Material Properties

In the Details panel, set:

- **Material Domain**: Surface
- **Blend Mode**: Translucent
- **Shading Model**: Default Lit
- **Translucency Lighting Mode**: Surface TranslucencyVolume
- **Enable "Two Sided"**: True (optional, for viewing from below)
- **Enable "Responsive AA"**: True (for better quality)

### Step 3: Set Up Base Material Parameters

Create the following parameters (right-click → Parameter):

#### Scalar Parameters

| Parameter Name | Default Value | Description |
|----------------|---------------|-------------|
| `Time` | 0.0 | Animation time (driven by actor) |
| `WaveSpeed` | 1.0 | Wave animation speed multiplier |
| `WaveScale` | 100.0 | Scale of wave patterns |
| `WaveHeight` | 50.0 | Height of wave displacement |
| `FresnelExponent` | 5.0 | Fresnel sharpness (higher = sharper) |
| `WaterDepth` | 1000.0 | Depth for color transitions |
| `Opacity` | 0.85 | Water transparency (0-1) |
| `Roughness` | 0.1 | Surface roughness |
| `Metallic` | 0.0 | Metallic property (keep at 0) |

#### Vector Parameters

| Parameter Name | Default Value | Description |
|----------------|---------------|-------------|
| `ShallowWaterColor` | (0.0, 0.8, 0.9) | Color in shallow areas (cyan) |
| `DeepWaterColor` | (0.0, 0.2, 0.5) | Color in deep areas (dark blue) |
| `FoamColor` | (1.0, 1.0, 1.0) | Color of wave peaks/foam |

### Step 4: Build Wave Animation System

#### A. Create Wave Function (Custom Node)

1. Add a **Custom** node
2. Name it "WaveNoise"
3. Set code to:

```hlsl
// Simple noise for waves
float Hash(float2 p)
{
    return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

float Noise(float2 p)
{
    float2 i = floor(p);
    float2 f = frac(p);
    float2 u = f * f * (3.0 - 2.0 * f);
    
    float a = Hash(i);
    float b = Hash(i + float2(1.0, 0.0));
    float c = Hash(i + float2(0.0, 1.0));
    float d = Hash(i + float2(1.0, 1.0));
    
    return lerp(lerp(a, b, u.x), lerp(c, d, u.x), u.y);
}

// Fractal Brownian Motion
float FBM(float2 p)
{
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;
    
    for (int i = 0; i < 4; i++)
    {
        value += amplitude * Noise(p * frequency);
        amplitude *= 0.5;
        frequency *= 2.0;
    }
    
    return value;
}

// Main wave calculation
float2 uv = UV / WaveScale;
float wave1 = FBM(uv + float2(Time * WaveSpeed * 0.1, 0.0));
float wave2 = FBM(uv * 2.0 - float2(0.0, Time * WaveSpeed * 0.15));
float wave3 = FBM(uv * 0.5 + float2(Time * WaveSpeed * 0.05, Time * WaveSpeed * 0.05));

return (wave1 + wave2 * 0.5 + wave3 * 0.25) / 1.75;
```

4. Add inputs:
   - `UV` (Float2): Connect WorldPosition.xy or TextureCoordinate
   - `Time` (Float): Connect Time parameter
   - `WaveSpeed` (Float): Connect WaveSpeed parameter
   - `WaveScale` (Float): Connect WaveScale parameter

#### B. Create Normal Calculation

1. Add another **Custom** node named "WaveNormals"
2. Calculate normals from wave height:

```hlsl
// Calculate gradient for normals
float epsilon = 1.0;
float heightCenter = WaveHeight;
float heightRight = WaveHeightRight;  // Sample offset in X
float heightUp = WaveHeightUp;        // Sample offset in Y

float3 tangent = normalize(float3(epsilon, 0.0, heightRight - heightCenter));
float3 bitangent = normalize(float3(0.0, epsilon, heightUp - heightCenter));

return normalize(cross(bitangent, tangent));
```

**Simplified Alternative**: Use Material's built-in normal generation:
1. Connect wave output to World Position Offset
2. Enable "Generate Normal From Height"

### Step 5: Create Color Blend System

1. **Lerp node** for depth-based colors:
   - A: DeepWaterColor parameter
   - B: ShallowWaterColor parameter
   - Alpha: Create a depth mask (see below)

2. **Depth Fade** node (for depth mask):
   - Fade Distance: 500.0
   - Opacity: 1.0
   - This creates gradient from deep to shallow

3. **Fresnel** node for reflections:
   - Exponent: Connect FresnelExponent parameter
   - Base Reflect Fraction: 0.1
   - Normal: Connect calculated normals (or leave default)

4. **Lerp** between water color and white/sky color:
   - A: Water color (from depth lerp)
   - B: FLinearColor(1, 1, 1) or sky color
   - Alpha: Fresnel output * 0.3

### Step 6: Connect to Material Outputs

Connect nodes to material output:

- **Base Color**: Final color from Fresnel lerp
- **Metallic**: Metallic parameter (0.0)
- **Roughness**: Roughness parameter or lerp based on Fresnel
- **Normal**: Calculated wave normals
- **Opacity**: Opacity parameter
- **World Position Offset**: Wave displacement (WaveHeight * WaveNoise result * VertexNormalWS)

### Step 7: Add Optional Enhancements

#### Foam on Wave Peaks

1. Add **If** node: `if WaveNoise > 0.8`
2. Lerp foam color onto base color based on wave height

#### Refraction

1. Add **Scene Texture** node, set to PostProcessInput0
2. **Multiply** by refraction strength (0.1)
3. Add to UV coordinates for distortion

#### Subsurface Scattering

1. Add subsurface color (light green/blue)
2. Multiply by (1 - depth)
3. Add to base color

## Material Instance Setup

After creating the base material:

1. Right-click M_DynamicWater → Create Material Instance
2. Name it "MI_DynamicWater"
3. Enable parameters you want to customize
4. Adjust values:
   - **ShallowWaterColor**: Lighter blue/cyan for tropical water
   - **DeepWaterColor**: Darker blue for deep ocean
   - **WaveHeight**: 20-100 depending on water body size
   - **WaveSpeed**: 0.5-2.0 for calm to stormy

## Applying to Water Actor

1. Select DynamicWaterActor in level
2. In Details panel, find "Water Material"
3. Select MI_DynamicWater
4. The actor will automatically update material parameters

## Performance Optimization

### For Better Performance

- Reduce Custom node complexity (fewer FBM iterations)
- Lower mesh subdivisions on DynamicWaterActor
- Use simpler normal calculation
- Disable refraction if not needed

### For Better Quality

- Increase FBM iterations (up to 6-8)
- Add more wave layers with different scales
- Enable high-precision normals
- Add foam, refraction, and subsurface scattering

## Troubleshooting

### Water appears black
- Check that material domain is "Surface"
- Verify lighting in scene (add a light)
- Check Base Color is connected

### Waves not animating
- Ensure Time parameter is being updated by actor
- Check WaveSpeed is greater than 0
- Verify Custom nodes compiled without errors

### Water is too transparent/opaque
- Adjust Opacity parameter (0.7-0.95 range)
- Check Blend Mode is "Translucent"
- Adjust Translucency Lighting Mode

### Normals look wrong
- Recalculate normals with correct gradient
- Try Material's "Derive Normal From Height"
- Check normal vector range is -1 to 1

### Performance is poor
- Reduce material complexity (remove Custom nodes)
- Lower water mesh subdivisions
- Disable expensive features (refraction, SSS)
- Use Material Quality switches for scalability

## Example Presets

### Calm Lake
```
WaveHeight: 10.0
WaveSpeed: 0.3
WaveScale: 150.0
ShallowWaterColor: (0.1, 0.7, 0.8)
DeepWaterColor: (0.0, 0.3, 0.5)
Opacity: 0.9
```

### Ocean
```
WaveHeight: 80.0
WaveSpeed: 1.5
WaveScale: 80.0
ShallowWaterColor: (0.0, 0.6, 0.7)
DeepWaterColor: (0.0, 0.1, 0.3)
Opacity: 0.8
```

### Tropical Waters
```
WaveHeight: 30.0
WaveSpeed: 0.8
WaveScale: 120.0
ShallowWaterColor: (0.2, 0.9, 0.95)
DeepWaterColor: (0.0, 0.4, 0.6)
Opacity: 0.85
```

### Stormy Sea
```
WaveHeight: 150.0
WaveSpeed: 2.5
WaveScale: 60.0
ShallowWaterColor: (0.3, 0.4, 0.5)
DeepWaterColor: (0.1, 0.2, 0.3)
Opacity: 0.75
```

## Advanced: Custom Shader Integration

To directly integrate the provided `WaterShader.usf`:

1. Place shader file in `Shaders/` folder
2. Reference in Material using **Custom** node
3. Include shader: `#include "/Project/WaterShader.usf"`
4. Call shader functions directly

**Note**: This requires Unreal Engine to recognize custom shader directories. See Unreal documentation on custom shader development.

---

For more details on materials in Unreal Engine, see:
- [Unreal Engine Material Documentation](https://docs.unrealengine.com/en-US/RenderingAndGraphics/Materials/)
- [Material Parameter Collections](https://docs.unrealengine.com/en-US/RenderingAndGraphics/Materials/ParameterCollections/)

For TerraForge-specific questions, refer to the [API Reference](API_REFERENCE.md).
