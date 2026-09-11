using UnrealBuildTool;
using System.Collections.Generic;

public class AventuraUSFXEditorTarget : TargetRules
{
	public AventuraUSFXEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "AventuraUSFX" });
	}
}
