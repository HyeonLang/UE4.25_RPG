// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UE4_RPG : ModuleRules
{
	private string MySQL_Directory
    {
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../MySQL/")); }
    }

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
			"Paper2D",
			"Slate",
			"SlateCore",
			"OnlineSubsystem",
			"OnlineSubsystemSteam",
			"LevelSequence",
			"MovieScene",
			"CinematicCamera",
			"HTTP", 
			"JSON",
			"JsonUtilities", 
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[]
		{
			Path.Combine(MySQL_Directory, "include"),
			Path.Combine(MySQL_Directory, "include/mysqlx"),
			Path.Combine(MySQL_Directory, "include/mysqlx/common"),
			Path.Combine(MySQL_Directory, "include/mysqlx/devapi")
		});

		// MySQL 라이브러리 추가
		PublicAdditionalLibraries.AddRange(new string[] 
		{
			Path.Combine(MySQL_Directory, "lib64/vs14/mysqlcppconnx.lib"),	// 주요 동적 라이브러리 연결을 위한 import 라이브러리
			Path.Combine(MySQL_Directory, "lib64/vs14/libssl.lib"),			// OpenSSL 라이브러리
			Path.Combine(MySQL_Directory, "lib64/vs14/libcrypto.lib")		// OpenSSL 라이브러리
		});

		// MySQL DLL 복사
		RuntimeDependencies.Add(
			"$(BinaryOutputDir)/mysqlcppconn-10-vs14.dll", 
			Path.Combine(MySQL_Directory, "lib64/mysqlcppconn-10-vs14.dll"));

		// MySQL DLL 복사
		RuntimeDependencies.Add(
			"$(BinaryOutputDir)/mysqlcppconnx-2-vs14.dll",
			Path.Combine(MySQL_Directory, "lib64/mysqlcppconnx-2-vs14.dll"));

		// MySQL DLL 복사
		RuntimeDependencies.Add(
			"$(BinaryOutputDir)/libssl-3-x64.dll",
			Path.Combine(MySQL_Directory, "lib64/libssl-3-x64.dll"));

		// MySQL DLL 복사
		RuntimeDependencies.Add(
			"$(BinaryOutputDir)/libcrypto-3-x64.dll",
			Path.Combine(MySQL_Directory, "lib64/libcrypto-3-x64.dll"));

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
