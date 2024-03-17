// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Components/PrimitiveComponent.h"
inline NeuralNetwork* LastBest;


// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	prevrotat = GetActorRotation();
	StartPos = GetActorLocation();
	InitNN();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(t<=0.0f)
	{
		t=5.0f;
		ProcessLearning();
		SetupLearning();
		lastcost = 0.0f;
	}

	RotationVelocity = (GetActorRotation() - prevrotat)*DeltaTime;
	prevrotat = GetActorRotation();

	UStaticMeshComponent* StaticMesh = GetComponentByClass<UStaticMeshComponent>();

	float* inputs = new float[6];
	inputs[0] = GetActorLocation().Y - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Y;
	inputs[1] = GetActorLocation().Z - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Z;
	inputs[2] = GetActorLocation().X - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().X;
	inputs[3] = RotationVelocity.Yaw;
	inputs[4] = RotationVelocity.Roll;
	inputs[5] = RotationVelocity.Pitch;

	lastcost += (GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() -  GetActorLocation()).Length();

	nn->Run(inputs);

 	StaticMesh->AddTorqueInRadians(FVector(1000000.0f * (nn->outputs[0]*2.0f - 1.0f),1000000.0f * (nn->outputs[1]*2.0f - 1.0f),1000000.0f* (nn->outputs[2]*2.0f - 1.0f)));

	t -= DeltaTime;

	SetActorLocation(StartPos);
	delete[] inputs;
}
void AMyActor::InitNN()
{	
	if(nn==NULL)
		nn = new NeuralNetwork();

	int arch[3];
	arch[0] = 6;
	arch[1] = 30;
	arch[2] = 3;
	nn->Create(arch,3);
	nn->SetupLearing();

	if(LastBest == NULL)
		{
			LastBest = new NeuralNetwork();
			LastBest->Create(arch,3);
		}
}
void AMyActor::SetupLearning()
{
	LastBest->h = 0.5f;
	nn->h = 0.5f;
	LastBest->CopyInto(nn);
	nn->SetupLearing();
}
void AMyActor::ProcessLearning()
{
	if(LastBest->lastCost >=lastcost)
		{
			nn->CopyInto(LastBest);
			LastBest->lastCost = lastcost;
		}
	nn->ApplyLearing(lastcost);
}
void AMyActor::SaveNN()
{
	nn->SaveTo("TestBall.nn");

}
