
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WPlayerControllerInterface.h"
#include "WCoreTypes.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Abilities/WAbility.h"
/////////////////////////////////////////
#include "WPlayerController.generated.h"
/////////////////////////////////////////

#define REGISTER_INPUT_ACTION(EnhancedInputComponent,InputAction,TriggerEvent)\
if(InputAction.IsNull()){WHIPLASH_LOG(LogWhiplash,Error,TEXT("input action property is null!"));}\
else{EnhancedInputComponent->BindAction(InputAction.LoadSynchronous(),TriggerEvent,this,&AWPlayerController::On##InputAction);}

UCLASS()
class WHIPLASH_API AWPlayerController : public APlayerController,public IWPlayerControllerInterface
{
	GENERATED_BODY()
	friend class UWAbilityComponent;
	friend class UWAbility;
public:
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void Crouch();
	
	
protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	UPROPERTY(VisibleInstanceOnly,BlueprintReadOnly,Category="Character")
	TObjectPtr<class AWCharacter> WhiplashCharacter;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	int32 InputMappingPriority = 0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Input")
	EAnalogMovementBehavior MovementStickMode = EAnalogMovementBehavior::FixedSpeedSingleGait;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Input")
	float AnalogWalkRunThreshold = 0.7;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> MoveInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> MoveWorldSpaceInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> LookInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> LookGamepadInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> SprintInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> WalkInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> JumpInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> CrouchInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> StrafeInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> PerspectiveInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input|Action")
	TSoftObjectPtr<UInputAction> AimInputAction;
	
	/* This function normalizes the input scale when the movement stick mode is set to fixed speed.
	 * This means any analog input from a gamepad will be converted to max input, similar to the behavior of keyboard movement.
	 *Improvements will be made to better support variable speeds in the future.*/
	UFUNCTION(BlueprintPure, Category="Input")
	virtual FVector2D GetMovementInputScaleValue(const FVector2D& Input) const;
	UFUNCTION(BlueprintCallable, Category="Input|Action")
	virtual FVector2D GetMoveInputActionValue() const;
	UFUNCTION(BlueprintCallable, Category="Input|Action")
	virtual FVector2D GetMoveWorldSpaceInputActionValue() const;
	
	UFUNCTION()
	virtual void OnMoveInputAction(const FInputActionInstance& Instance);
	UFUNCTION()
	virtual void OnMoveWorldSpaceInputAction(const FInputActionInstance& Instance);
	UFUNCTION()
	virtual void OnLookInputAction(const FInputActionInstance& Instance);
	UFUNCTION()
	virtual void OnLookGamepadInputAction(const FInputActionInstance& Instance);
	UFUNCTION()
	virtual void OnSprintInputAction(const FInputActionInstance& Instance);
	UFUNCTION()
	virtual void OnWalkInputAction(const FInputActionInstance& Instance);
	UFUNCTION()
	virtual void OnJumpInputAction(const FInputActionInstance& Instance);
	UFUNCTION()
	virtual void OnCrouchInputAction(const FInputActionInstance& Instance);
	UFUNCTION()
	virtual void OnStrafeInputAction(const FInputActionInstance& Instance);
	UFUNCTION()
	virtual void OnPerspectiveInputAction(const FInputActionInstance& Instance);
	UFUNCTION()
	virtual void OnAimInputAction(const FInputActionInstance& Instance);
	

public:
	virtual bool IsFullMovementInput_Implementation() const;
	
	FORCEINLINE virtual AWCharacter* GetWhiplashCharacter() const {return WhiplashCharacter;}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};
