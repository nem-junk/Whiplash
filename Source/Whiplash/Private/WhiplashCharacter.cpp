
#include "WhiplashCharacter.h"


AWhiplashCharacter::AWhiplashCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

}


void AWhiplashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void AWhiplashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWhiplashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

