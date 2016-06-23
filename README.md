# UE4-TankVehiclePlugin
Blueprintable Unreal Engine 4 classes along the lines of WheeledVehicle, except for tanks! (Or [skid steer vehicles](https://en.wikipedia.org/wiki/Skid-steer_loader))

## Authors
- [Leon Rosengarten](https://github.com/lion03) (code)
- Boone Adkins (packaged into plugin)

# Usage
Just like a ```WheeledVehicle```, except with Tank- classes.

# Technical Details
PhysX, which powers Unreal engine's physics, has additional vehicle movement components that are not included with stock UE4. This particular plugin wraps the ```WheeledVehicleMovementComponent4W```. See the [PhysX documentation on vehicles](docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Vehicles.html).
