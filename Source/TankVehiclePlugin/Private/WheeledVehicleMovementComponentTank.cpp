// Fill out your copyright notice in the Description page of Project Settings.

#include "UObjectPluginPrivatePCH.h"
//Copyright 2014-2015 Leon Rosengarten and Open House Studios, All Rights Reserved.

//   #include "Runtime/Engine/Private/EnginePrivate.h"
#include "WheeledVehicleMovementComponentTank.h"
#include "PhysicsPublic.h"

// @third party code - BEGIN PhysX

#define WITH_VEHICLE 1
#define WITH_EDITOR 1

#if WITH_VEHICLE
#include "PhysXIncludes.h"
#endif // WITH_VEHICLE



// @third party code - END PhysX


UWheeledVehicleMovementComponentTank::UWheeledVehicleMovementComponentTank(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_VEHICLE

	RightThrustRate.FallRate = 5.f;
	RightThrustRate.RiseRate = 2.5f;
	LeftThrustRate.FallRate = 5.f;
	LeftThrustRate.RiseRate = 2.5f;
	LeftBrakeRate.RiseRate = 6.f;
	LeftBrakeRate.FallRate = 10.f;
	RightBrakeRate.RiseRate = 6.f;
	RightBrakeRate.FallRate = 10.f;

	TankControlMethod = VehicleDriveTankControlMethod::SingleStick;

	TankControlModel = VehicleDriveTankControlModel::STANDARD;


	PxVehicleEngineData DefEngineData;
	EngineSetup.MOI = DefEngineData.mMOI;
	EngineSetup.MaxRPM = OmegaToRPM(DefEngineData.mMaxOmega);
	EngineSetup.DampingRateFullThrottle = DefEngineData.mDampingRateFullThrottle;
	EngineSetup.DampingRateZeroThrottleClutchEngaged = DefEngineData.mDampingRateZeroThrottleClutchEngaged;
	EngineSetup.DampingRateZeroThrottleClutchDisengaged = DefEngineData.mDampingRateZeroThrottleClutchDisengaged;

	// Convert from PhysX curve to ours
	FRichCurve* TorqueCurveData = EngineSetup.TorqueCurve.GetRichCurve();
	for (PxU32 KeyIdx = 0; KeyIdx < DefEngineData.mTorqueCurve.getNbDataPairs(); KeyIdx++)
	{
		float Input = DefEngineData.mTorqueCurve.getX(KeyIdx) * EngineSetup.MaxRPM;
		float Output = DefEngineData.mTorqueCurve.getY(KeyIdx) * DefEngineData.mPeakTorque;
		TorqueCurveData->AddKey(Input, Output);
	}

	PxVehicleClutchData DefClutchData;
	TransmissionSetup.ClutchStrength = DefClutchData.mStrength;

	PxVehicleGearsData DefGearSetup;
	TransmissionSetup.GearSwitchTime = DefGearSetup.mSwitchTime;
	TransmissionSetup.ReverseGearRatio = DefGearSetup.mRatios[PxVehicleGearsData::eREVERSE];
	TransmissionSetup.FinalRatio = DefGearSetup.mFinalRatio;

	PxVehicleAutoBoxData DefAutoBoxSetup;
	TransmissionSetup.NeutralGearUpRatio = DefAutoBoxSetup.mUpRatios[PxVehicleGearsData::eNEUTRAL];
	TransmissionSetup.GearAutoBoxLatency = DefAutoBoxSetup.getLatency();
	TransmissionSetup.bUseGearAutoBox = true;

	for (uint32 i = PxVehicleGearsData::eFIRST; i < DefGearSetup.mNbRatios; i++)
	{
		FVehicleGearData GearData;
		GearData.DownRatio = DefAutoBoxSetup.mDownRatios[i];
		GearData.UpRatio = DefAutoBoxSetup.mUpRatios[i];
		GearData.Ratio = DefGearSetup.mRatios[i];
		TransmissionSetup.ForwardGears.Add(GearData);
	}

	FRichCurve* SteeringCurveData = SteeringCurve.GetRichCurve();
	SteeringCurveData->AddKey(0.f, 1.f);
	SteeringCurveData->AddKey(20.f, 0.9f);
	SteeringCurveData->AddKey(60.f, 0.8f);
	SteeringCurveData->AddKey(120.f, 0.7f);

	// Initialize WheelSetups array with 4 wheels
	WheelSetups.SetNum(4);
#endif
}



float FVehicleEngineData::FindPeakTorque() const
{
// Find max torque
float PeakTorque = 0.f;
TArray<FRichCurveKey> TorqueKeys = TorqueCurve.GetRichCurveConst()->GetCopyOfKeys();
for (int32 KeyIdx = 0; KeyIdx < TorqueKeys.Num(); KeyIdx++)
{
FRichCurveKey& Key = TorqueKeys[KeyIdx];
PeakTorque = FMath::Max(PeakTorque, Key.Value);
}
return PeakTorque;
}

