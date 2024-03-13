// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireBall.generated.h"

UCLASS()
class ANEMIA_PROJECT_API AFireBall : public AActor
{
	GENERATED_BODY()
	
public:	

	UFUNCTION(BlueprintImplementableEvent)
	void OnCollision();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	AActor* Target = NULL;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float velocity = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float damage = 0.0f;
	// Sets default values for this actor's properties
	AFireBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
