// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeuralNetwork.h"
#include "MyActor.generated.h"


UCLASS()
class ANEMIA_UE5_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	NeuralNetwork* nn;
	// Sets default values for this actor's properties
	AMyActor();
	
	UFUNCTION( BlueprintCallable, Category = nn)
	void InitNN();
	UFUNCTION( BlueprintCallable, Category = nn)
	void SetupLearning();
	UFUNCTION( BlueprintCallable, Category = nn)
	void ProcessLearning();
	UFUNCTION( BlueprintCallable, Category = nn)
	void SaveNN();
	FVector StartPos;
	FRotator prevrotat;
	FRotator RotationVelocity;
	float lastcost = 0.0f;
	float t = 5.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
