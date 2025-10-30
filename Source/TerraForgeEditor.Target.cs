using UnrealBuildTool;
using System.Collections.Generic;

public class TerraForgeEditorTarget : TargetRules
{
	public TerraForgeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.AddRange( new string[] { "TerraForge" } );
	}
}
