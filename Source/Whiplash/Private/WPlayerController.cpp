#include "WPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "WCharacter.h"
#include "Attributes/WAttributeSet.h"
#include "Component/WAbilityComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/NavMovementComponent.h"
#include "Materials/MaterialExpressionLocalPosition.h"
#include "Tags/WGameplayTags.h"



void AWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);
	
	REGISTER_INPUT_ACTION(EnhancedInputComponent,MoveInputAction,ETriggerEvent::Triggered);
	REGISTER_INPUT_ACTION(EnhancedInputComponent,MoveWorldSpaceInputAction,ETriggerEvent::Triggered);
	REGISTER_INPUT_ACTION(EnhancedInputComponent,LookInputAction,ETriggerEvent::Triggered);
	REGISTER_INPUT_ACTION(EnhancedInputComponent,LookGamepadInputAction,ETriggerEvent::Triggered);
	REGISTER_INPUT_ACTION(EnhancedInputComponent,SprintInputAction,ETriggerEvent::Started);
	REGISTER_INPUT_ACTION(EnhancedInputComponent,WalkInputAction,ETriggerEvent::Triggered);
	REGISTER_INPUT_ACTION(EnhancedInputComponent,JumpInputAction,ETriggerEvent::Triggered);
	REGISTER_INPUT_ACTION(EnhancedInputComponent, CrouchInputActionTriggered, ETriggerEvent::Started);

	REGISTER_INPUT_ACTION(EnhancedInputComponent,StrafeInputAction,ETriggerEvent::Triggered);
	REGISTER_INPUT_ACTION(EnhancedInputComponent,PerspectiveInputAction,ETriggerEvent::Triggered);
	REGISTER_INPUT_ACTION(EnhancedInputComponent,AimInputAction,ETriggerEvent::Triggered);
	
}

void AWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InputMapping.IsNull())
	{
		WHIPLASH_LOG(LogWhiplash,Error,TEXT("the input mapping is null!"));
		return;
	}
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->AddMappingContext(InputMapping.LoadSynchronous(),InputMappingPriority);
	
	WhiplashCharacter = Cast<AWCharacter>(InPawn);
}

FVector2D AWPlayerController::GetMovementInputScaleValue(const FVector2D& Input) const
{
	switch (MovementStickMode)
	{
	default:
	case EAnalogMovementBehavior::FixedSpeedSingleGait:
	case EAnalogMovementBehavior::FixedSpeedWalkRun: return Input.GetSafeNormal();
	case EAnalogMovementBehavior::VariableSpeedSingleGait:
	case EAnalogMovementBehavior::VariableSpeedWalkRun: return Input.GetSafeNormal();
		
	}
	
}

FVector2D AWPlayerController::GetMoveInputActionValue() const
{
	if (IsValid(EnhancedInputComponent))
	{
		if (!MoveInputAction.IsNull())
		{
			return EnhancedInputComponent->BindActionValue(MoveInputAction.LoadSynchronous()).GetValue().Get<FVector2D>();
		}
	}
	return FVector2D::ZeroVector;
}

FVector2D AWPlayerController::GetMoveWorldSpaceInputActionValue() const
{
	if (IsValid(EnhancedInputComponent))
	{
		if (!MoveWorldSpaceInputAction.IsNull())
		{
			return EnhancedInputComponent->BindActionValue(MoveWorldSpaceInputAction.LoadSynchronous()).GetValue().Get<FVector2D>();
		}
	}
	return FVector2D::ZeroVector;
}


bool AWPlayerController::IsFullMovementInput_Implementation() const
{
	return MovementStickMode == EAnalogMovementBehavior::FixedSpeedSingleGait ||
		MovementStickMode == EAnalogMovementBehavior::VariableSpeedSingleGait||
			((MovementStickMode == EAnalogMovementBehavior::FixedSpeedWalkRun||
				MovementStickMode == EAnalogMovementBehavior::VariableSpeedWalkRun)&&
				(GetMoveInputActionValue().Length() >= AnalogWalkRunThreshold ||
					GetMoveWorldSpaceInputActionValue().Length() >= AnalogWalkRunThreshold));
		
	
	
}
void AWPlayerController::OnMoveInputAction(const FInputActionInstance& Instance)
{
	if (APawn * ControlledPawn = GetPawn())
	{
		const FVector2D InputScale = GetMovementInputScaleValue(Instance.GetValue().Get<FVector2D>());
		const FRotator Rotator = FRotator(0,GetControlRotation().Yaw,0);
		ControlledPawn->AddMovementInput(FRotationMatrix(Rotator).GetScaledAxis(EAxis::Y),InputScale.X);
		ControlledPawn->AddMovementInput(Rotator.Vector(),InputScale.Y);
	}
}

