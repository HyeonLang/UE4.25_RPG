// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UE4_RPG : ModuleRules
{
	public UE4_RPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add(ModuleDirectory);
		//PublicIncludePaths.Add("C:/Program Files/MySQL/MySQL Server 8.0/include");

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"Paper2D"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// MySQL 라이브러리 추가
		//PublicAdditionalLibraries.Add("C:/Program Files/MySQL/MySQL Server 8.0/lib/libmysql.lib");

		// MySQL DLL 복사
		//RuntimeDependencies.Add("$(TargetOutputDir)/libmysql.dll", "C:/Program Files/MySQL/MySQL Server 8.0/lib/libmysql.dll");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
