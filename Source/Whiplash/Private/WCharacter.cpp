//////////??//////////////////
#include "WCharacter.h"
//////////??//////////////////
#include "AnimationWarpingLibrary.h"
#include "ChooserFunctionLibrary.h"



AWCharacter::AWCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

}


void AWCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void AWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AWCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
}

void AWCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}


void AWCharacter::UpdateCamera(bool bInterpolate)
{
}

void AWCharacter::UpdateMovement()
{
}

void AWCharacter::UpdateRotation()
{
}

EGait AWCharacter::GetDesiredGait() const
{
	return EGait::Run;
}

float AWCharacter::CalculateMaxSpeed() const
{
	return 0.02f;
}

void AWCharacter::TryTraversalAction(float TraceForwardDistance, bool& bOutTraversalCheckFailed,
	bool& bOutMontageSelectionFailed)
{
}

float AWCharacter::GetTraversalForwardTraceDistance() const
{
	return 0.02f;
}

void AWCharacter::UpdateWrapTargets() const
{
}


void AWCharacter::OnAnimationMontageCompletedOrInterrupted()
{
}

void AWCharacter::PlayAudioEvent_Implementation(const FGameplayTag& Value, float VolumeMultiplier,
	float PitchMultiplier)
{
}

void AWCharacter::PlayAnimationMontage_Implementation(const UAnimMontage* Montage, float PlayRate,
	float StartingPosition)
{
}
