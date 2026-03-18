
#include "WhiplashCharacter.h"



AWhiplashCharacter::AWhiplashCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

}


void AWhiplashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void AWhiplashCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AWhiplashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWhiplashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AWhiplashCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
}

void AWhiplashCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}


void AWhiplashCharacter::UpdateCamera(bool bInterpolate)
{
}

void AWhiplashCharacter::UpdateMovement()
{
}

void AWhiplashCharacter::UpdateRotation()
{
}

EGait AWhiplashCharacter::GetDesiredGait() const
{
}

float AWhiplashCharacter::CalculateMaxSpeed() const
{
}

void AWhiplashCharacter::TryTraversalAction(float TraceForwardDistance, bool& bOutTraversalCheckFailed,
	bool& bOutMontageSelectionFailed)
{
}

float AWhiplashCharacter::GetTraversalForwardTraceDistance() const
{
}

void AWhiplashCharacter::UpdateWrapTargets() const
{
}


void AWhiplashCharacter::OnAnimationMontageCompletedOrInterrupted()
{
}

void AWhiplashCharacter::PlayAudioEvent_Implementation(const FGameplayTag& Value, float VolumeMultiplier,
	float PitchMultiplier)
{
}

void AWhiplashCharacter::PlayAnimationMontage_Implementation(const UAnimMontage* Montage, float PlayRate,
	float StartingPosition)
{
}
