using UnrealBuildTool;
using System.Collections.Generic;

public class AventuraUSFXTarget : TargetRules
{
	public AventuraUSFXTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "AventuraUSFX" });
	}
}
