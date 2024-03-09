// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAIController.h"

#include "../ProjGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void ATestAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);


    UpdateTimer -= DeltaSeconds;
    if (UpdateTimer <= 0.0f)
    {
        UpdateTimer = 0.2f;


        target = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, target);
        

    }

}