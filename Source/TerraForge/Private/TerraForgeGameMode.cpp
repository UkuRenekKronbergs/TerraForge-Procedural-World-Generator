// TerraForge - Procedural World Generator
// Game Mode Implementation

#include "TerraForgeGameMode.h"
#include "FreeCameraPawn.h"

ATerraForgeGameMode::ATerraForgeGameMode()
{
	// Set default pawn class to our free camera pawn
	DefaultPawnClass = AFreeCameraPawn::StaticClass();
}
