#include "TankVehiclePluginPrivatePCH.h"

//Copyright 2014-2016 Leon Rosengarten and Open House Studios

// #include "EnginePrivate.h"
#include "TankVehicle.h"
#include "WheeledVehicleMovementComponentTank.h"

ATankVehicle::ATankVehicle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UWheeledVehicleMovementComponentTank>(AWheeledVehicle::VehicleMovementComponentName))
{


}

// Called when the game starts or when spawned
void ATankVehicle::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATankVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATankVehicle::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{

	Super::SetupPlayerInputComponent(InputComponent);

}

void ATankVehicle::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATankVehicle::SetRightThrust(float RightThrust)
{
	UWheeledVehicleMovementComponentTank* TankMovmentComponent = Cast<UWheeledVehicleMovementComponentTank>(GetVehicleMovement());

	if (TankMovmentComponent)
	{
		TankMovmentComponent->SetRightThrust(RightThrust);
	}
}

void ATankVehicle::SetLeftThrust(float LeftThrust)
{
	UWheeledVehicleMovementComponentTank* TankMovmentComponent = Cast<UWheeledVehicleMovementComponentTank>(GetVehicleMovement());

	if (TankMovmentComponent)
	{
		TankMovmentComponent->SetLeftThrust(LeftThrust);
	}
}
