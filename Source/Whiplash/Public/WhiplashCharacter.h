
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WhiplashCoreTypes.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PoseSearch/PoseSearchTrajectoryLibrary.h"
#include "WTraversalComponent.h"



////////////////////////////////////////
#include "WhiplashCharacter.generated.h"
////////////////////////////////////////

class USpringArmComponent;
class UCameraComponent;
class UMotionWarpingComponent;
class UChooserTable;
UCLASS()
class WHIPLASH_API AWhiplashCharacter : public ACharacter
{
	GENERATED_BODY()
	friend class AWhiplashPlayerController;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Character)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Character)
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Character)
	TObjectPtr<UMotionWarpingComponent>MotionWarping;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Character)
	bool bPreviousWantsFirstPerson = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Character)
	bool bWantsToSprint = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Character)
	bool bWantsToWalk = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Character)
	bool bWantsToStrafe = true;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Character)
	bool bWantsToAim = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Movement)
	TSoftObjectPtr<UCurveFloat> StrafeSpeedMapCurve; // use soft object ptr when the object is not needed everytime <> only get loaded when needed 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Movement)
	EGait Gait = EGait::Run;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Movement)
	bool bJustLanded = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Movement)
	FVector LandVelocity = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Traversal)
	TSoftObjectPtr<UChooserTable> TraversalAnimationChooserTable;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Audio)
	FGameplayTag JumpEventAudioGameplayTag;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Audio)
	FGameplayTag LandEventAudioGameplayTag;
	UPROPERTY()
	FTimerHandle LandTimerHandle;
////////////////funcs//////////////////////	
	
	
//////////////////////////////////////	

public:
	AWhiplashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
////	virtual void PossessedBy(AController* NewController) override;
	// event on Jump - play a sound whenever the  character jumps, this could be done with audio events on the jump animations, but this way gives us more consistent timing, since motion matching can pick different entry frames of the jump animations. 
////	virtual void OnJumped_Implementation() override;
	/* event on Land - play a sound whenever the character Lands, this logic also caches the Land velocity and enables a Just Landed Flag,
	 * which is used in a chooser table asset to select landing databases. We want this flag to be true for more than one frame,
	 * so that if movement conditions change upon landing, such as starting or stopping right after impact, Landing databases will still be valid*/	 
////	virtual void Landed(const FHitResult& Hit) override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Camera)
	float CameraDistanceMultiplier = 1.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Camera)
	FCameraParameters CameraStyleFirstPerson;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Camera)
	FCameraParameters CameraStyleThirdPersonFar;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Camera)
	FCameraParameters CameraStyleThirdPersonClose;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Camera)
	FCameraParameters CameraStyleThirdPersonAim;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	FVector WalkSpeeds = FVector(200.0f,175.0f,150.0f);
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	FVector RunSpeeds = FVector(500.0f,350.0f,300.0f);
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	FVector SprintSpeeds = FVector(700.0f,700.0f,700.0f);
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	FVector CrouchSpeeds = FVector(200.0f,175.0f,150.0f);
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	FRotator ThirdPersonFallingRotationRate = FRotator(0.0f,200.0f,0.0f);
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	FRotator ThirdPersonNotFallingRotationRate = FRotator(0.0f,-1.0f,0.0f);
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	FRotator FirstPersonFallingRotationRate = FRotator(0.0f,-1.0f,0.0f);
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	FRotator FirstPersonNotFallingRotationRate = FRotator(0.0f,-1.0f,0.0f);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	FTraversalCheckResult TraversalCheckResult ;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	bool bDoingTraversalAction = false;

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

};
