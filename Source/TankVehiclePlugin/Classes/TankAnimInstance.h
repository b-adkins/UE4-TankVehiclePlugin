//Copyright 2014-2016 Leon Rosengarten and Open House Studios

#pragma once

#include "VehicleAnimInstance.h"
#include "TankAnimInstance.generated.h"

UCLASS(transient)
class UTankAnimInstance : public UVehicleAnimInstance
{

	GENERATED_UCLASS_BODY()

		/** Makes a montage jump to the end of a named section. */
		UFUNCTION(BlueprintCallable, Category = "Animation")
	class ATankVehicle * GetTank();

};
