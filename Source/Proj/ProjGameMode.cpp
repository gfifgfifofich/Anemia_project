// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjGameMode.h"
#include "ProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjGameMode::AProjGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
