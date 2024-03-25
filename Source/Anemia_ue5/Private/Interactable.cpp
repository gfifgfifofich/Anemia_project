// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	cd -= DeltaTime;

}

void AInteractable::Interact()
{
	if (interacted && OneTime)
		return;

	interacted = true;

	if (cd <= 0.0f)
	{
		cd = CoolDown;
		OnInteractEvent();
	}
}