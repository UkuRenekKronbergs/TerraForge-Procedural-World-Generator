// Copyright Epic Games, Inc. All Rights Reserved.

#include "TerraForge.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FTerraForgeModule"

void FTerraForgeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogTemp, Log, TEXT("TerraForge Module Started"));
}

void FTerraForgeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogTemp, Log, TEXT("TerraForge Module Shutdown"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_PRIMARY_GAME_MODULE( FTerraForgeModule, TerraForge, "TerraForge" );
