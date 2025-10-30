# Contributing to TerraForge

Thank you for your interest in contributing to TerraForge! This document provides guidelines and information for contributors.

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue with:

1. **Clear title** describing the issue
2. **Steps to reproduce** the bug
3. **Expected behavior** vs actual behavior
4. **Environment details**:
   - Operating system and version
   - Unreal Engine version
   - Hardware specifications (GPU, RAM)
5. **Screenshots or logs** if applicable
6. **Minimal reproduction case** if possible

### Suggesting Features

Feature requests are welcome! Please:

1. Check if the feature already exists or is planned
2. Describe the feature and its use case
3. Explain how it fits with the project goals
4. Consider implementation complexity
5. Be open to discussion and feedback

### Submitting Pull Requests

1. **Fork the repository**
2. **Create a feature branch** from `main`:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Make your changes** following the coding standards below
4. **Test your changes** thoroughly
5. **Commit with clear messages**:
   ```bash
   git commit -m "Add feature: description of what you added"
   ```
6. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```
7. **Open a Pull Request** with:
   - Clear description of changes
   - Reference to related issues
   - Screenshots/videos for visual changes
   - List of testing performed

## Development Setup

See [SETUP_GUIDE.md](Documentation/SETUP_GUIDE.md) for detailed setup instructions.

Quick setup:
```bash
git clone https://github.com/UkuRenekKronbergs/TerraForge-Procedural-World-Generator.git
cd TerraForge-Procedural-World-Generator
# Generate project files for your platform
# Build and open in Unreal Engine
```

## Coding Standards

### C++ Style Guide

We follow Unreal Engine's coding standards with some additions:

#### Naming Conventions

- **Classes**: PascalCase with prefix
  - Actors: `A` prefix (e.g., `AProceduralTerrainActor`)
  - Objects: `U` prefix (e.g., `UNoiseGenerator`)
  - Structs: `F` prefix (e.g., `FTerrainData`)
  - Enums: `E` prefix (e.g., `ENoiseType`)
  - Interfaces: `I` prefix (e.g., `ITerrainGenerator`)

- **Variables**:
  - Member variables: PascalCase (e.g., `TerrainWidth`)
  - Local variables: camelCase (e.g., `currentHeight`)
  - Boolean variables: `b` prefix (e.g., `bAutoGenerate`)
  - Pointers: no special prefix

- **Functions**: PascalCase (e.g., `GenerateTerrain()`)

- **Constants**: UPPER_SNAKE_CASE or PascalCase for complex types

#### Code Organization

```cpp
// Header file (.h)
#pragma once

#include "CoreMinimal.h"
#include "ParentClass.h"
#include "ClassName.generated.h"

/**
 * Brief class description
 * Detailed explanation of what the class does
 */
UCLASS()
class TERRAFORGE_API UClassName : public UParentClass
{
    GENERATED_BODY()

public:
    // Public methods
    UClassName();
    
    UFUNCTION(BlueprintCallable, Category = "TerraForge|Category")
    void PublicMethod();

protected:
    // Protected methods
    virtual void BeginPlay() override;

private:
    // Private methods
    void PrivateHelper();

public:
    // Public properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Category")
    float PublicProperty;

private:
    // Private properties
    float PrivateProperty;
};
```

#### Comments

- Use `/** */` for documentation comments on public API
- Use `//` for inline comments
- Document all public functions and properties
- Explain "why" not "what" in comments
- Keep comments up to date with code changes

#### Best Practices

- **RAII**: Use smart pointers (`TSharedPtr`, `TWeakPtr`, `TUniquePtr`) for non-UObject types
- **Const correctness**: Mark methods and parameters `const` when appropriate
- **Early returns**: Use early returns to reduce nesting
- **No magic numbers**: Use named constants or properties
- **Error handling**: Check pointers before use, validate input
- **Performance**: Avoid unnecessary allocations, prefer `Reserve()` for arrays

### Blueprint Considerations

When creating C++ classes for Blueprint use:

