// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyPluginObject.generated.h"


/**
 * Example UStruct declared in a plugin module
 */
USTRUCT()
struct FMyPluginStruct
{
	GENERATED_USTRUCT_BODY()
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	FString TestString;
};
 

/**
 * Example of declaring a UObject in a plugin module
 */
UCLASS(Blueprintable, Category = "Test")
class UMyPluginObject : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Test")
	void hello();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	FMyPluginStruct MyStruct;
};

//void Hello();