void AWPlayerController::OnMoveWorldSpaceInputAction(const FInputActionInstance& Instance)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector2D InputScale = Instance.GetValue().Get<FVector2D>().GetSafeNormal();
		ControlledPawn->AddMovementInput(FVector::RightVector,InputScale.X);
		ControlledPawn->AddMovementInput(FVector::ForwardVector,InputScale.Y);
	}
}

void AWPlayerController::OnLookInputAction(const FInputActionInstance& Instance)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector2D InputScale = Instance.GetValue().Get<FVector2D>();
		ControlledPawn->AddControllerYawInput(InputScale.X);
		ControlledPawn->AddControllerPitchInput(InputScale.Y);
	}
	
}

void AWPlayerController::OnLookGamepadInputAction(const FInputActionInstance& Instance)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector2D InputScale = Instance.GetValue().Get<FVector2D>() * GetWorld()->GetDeltaSeconds();
		ControlledPawn->AddControllerYawInput(InputScale.X);
		ControlledPawn->AddControllerPitchInput(InputScale.Y);
	}
}

void AWPlayerController::OnSprintInputAction(const FInputActionInstance& Instance)
{
	if (AWCharacter* ControlledPawn = GetWhiplashCharacter())
	{
		ControlledPawn->bWantsToSprint = !ControlledPawn->bWantsToSprint;
		if (ControlledPawn->bWantsToSprint)
		{
			ControlledPawn->AttributeSet->StartStaminaDrain();
		}
		else if (!ControlledPawn->bWantsToSprint)
		{
			ControlledPawn->AttributeSet->StopStaminaDrain();
			
		}
		
	}
}

void AWPlayerController::OnWalkInputAction(const FInputActionInstance& Instance)
{
	if (AWCharacter* ControlledPawn = GetWhiplashCharacter())
	{
		if (!ControlledPawn->bWantsToSprint)
		{
			ControlledPawn->bWantsToWalk = !ControlledPawn->bWantsToWalk;
		}
		
	}

}

void AWPlayerController::OnJumpInputAction(const FInputActionInstance& Instance)
{
	if (AWCharacter* ControlledPawn = GetWhiplashCharacter())
	{
		if (!ControlledPawn->bDoingTraversalAction)
		{
			if (const UNavMovementComponent* NavCharacterMovementComponent = 
				Cast<UNavMovementComponent>(ControlledPawn->GetCharacterMovement()))
			{
				if (NavCharacterMovementComponent->IsMovingOnGround())
				{
					bool bTraversalCheckFailed,bMontageSelectionFailed;
					ControlledPawn->TryTraversalAction(ControlledPawn->GetTraversalForwardTraceDistance(),bTraversalCheckFailed,bMontageSelectionFailed);
					if (bTraversalCheckFailed)
					{
						ControlledPawn->Jump();
					}
				}
			}
		}
	}
}

void AWPlayerController::OnCrouchInputActionTriggered(const FInputActionInstance& Instance)
{
	if (AWCharacter* ControlledPawn = GetWhiplashCharacter())
	{
		if (!ControlledPawn->bDoingTraversalAction)
		{
			if (UWAbilityComponent* AbilityComp = ControlledPawn->AbilityComponent)
			{
				if (UCharacterMovementComponent* MoveComp =ControlledPawn->GetCharacterMovement())
				{
					if (bIsCrouching)
					{
						AbilityComp->StopAbilityByTag(ControlledPawn,WhiplashTags::Ability_Action_Crouch);
						
						bIsCrouching = false;
						WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("Stopped: %s"),MoveComp->IsCrouching() ?TEXT("true") : TEXT("false"));
					}
					else
					{
						AbilityComp->StartAbilityByTag(ControlledPawn,WhiplashTags::Ability_Action_Crouch);
					
						bIsCrouching = true;
						WHIPLASH_LOG(LogWhiplashAbility,Error,TEXT("Started: %s"),MoveComp->IsCrouching() ?TEXT("true") : TEXT("false"));
					}
					
				}
			
			
			}
		}
		
	}
	
}



void AWPlayerController::OnStrafeInputAction(const FInputActionInstance& Instance)
{
	if (AWCharacter* ControlledPawn = GetWhiplashCharacter())
	{
		ControlledPawn->bWantsToStrafe = !ControlledPawn->bWantsToStrafe;
	}
}

void AWPlayerController::OnPerspectiveInputAction(const FInputActionInstance& Instance)
{
	
	if (AWCharacter* ControlledPawn = GetWhiplashCharacter())
	{
		ControlledPawn->bWantsFirstPerson = !ControlledPawn->bWantsFirstPerson;
	}
}

void AWPlayerController::OnAimInputAction(const FInputActionInstance& Instance)
{
	
	if (AWCharacter* ControlledPawn = GetWhiplashCharacter())
	{
		ControlledPawn->bWantsToAim = Instance.GetValue().Get<bool>();
		if (ControlledPawn->bWantsToAim)
		{
			ControlledPawn->bWantsToStrafe = true;
		}
	}
}
