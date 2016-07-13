//Copyright 2014-2016 Leon Rosengarten and Open House Studios

#pragma once

#include "GameFramework/WheeledVehicle.h"
#include "TankVehicle.generated.h"

UCLASS()
class ATankVehicle : public AWheeledVehicle
{
	GENERATED_UCLASS_BODY()

public:
	// Sets default values for this pawn's properties
	//ATankPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void PostInitializeComponents() override;

public:

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		void SetLeftThrust(float LeftThrust);

	UFUNCTION(BlueprintCallable, Category = "Game|Components|TankVehicleMovement")
		void SetRightThrust(float RightThrust);
};
