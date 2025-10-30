# TerraForge Setup Guide

This guide will walk you through setting up TerraForge on your system.

## Prerequisites

Before you begin, ensure you have the following installed:

### Required Software

1. **Unreal Engine 5.3 or later**
   - Download from [Epic Games Launcher](https://www.unrealengine.com/download)
   - Minimum version: 5.2 (recommended: 5.3+)

2. **C++ Compiler**
   
   **Windows**:
   - Visual Studio 2022 (Community, Professional, or Enterprise)
   - Required workloads:
     - Game development with C++
     - .NET desktop development
   - Download from [visualstudio.microsoft.com](https://visualstudio.microsoft.com/)

   **Mac**:
   - Xcode 14.0 or later
   - Command Line Tools for Xcode
   - Download from Mac App Store or [developer.apple.com](https://developer.apple.com/xcode/)

   **Linux**:
   - GCC 11+ or Clang 13+
   - Build essentials package
   - Install: `sudo apt-get install build-essential`

3. **Git** (for cloning the repository)
   - Download from [git-scm.com](https://git-scm.com/)

### System Requirements

**Minimum**:
- OS: Windows 10 64-bit, macOS 12+, or Ubuntu 20.04+
- Processor: Quad-core Intel or AMD, 2.5 GHz+
- RAM: 8 GB
- Graphics: DirectX 12 or Vulkan compatible GPU
- Storage: 10 GB available space

**Recommended**:
- OS: Windows 11 64-bit, macOS 13+, or Ubuntu 22.04+
- Processor: 8-core Intel or AMD, 3.5 GHz+
- RAM: 16 GB or more
- Graphics: NVIDIA RTX or AMD Radeon with 6GB+ VRAM
- Storage: 20 GB available space (SSD recommended)

---

## Installation Steps

### Step 1: Clone the Repository

Open your terminal or command prompt and run:

```bash
git clone https://github.com/UkuRenekKronbergs/TerraForge-Procedural-World-Generator.git
cd TerraForge-Procedural-World-Generator
```

### Step 2: Generate Project Files

Project files must be generated for your platform before opening the project.

#### Windows

1. Navigate to the project folder
2. Right-click on `TerraForge.uproject`
3. Select **"Generate Visual Studio project files"**
   - If this option doesn't appear, you may need to associate `.uproject` files with Unreal Engine
   - Alternative: Run `"C:\Program Files\Epic Games\UE_5.3\Engine\Build\BatchFiles\GenerateProjectFiles.bat" TerraForge.uproject`

#### Mac

1. Navigate to the project folder
2. Right-click on `TerraForge.uproject`
3. Select **"Generate Xcode Project"**
   - Alternative: Run from terminal:
     ```bash
     /Users/Shared/Epic\ Games/UE_5.3/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh -project="/path/to/TerraForge.uproject" -game
     ```

#### Linux

1. Open terminal in project folder
2. Run:
   ```bash
   ~/UnrealEngine/Engine/Build/BatchFiles/Linux/GenerateProjectFiles.sh -project="$(pwd)/TerraForge.uproject" -game
   ```

### Step 3: Build the Project

#### Windows

1. Open `TerraForge.sln` in Visual Studio 2022
2. Select build configuration:
   - **Development Editor** (for development)
   - **DebugGame Editor** (for debugging)
   - **Shipping** (for final builds)
3. Set the startup project to `TerraForge`
4. Build → Build Solution (or press Ctrl+Shift+B)
5. Wait for compilation to complete (5-15 minutes on first build)

**Common Build Issues**:
- If you get "missing SDK" errors, install Windows 10 SDK via Visual Studio Installer
- If you get "missing DLL" errors, ensure Unreal Engine is properly installed

#### Mac

1. Open `TerraForge.xcworkspace` in Xcode
2. Select scheme: `TerraForge` with target `My Mac`
3. Product → Build (or press Cmd+B)
4. Wait for compilation to complete

**Common Build Issues**:
- If you get "No provisioning profile" errors, you can ignore them for development
- If build fails, ensure Command Line Tools are installed: `xcode-select --install`

#### Linux

1. Open terminal in project folder
2. Run:
   ```bash
   make TerraForgeEditor
   ```
3. Wait for compilation to complete

**Common Build Issues**:
- If you get linking errors, ensure all dependencies are installed
- Install missing packages: `sudo apt-get install libxcb-xinerama0 libglu1-mesa-dev`

### Step 4: Open in Unreal Engine

#### Method 1: Double-click

Simply double-click `TerraForge.uproject` to open in Unreal Engine.

#### Method 2: Epic Games Launcher

1. Open Epic Games Launcher
2. Navigate to Library → My Projects
3. Click **"Add"** and browse to the project folder
4. Select `TerraForge.uproject`
5. Click **"Open"** to launch

### Step 5: First-time Setup in Unreal Editor

When the project opens for the first time:

1. **Wait for shaders to compile**
   - This can take 5-30 minutes depending on your system
   - Progress is shown in the bottom-right corner

2. **Verify the project loaded correctly**
   - Check the Content Browser has folders: Blueprints, Materials, Shaders
   - Check the Place Actors panel includes TerraForge actors

3. **Create a test level**
   - File → New Level → Empty Level
   - Save as `TestLevel` in Content/Maps/

---

## Quick Start After Installation

### Create Your First Procedural World

1. **Add Terrain**:
   - Open the Place Actors panel (Window → Place Actors)
   - Search for "ProceduralTerrainActor"
   - Drag into viewport
   - In Details panel, adjust:
     - Terrain Width: 100
     - Terrain Height: 100
     - Max Height: 2000
     - Noise Scale: 100
   - Click "Generate Terrain" button or enable "Auto Generate"

2. **Add Water**:
   - Search for "DynamicWaterActor" in Place Actors
   - Drag into viewport
   - Position at Z = 0 (or desired water level)
   - Adjust Water Width and Length to cover terrain

3. **Add Day/Night Cycle**:
   - Search for "DayNightCycleManager" in Place Actors
   - Drag into viewport (position doesn't matter)
   - Configure:
     - Time of Day: 12.0 (noon)
     - Cycle Speed: 1.0 (1 hour per real second)
     - Auto Progress: True

4. **Test in Play Mode**:
   - Click Play button (or press Alt+P)
   - Use WASD to move, Mouse to look
   - Press E/Q for vertical movement
   - Hold Shift to sprint

---

## Troubleshooting Setup Issues

### Project won't open

**Error: "Project is for a different version of Unreal Engine"**
- Solution: Right-click project file → Switch Unreal Engine version → Select installed version

**Error: "Missing modules" or "TerraForge could not be compiled"**
- Solution 1: Rebuild from IDE (Visual Studio/Xcode)
- Solution 2: Delete `Binaries`, `Intermediate`, and `Saved` folders, then regenerate project files
- Solution 3: Verify Unreal Engine installation in Epic Games Launcher

### Build fails with errors

**Windows - Visual Studio errors**:
- Ensure Visual Studio 2022 is installed with correct workloads
- Update Visual Studio to latest version
- Close Visual Studio, delete `.vs` folder, reopen and rebuild

**Mac - Xcode errors**:
- Update Xcode to latest version
- Install Command Line Tools: `xcode-select --install`
- Clean build folder: Product → Clean Build Folder

**Linux - Make errors**:
- Install development packages:
  ```bash
  sudo apt-get install build-essential clang libvulkan1 libvulkan-dev
  ```
- Check Unreal Engine is properly compiled

### Shaders won't compile

**Error: "Failed to compile shader"**
- Solution: Wait for all shaders to compile (check bottom-right corner)
- If stuck, close editor and delete `Saved/ShaderDebugInfo` folder
- Reopen project to recompile shaders

### Performance issues in editor

- Lower editor scalability: Settings → Engine Scalability Settings → Low
- Disable real-time rendering: Viewport → Realtime (toggle off)
- Close other applications to free memory
- Ensure graphics drivers are up to date

---

## Optional: Setting Up Source Control

### Using Git

Already set up! The repository includes a `.gitignore` configured for Unreal Engine projects.

**Recommended workflow**:
```bash
# Make changes
git add .
git commit -m "Description of changes"
git push origin main
```

**What's ignored** (automatically):
- Binaries/
- Intermediate/
- Saved/
- Build/
- .vs/
- *.sln

### Using Perforce

1. Set up Perforce server (P4V)
2. In Unreal Editor: Editor Preferences → Source Control
3. Select "Perforce" as provider
4. Enter server details
5. Click "Accept Settings"

---

## Next Steps

Once setup is complete:

1. **Read the [README.md](../README.md)** for feature overview
2. **Explore the [API Reference](API_REFERENCE.md)** for detailed class documentation
3. **Experiment with parameters** to create different terrain types
4. **Create custom materials** for terrain and water
5. **Extend the system** with your own features

---

## Getting Help

If you encounter issues not covered in this guide:

1. Check the [Troubleshooting section](#troubleshooting-setup-issues) above
2. Search existing [GitHub Issues](https://github.com/UkuRenekKronbergs/TerraForge-Procedural-World-Generator/issues)
3. Create a new issue with:
   - Your operating system and version
   - Unreal Engine version
   - Exact error message (with screenshot if possible)
   - Steps to reproduce the issue

---

**Welcome to TerraForge! Happy world building! 🌍**
