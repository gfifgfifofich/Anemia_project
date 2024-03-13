// Copyright Epic Games, Inc. All Rights Reserved.

#include "Anemia_projectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AAnemia_projectCharacter

AAnemia_projectCharacter::AAnemia_projectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	StartFOV = FollowCamera->FieldOfView;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AAnemia_projectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AAnemia_projectCharacter::Tick(float dt)
{
	Super::Tick(dt);

	ShootCD -=dt;


	AddControllerYawInput(-CameraShakeDir.X) ;
	AddControllerPitchInput(-CameraShakeDir.Y);

	CameraShakeDir.X = FMath::RandRange(-1.0f,1.0f) * CameraShakeAmount;
	CameraShakeDir.Y = FMath::RandRange(-1.0f,1.0f) * CameraShakeAmount;

	AddControllerYawInput(CameraShakeDir.X) ;
	AddControllerPitchInput(CameraShakeDir.Y);

	CameraShakeAmount -=CameraShakeFallOff * dt;
	if(CameraShakeAmount<0.0f)
		CameraShakeAmount = 0.0f;
	FollowCamera->FieldOfView = FollowCamera->FieldOfView + (TargetFOV - FollowCamera->FieldOfView)*dt * ZoomSpeed;

} 
//////////////////////////////////////////////////////////////////////////
// Input

void AAnemia_projectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAnemia_projectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAnemia_projectCharacter::Look);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAnemia_projectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y*MovementSpeed);
		AddMovementInput(RightDirection, MovementVector.X*MovementSpeed);
	}
}

void AAnemia_projectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * CameraSpeed) ;
		AddControllerPitchInput(LookAxisVector.Y * CameraSpeed);
	}
}

void AAnemia_projectCharacter::Shoot(float CoolDown, FVector2f RecoilDir, float RecoilRandomness)
{
	if(ShootCD<=0.0f)
	{
		//CameraShakeAmount = 1.5f;
		ShootCD = CoolDown;
		APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

		CastOnGroundSuccess = GetWorld()->LineTraceSingleByChannel(CastOnGroundHitResult, camManager->GetCameraLocation() , camManager->GetCameraLocation() +GetFollowCamera()->GetForwardVector()*5000.0f  , ECollisionChannel::ECC_Visibility);

		if(CastOnGroundSuccess)
			CastOnGroundPosition = CastOnGroundHitResult.Location;

		CastOnPawnSuccess = GetWorld()->LineTraceSingleByChannel(CastOnPawnHitResult, camManager->GetCameraLocation() , camManager->GetCameraLocation() +GetFollowCamera()->GetForwardVector()*5000.0f  , ECollisionChannel::ECC_Pawn);

		if(CastOnPawnSuccess)
		{
			CastOnPawnPosition = CastOnPawnHitResult.Location;
			CastOnPawnActor = CastOnPawnHitResult.GetActor();
		}


		AddControllerYawInput(FMath::RandRange(-1.0f,1.0f) * RecoilRandomness + RecoilDir.X) ;
		AddControllerPitchInput(FMath::RandRange(-1.0f,1.0f) * RecoilRandomness + RecoilDir.Y);

		OnShootEvent();
	}
}