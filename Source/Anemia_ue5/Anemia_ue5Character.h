// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "VectorTypes.h"
#include "Anemia_ue5Character.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AAnemia_ue5Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;


	


public:
	AAnemia_ue5Character();
	


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Casts)
	FVector CastOnPawnPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Casts)
	FHitResult CastOnPawnHitResult;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Casts)
	bool CastOnPawnSuccess;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Casts)
	AActor* CastOnPawnActor;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Casts)
	FVector CastOnGroundPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Casts)
	FHitResult CastOnGroundHitResult;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Casts)
	bool CastOnGroundSuccess;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MovementSpeed = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
	float CameraSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
	float TargetFOV = 90.0f;

	float StartFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
	float ZoomSpeed = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	float CameraShakeAmount = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	float CameraShakeFallOff = 5.0f;
	
	FVector2f CameraShakeDir  = FVector2f();

	UFUNCTION( BlueprintCallable, Category = Input, DisplayName = "Shoot")
	void Shoot(float CoolDown = 1.0f, FVector2f RecoilDir = FVector2f(), float RecoilRandomness = 0.0f);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShootEvent();

protected:
	float ShootCD = 0.0f;


	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();
	
	virtual void Tick(float dt) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

