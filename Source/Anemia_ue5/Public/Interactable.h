// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class ANEMIA_UE5_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	float cd = 0.0f;
	bool interacted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactable)
	float CoolDown = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactable)
	bool OneTime = false;

	UFUNCTION(BlueprintCallable, Category = Interactable)
	void Interact();

	UFUNCTION(BlueprintImplementableEvent, Category = Interactable)
	void OnInteractEvent();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