#if WITH_VEHICLE
static void GetTankVehicleEngineSetup(const FVehicleEngineData& Setup, PxVehicleEngineData& PxSetup)
{
	PxSetup.mMOI = M2ToCm2(Setup.MOI);
	PxSetup.mMaxOmega = RPMToOmega(Setup.MaxRPM);
	PxSetup.mDampingRateFullThrottle = M2ToCm2(Setup.DampingRateFullThrottle);
	PxSetup.mDampingRateZeroThrottleClutchEngaged = M2ToCm2(Setup.DampingRateZeroThrottleClutchEngaged);
	PxSetup.mDampingRateZeroThrottleClutchDisengaged = M2ToCm2(Setup.DampingRateZeroThrottleClutchDisengaged);

	float PeakTorque = Setup.FindPeakTorque(); // In Nm
	PxSetup.mPeakTorque = M2ToCm2(PeakTorque);	// convert Nm to (kg cm^2/s^2)

												// Convert from our curve to PhysX
	PxSetup.mTorqueCurve.clear();
	TArray<FRichCurveKey> TorqueKeys = Setup.TorqueCurve.GetRichCurveConst()->GetCopyOfKeys();
	int32 NumTorqueCurveKeys = FMath::Min<int32>(TorqueKeys.Num(), PxVehicleEngineData::eMAX_NB_ENGINE_TORQUE_CURVE_ENTRIES);
	for (int32 KeyIdx = 0; KeyIdx < NumTorqueCurveKeys; KeyIdx++)
	{
		FRichCurveKey& Key = TorqueKeys[KeyIdx];
		PxSetup.mTorqueCurve.addPair(FMath::Clamp(Key.Time / Setup.MaxRPM, 0.f, 1.f), Key.Value / PeakTorque); // Normalize torque to 0-1 range
	}
}

static void GetTankVehicleGearSetup(const FVehicleTransmissionData& Setup, PxVehicleGearsData& PxSetup)
{
	PxSetup.mSwitchTime = Setup.GearSwitchTime;
	PxSetup.mRatios[PxVehicleGearsData::eREVERSE] = Setup.ReverseGearRatio;
	for (int32 i = 0; i < Setup.ForwardGears.Num(); i++)
	{
		PxSetup.mRatios[i + PxVehicleGearsData::eFIRST] = Setup.ForwardGears[i].Ratio;
	}
	PxSetup.mFinalRatio = Setup.FinalRatio;
	PxSetup.mNbRatios = Setup.ForwardGears.Num() + PxVehicleGearsData::eFIRST;
}

static void GetTankVehicleAutoBoxSetup(const FVehicleTransmissionData& Setup, PxVehicleAutoBoxData& PxSetup)
{
	for (int32 i = 0; i < Setup.ForwardGears.Num(); i++)
	{
		const FVehicleGearData& GearData = Setup.ForwardGears[i];
		PxSetup.mUpRatios[i] = GearData.UpRatio;
		PxSetup.mDownRatios[i] = GearData.DownRatio;
	}
	PxSetup.mUpRatios[PxVehicleGearsData::eNEUTRAL] = Setup.NeutralGearUpRatio;
	PxSetup.setLatency(Setup.GearAutoBoxLatency);
}

void SetupTankDriveHelper(const UWheeledVehicleMovementComponentTank* VehicleData, const PxVehicleWheelsSimData* PWheelsSimData, PxVehicleDriveSimData4W& DriveData, uint32 NumOfWheels)
{

	//GetVehicleDifferential4WSetup(VehicleData->DifferentialSetup, DifferentialSetup);

	//DriveData.setDiffData(DifferentialSetup);
	//DriveData.

	PxVehicleEngineData EngineSetup;
	GetTankVehicleEngineSetup(VehicleData->EngineSetup, EngineSetup);
	DriveData.setEngineData(EngineSetup);

	PxVehicleClutchData ClutchSetup;
	ClutchSetup.mStrength = M2ToCm2(VehicleData->TransmissionSetup.ClutchStrength);
	DriveData.setClutchData(ClutchSetup);

	PxVehicleGearsData GearSetup;
	GetTankVehicleGearSetup(VehicleData->TransmissionSetup, GearSetup);
	DriveData.setGearsData(GearSetup);

	PxVehicleAutoBoxData AutoBoxSetup;
	GetTankVehicleAutoBoxSetup(VehicleData->TransmissionSetup, AutoBoxSetup);
	DriveData.setAutoBoxData(AutoBoxSetup);

}

#endif

