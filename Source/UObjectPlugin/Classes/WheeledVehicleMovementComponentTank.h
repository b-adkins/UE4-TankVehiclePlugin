//Copyright 2014-2015 Leon Rosengarten and Open House Studios, All Rights Reserved.

#pragma once

#include "Vehicles/WheeledVehicleMovementComponent.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "WheeledVehicleMovementComponentTank.generated.h"


UENUM(BlueprintType)
enum class VehicleDriveTankControlModel : uint8
{
	STANDARD = 0, //Left/Right thrust range [0,1]
	SPECIAL //Left/Right thrust range [-1,1]
};

UENUM(BlueprintType)
enum class VehicleDriveTankControlMethod : uint8
{
	SingleStick,
	DualStick,
};

/**
*
*/
UCLASS(Blueprintable)
class UWheeledVehicleMovementComponentTank : public UWheeledVehicleMovementComponent
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		void SetAcceleration(float Acceleration);

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		void SetLeftBreak(float LeftBreak);

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		void SetRightBreak(float RightBreak);

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		void SetLeftThrust(float LeftThrust);

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		void SetRightThrust(float RightThrust);

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		float GetAcceleration() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		float GetLeftBreak() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		float GetRightBreak() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		float GetLeftThrust() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		float GetRightThrust() const;

public:


	/** Engine */
	UPROPERTY(EditAnywhere, Category = MechanicalSetup)
		FVehicleEngineData EngineSetup;

	/** Transmission data */
	UPROPERTY(EditAnywhere, Category = MechanicalSetup)
		FVehicleTransmissionData TransmissionSetup;

	UPROPERTY(EditAnywhere, Category = "Tank Setup")
		uint32 NumOfWheels;

	UPROPERTY(EditAnywhere, Category = "Tank Setup")
		VehicleDriveTankControlModel TankControlModel;

	UPROPERTY(EditAnywhere, Category = "Tank Setup")
		VehicleDriveTankControlMethod TankControlMethod;

	UPROPERTY(EditAnywhere, Category = "Tank Setup")
		bool bUseDualBreak;

	/** Maximum steering versus forward speed (km/h) */
	UPROPERTY(EditAnywhere, Category = SteeringSetup)
		FRuntimeFloatCurve SteeringCurve;

	UPROPERTY(EditAnywhere, Category = TankInput, AdvancedDisplay)
		FVehicleInputRate LeftThrustRate;
	UPROPERTY(EditAnywhere, Category = TankInput, AdvancedDisplay)
		FVehicleInputRate RightThrustRate;
	UPROPERTY(EditAnywhere, Category = TankInput, AdvancedDisplay)
		FVehicleInputRate RightBrakeRate;
	UPROPERTY(EditAnywhere, Category = TankInput, AdvancedDisplay)
		FVehicleInputRate LeftBrakeRate;

	UPROPERTY(EditAnywhere, Category = TankSetup, AdvancedDisplay)
		FName TurretBoneName;

	UPROPERTY(EditAnywhere, Category = TankSetup, AdvancedDisplay)
		FName CannonBoneName;
	/*
	10.0f,	//fall rate eANALOG_INPUT_ACCEL=0
	10.0f,	//fall rate eANALOG_INPUT_BRAKE_LEFT
	10.0f,	//fall rate eANALOG_INPUT_BRAKE_RIGHT
	5.0f,	//fall rate eANALOG_INPUT_THRUST_LEFT
	5.0f	//fall rate eANALOG_INPUT_THRUST_RIGHT
	//6.0f,	//rise rate eANALOG_INPUT_ACCEL=0,
	//6.0f,	//rise rate eANALOG_INPUT_BRAKE,
	//6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE,
	//2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT,
	//2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT,
	*/
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Track Data")
		float RightTrackSpeed;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Track Data")
		float LeftTrackSpeed;

public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

#if WITH_VEHICLE

	virtual void SetupVehicle() override;

	virtual void SetupWheels(physx::PxVehicleWheelsSimData* PWheelsSimData) override;

	virtual void UpdateSimulation(float DeltaTime) override;

#endif

protected:


	UPROPERTY(Transient)
		float Acceleration;
	UPROPERTY(Transient)
		float LeftBreak;
	UPROPERTY(Transient)
		float RightBreak;
	UPROPERTY(Transient)
		float LeftThrust;
	UPROPERTY(Transient)
		float RightThrust;

private:



};
