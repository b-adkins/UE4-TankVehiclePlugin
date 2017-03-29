//Copyright 2014-2016 Leon Rosengarten and Open House Studios

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


			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "UnrealEd",
                    "PhysX",
                    "PhysXVehicles",
                    "PhysXVehicleLib",
                    "APEX"
					// ... add private dependencies that you statically link with here ...
				}
				);
           // PublicDependencyModuleNames.AddRange(new string[] { "PhysX", "PhysXVehicleLib","PhysXVehicles" });
            DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...

                }
				);
		}
	}
}