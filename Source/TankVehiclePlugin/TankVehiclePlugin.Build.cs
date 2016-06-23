// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class TankVehiclePlugin : ModuleRules
	{
		public TankVehiclePlugin(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "UnrealEd",
                    "PhysX",
                    "APEX"
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"Developer/TankVehiclePlugin/Private",
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "UnrealEd",
                    "PhysX",
                    "APEX"
					// ... add other public dependencies that you statically link with here ...
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "UnrealEd",
                    "PhysX",
                    "APEX"
					// ... add private dependencies that you statically link with here ...
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}