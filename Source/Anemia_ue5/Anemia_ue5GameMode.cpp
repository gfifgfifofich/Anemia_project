// Copyright Epic Games, Inc. All Rights Reserved.

#include "Anemia_ue5GameMode.h"
#include "Anemia_ue5Character.h"
#include "UObject/ConstructorHelpers.h"

AAnemia_ue5GameMode::AAnemia_ue5GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