1. **Expose to Blueprint** with appropriate specifiers:
   ```cpp
   UCLASS(Blueprintable)  // Can create BP child classes
   UFUNCTION(BlueprintCallable)  // Can call from BP
   UPROPERTY(BlueprintReadWrite)  // Can read/write from BP
   ```

2. **Use proper categories**:
   ```cpp
   Category = "TerraForge|Terrain"
   ```

3. **Add tooltips** for editor clarity:
   ```cpp
   meta = (ToolTip = "Maximum height of terrain features")
   ```

4. **Set sensible ranges**:
   ```cpp
   meta = (ClampMin = "0.0", ClampMax = "10000.0")
   ```

## Testing Guidelines

### Before Submitting

- [ ] Code compiles without errors or warnings
- [ ] Test in Unreal Editor (Development configuration)
- [ ] Test basic functionality:
  - [ ] Terrain generates correctly
  - [ ] Water renders properly
  - [ ] Day/night cycle works
  - [ ] Camera controls are responsive
- [ ] Check performance (FPS in editor and packaged game)
- [ ] Test on your target platform(s) if possible
- [ ] Verify no crashes or memory leaks

### Test Cases

For new features, consider:

1. **Normal usage**: Feature works as expected
2. **Edge cases**: Extreme values, zero values, negative values
3. **Error handling**: Invalid input, null pointers
4. **Performance**: No significant FPS drops
5. **Integration**: Works with existing features

## Documentation

When adding new features:

1. **Update README.md** if it affects user-facing functionality
2. **Update API_REFERENCE.md** for new classes/methods
3. **Add code comments** for complex algorithms
4. **Include usage examples** in documentation
5. **Update SETUP_GUIDE.md** if build process changes

## Project Structure Guidelines

### Adding New Classes

Place files in appropriate directories:

```
Source/TerraForge/
├── Public/          # Header files (.h)
│   ├── Core/        # Core systems
│   ├── Terrain/     # Terrain-related
│   ├── Water/       # Water-related
│   └── ...
└── Private/         # Implementation files (.cpp)
    ├── Core/
    ├── Terrain/
    ├── Water/
    └── ...
```

### Adding Content

```
Content/
├── Blueprints/      # Blueprint classes
├── Materials/       # Materials and Material Instances
├── Textures/        # Texture assets
├── Meshes/          # Static meshes
└── Shaders/         # Custom shader files (.usf)
```

## Git Workflow

### Branch Naming

- Features: `feature/description`
- Bug fixes: `bugfix/description`
- Hotfixes: `hotfix/description`
- Documentation: `docs/description`

### Commit Messages

Follow conventional commits:

```
type(scope): subject

body (optional)

footer (optional)
```

**Types**:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting)
- `refactor`: Code refactoring
- `perf`: Performance improvements
- `test`: Adding tests
- `chore`: Build process, dependencies

**Examples**:
```
feat(terrain): add Voronoi noise algorithm
fix(water): correct wave normal calculation
docs(api): update NoiseGenerator documentation
perf(terrain): optimize mesh generation
```

### Pull Request Process

1. Ensure your PR:
   - Has a clear description
   - References related issues (#123)
   - Includes tests/validation
   - Updates documentation
   - Follows coding standards

2. Address review feedback promptly

3. Keep PR focused on one feature/fix

4. Squash commits if requested before merge

## Code Review Checklist

Reviewers should check:

- [ ] Code follows style guidelines
- [ ] No compiler warnings
- [ ] Appropriate error handling
- [ ] Performance considerations addressed
- [ ] Documentation updated
- [ ] No breaking changes (or properly documented)
- [ ] Memory management is correct
- [ ] Thread safety if applicable

## Community Guidelines

### Be Respectful

- Treat everyone with respect
- Be constructive in feedback
- Help newcomers
- Stay on topic in discussions
- No harassment or discrimination

### Communication Channels

- **GitHub Issues**: Bug reports, feature requests
- **Pull Requests**: Code contributions, reviews
- **Discussions**: General questions, ideas

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Questions?

If you have questions about contributing:

1. Check existing documentation
2. Search closed issues
3. Open a discussion or issue
4. Contact maintainers

Thank you for contributing to TerraForge! 🌍