void UWheeledVehicleMovementComponentTank::SetupVehicle()
{


#if WITH_VEHICLE
	if (!UpdatedPrimitive)
	{
		return;
	}

	if (WheelSetups.Num() % 2 != 0 && WheelSetups.Num() > 20)
	{
		PVehicle = NULL;
		PVehicleDrive = NULL;
		return;
	}

	for (int32 WheelIdx = 0; WheelIdx < WheelSetups.Num(); ++WheelIdx)
	{
		const FWheelSetup& WheelSetup = WheelSetups[WheelIdx];
		if (WheelSetup.BoneName == NAME_None)
		{
			return;
		}
	}

	// Setup the chassis and wheel shapes
	SetupVehicleShapes();

	// Setup mass properties
	SetupVehicleMass();

	// Setup the wheels
	PxVehicleWheelsSimData* PWheelsSimData = PxVehicleWheelsSimData::allocate(WheelSetups.Num());

	SetupWheels(PWheelsSimData);

	// Setup drive data
	PxVehicleDriveSimData4W DriveData;
	SetupTankDriveHelper(this, PWheelsSimData, DriveData, WheelSetups.Num());

	// Create the vehicle
	PxVehicleDriveTank* PVehicleDriveTank = PxVehicleDriveTank::allocate(WheelSetups.Num());
	check(PVehicleDriveTank);

	PVehicleDriveTank->setup(GPhysXSDK, UpdatedPrimitive->GetBodyInstance()->GetPxRigidDynamic(), *PWheelsSimData, DriveData, WheelSetups.Num());
	PVehicleDriveTank->setToRestState();

	// cleanup
	PWheelsSimData->free();
	PWheelsSimData = NULL;

	// cache values
	PVehicle = PVehicleDriveTank;
	PVehicleDrive = PVehicleDriveTank;

	SetUseAutoGears(TransmissionSetup.bUseGearAutoBox);
#endif

}

void UWheeledVehicleMovementComponentTank::SetupWheels(physx::PxVehicleWheelsSimData* PWheelsSimData)
{
#if WITH_VEHICLE
	Super::SetupWheels(PWheelsSimData);
#endif
}

void UWheeledVehicleMovementComponentTank::UpdateSimulation(float DeltaTime)
{

	if (PVehicleDrive == NULL)
		return;
#if WITH_VEHICLE

	PxVehicleDriveTankRawInputData VehicleInputData((PxVehicleDriveTankControlModel::Enum)TankControlModel);




	VehicleInputData.setAnalogAccel(ThrottleInput);
	switch (TankControlModel)
	{
	case VehicleDriveTankControlModel::STANDARD:

		switch (TankControlMethod)
		{
		case VehicleDriveTankControlMethod::SingleStick:
		{

			/*

			Get X and Y from the Joystick, do whatever scaling and calibrating you need to do based on your hardware.
			Invert X
			Calculate R+L (Call it V): V =(100-ABS(X)) * (Y/100) + Y
			Calculate R-L (Call it W): W= (100-ABS(Y)) * (X/100) + X
			Calculate R (thrust): R = (V+W) /2
			Calculate L (thrust): L= (V-W)/2

			*/
			float scaledSteering = SteeringInput * 100;
			float scaledThrottle = ThrottleInput * 100;
			float invSteering = -scaledSteering;
			float v = (100 - FMath::Abs(invSteering)) * (scaledThrottle / 100) + scaledThrottle;
			float w = (100 - FMath::Abs(scaledThrottle))* (invSteering / 100) + invSteering;
			float r = (v + w) / 2.f;
			float l = (v - w) / 2.f;


			VehicleInputData.setAnalogLeftThrust(l / 100.f);
			VehicleInputData.setAnalogRightThrust(r / 100.f);
			VehicleInputData.setAnalogLeftBrake(BrakeInput);
			VehicleInputData.setAnalogRightBrake(BrakeInput);
		}
		break;
		case VehicleDriveTankControlMethod::DualStick:

			VehicleInputData.setAnalogLeftThrust(LeftThrust);
			VehicleInputData.setAnalogRightThrust(RightThrust);
			VehicleInputData.setAnalogLeftBrake(BrakeInput);
			VehicleInputData.setAnalogRightBrake(BrakeInput);

			break;
		default:
			break;
		}

		break;
	case VehicleDriveTankControlModel::SPECIAL:

		if (bUseDualBreak)
		{

			//TODO: implament

		}
		else
		{

			VehicleInputData.setAnalogLeftThrust(LeftThrust);
			VehicleInputData.setAnalogRightThrust(RightThrust);
			VehicleInputData.setAnalogLeftBrake(BrakeInput);
			VehicleInputData.setAnalogRightBrake(BrakeInput);

		}


		break;
	default:
		break;
	}


	if (!PVehicleDrive->mDriveDynData.getUseAutoGears())
	{
		VehicleInputData.setGearUp(bRawGearUpInput);
		VehicleInputData.setGearDown(bRawGearDownInput);
	}

	// Convert from our curve to PxFixedSizeLookupTable

	PxVehiclePadSmoothingData SmoothData = {
		{ ThrottleInputRate.RiseRate, LeftBrakeRate.RiseRate, RightBrakeRate.RiseRate, LeftThrustRate.RiseRate, RightThrustRate.RiseRate },
		{ ThrottleInputRate.FallRate, LeftBrakeRate.FallRate, RightBrakeRate.FallRate, LeftThrustRate.FallRate, RightThrustRate.FallRate }
	};

	PxVehicleDriveTank* PVehicleDriveTank = (PxVehicleDriveTank*)PVehicleDrive;
	PxVehicleDriveTankSmoothAnalogRawInputsAndSetAnalogInputs(SmoothData, VehicleInputData, DeltaTime, *PVehicleDriveTank);

	LeftTrackSpeed = PVehicle->mWheelsDynData.getWheelRotationSpeed(0) / 1000.f;
	RightTrackSpeed = PVehicle->mWheelsDynData.getWheelRotationSpeed(1) / 1000.f;
	//PVehicle->mWheelsDynData.
	//PxVehicleDrive4WSmoothAnalogRawInputsAndSetAnalogInputs(SmoothData, SpeedSteerLookup, RawInputData, DeltaTime, false, *PVehicleDrive4W);
#endif
}


