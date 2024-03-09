// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBall.h"

// Sets default values
AFireBall::AFireBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFireBall::BeginPlay()
{
	Super::BeginPlay();



}

// Called every frame
void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target != NULL)
	{

		FVector pos = GetActorLocation();
		FVector dif = Target->GetActorLocation() - pos;
		float dist = dif.Length();
		FVector vel;
		if(dist>0.1f)
			vel = (dif/dist) * velocity;

		SetActorLocation( pos + vel,true);

		if (dist < 10.0f)
		{
			OnCollision();
			Destroy();
		}
		

	}

}

