#include "TankVehiclePluginPrivatePCH.h"

//Copyright 2014-2016 Leon Rosengarten and Open House Studios

//#include "EnginePrivate.h"
#include "TankAnimInstance.h"
#include "TankVehicle.h"
#include "AnimationRuntime.h"

/////////////////////////////////////////////////////
// UVehicleAnimInstance
/////////////////////////////////////////////////////

UTankAnimInstance::UTankAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ATankVehicle* UTankAnimInstance::GetTank()
{
	return Cast<ATankVehicle>(GetOwningActor());
}