#if WITH_EDITOR
void UWheeledVehicleMovementComponentTank::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == TEXT("DownRatio"))
	{
		for (int32 GearIdx = 0; GearIdx < TransmissionSetup.ForwardGears.Num(); ++GearIdx)
		{
			FVehicleGearData & GearData = TransmissionSetup.ForwardGears[GearIdx];
			GearData.DownRatio = FMath::Min(GearData.DownRatio, GearData.UpRatio);
		}
	}
	else if (PropertyName == TEXT("UpRatio"))
	{
		for (int32 GearIdx = 0; GearIdx < TransmissionSetup.ForwardGears.Num(); ++GearIdx)
		{
			FVehicleGearData & GearData = TransmissionSetup.ForwardGears[GearIdx];
			GearData.UpRatio = FMath::Max(GearData.DownRatio, GearData.UpRatio);
		}
	}
	else if (PropertyName == TEXT("SteeringCurve"))
	{
		//make sure values are capped between 0 and 1
		TArray<FRichCurveKey> SteerKeys = SteeringCurve.GetRichCurve()->GetCopyOfKeys();
		for (int32 KeyIdx = 0; KeyIdx < SteerKeys.Num(); ++KeyIdx)
		{
			float NewValue = FMath::Clamp(SteerKeys[KeyIdx].Value, 0.f, 1.f);
			SteeringCurve.GetRichCurve()->UpdateOrAddKey(SteerKeys[KeyIdx].Time, NewValue);
		}
	}

}

#endif

void UWheeledVehicleMovementComponentTank::SetAcceleration(float OtherAcceleration)
{
#if WITH_VEHICLE
	this->Acceleration = OtherAcceleration;
#else
	return;
#endif
}

void UWheeledVehicleMovementComponentTank::SetLeftBreak(float OtherLeftBreak)
{
#if WITH_VEHICLE
	this->LeftBreak = OtherLeftBreak;
#else
	return;
#endif
}

void UWheeledVehicleMovementComponentTank::SetRightBreak(float OtherRightBreak)
{
#if WITH_VEHICLE
	this->RightBreak = OtherRightBreak;
#else
	return;
#endif
}

void UWheeledVehicleMovementComponentTank::SetLeftThrust(float OtherLeftThrust)
{
#if WITH_VEHICLE
	this->LeftThrust = OtherLeftThrust;
#else
	return;
#endif
}

void UWheeledVehicleMovementComponentTank::SetRightThrust(float OtherRightThrust)
{
#if WITH_VEHICLE
	this->RightThrust = OtherRightThrust;
#else
	return;
#endif
}

float UWheeledVehicleMovementComponentTank::GetAcceleration() const
{
#if WITH_VEHICLE
	return Acceleration;
#else
	return 0.f;
#endif
}

float UWheeledVehicleMovementComponentTank::GetLeftBreak() const
{
#if WITH_VEHICLE
	return LeftBreak;
#else
	return 0.f;
#endif
}

float UWheeledVehicleMovementComponentTank::GetRightBreak() const
{
#if WITH_VEHICLE
	return RightBreak;
#else
	return 0.f;
#endif
}

float UWheeledVehicleMovementComponentTank::GetLeftThrust() const
{
#if WITH_VEHICLE
	return LeftThrust;
#else
	return 0.f;
#endif
}

float UWheeledVehicleMovementComponentTank::GetRightThrust() const
{
#if WITH_VEHICLE
	return RightThrust;
#else
	return 0.f;
#endif
}


