// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TestAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJ_API ATestAIController : public AAIController
{
	GENERATED_BODY()

	float UpdateTimer = 0.2f;

	FVector target;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
