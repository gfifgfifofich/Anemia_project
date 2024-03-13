// Copyright Epic Games, Inc. All Rights Reserved.

#include "Anemia_projectGameMode.h"
#include "Anemia_projectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAnemia_projectGameMode::AAnemia_projectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
