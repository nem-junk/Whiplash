
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WCoreTypes.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PoseSearch/PoseSearchTrajectoryLibrary.h"
#include "WTraversalComponent.h"



////////////////////////////////////////
#include "WCharacter.generated.h"
////////////////////////////////////////

class USpringArmComponent;
class UCameraComponent;
class UMotionWarpingComponent;
class UChooserTable;
UCLASS()
class WHIPLASH_API AWCharacter : public ACharacter
{
	GENERATED_BODY()
	friend class AWPlayerController;
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	FTransform DefaultCharacterTransform = FTransform(FRotator(0.0f,-90.0f,0.0f).Quaternion(),
		FVector(0.0f,0.0f,-89.0f),
		FVector::OneVector);
	FTransform DefaultSpringArmTransform = FTransform(FRotator(0.0f,0.0f,0.0f).Quaternion(),FVector(0.0f,0.0f,20.0f),FVector::OneVector);
	
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CMC;
	UPROPERTY()
	TObjectPtr<UCapsuleComponent> WCapsuleComponent;
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> MeshComp;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Character)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Character)
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Character)
	TObjectPtr<UMotionWarpingComponent>MotionWarping;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Character)
	bool bPreviousWantsFirstPerson = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Character)
	bool bWantsFirstPerson = false;
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
	UFUNCTION(BlueprintCallable,Category="Camera")
	virtual void UpdateCamera(bool bInterpolate);
	/*this func will be called every tick, and is used to update Gait Value and use it to set the max walk speed of the CMC*/
	UFUNCTION(BlueprintCallable,Category="Movement")
	virtual void UpdateMovement();
	/*called every tick, and is used to update the character movement component's rotation mode using the Wants to strafe input condition.
	 *when the character is on the ground, this fucntion sets the rotation rate to -1, which causes the character to rotate instantly 
	 * this technique allows us to treat the actor as the "target rotation", while we independently control the rotation of the root bone within ABP -> allowsa us to do things not currently supported in the CMC, such as stick flicks(completely re-orienting the character when only tapping movement input), and gives us more control over the rotation behavior during action like turn states, pivots, and turning in placethis is still an experimental technique. 
	 */
	UFUNCTION(BlueprintCallable,Category="Movement")
	virtual void UpdateRotation();
	/* this function determines the gait of the character based on the current input and the Wants to walk or Wants to Sprint condition. The movement stick mode determines whether walking or running can be controlled via stick deflection (currently, this can cause issues with motion matching selectionfot the safect Implementation, use the Fixed Speed - Single Gait option for now )
	 */
	UFUNCTION(BlueprintPure,Category="Movement")
	virtual EGait GetDesiredGait()const;
	/*this function is used to set the max speed for the character's movement. because the forwards,strafes, and backward Animation move at different speeds, we need to change the max speed of the character based on its movement direction. We use a simple curve to map different speed values to the different directions. 0 = forward, 1 = strafe L or R, 2 = Backwards. */
	UFUNCTION(BlueprintPure,Category="Movement")
	virtual float CalculateMaxSpeed() const;
	
	UFUNCTION(BlueprintCallable,Category="Traversal")
	virtual void TryTraversalAction(float TraceForwardDistance,bool& bOutTraversalCheckFailed, bool& bOutMontageSelectionFailed);
	/* Gets how fast the character is moving in its forward direction and uses the value to scale the distance of the forward trace*/
	UFUNCTION(BlueprintPure,Category="Traversal")
	virtual float GetTraversalForwardTraceDistance() const;
	/*In order for the actor to move to the exact points on the obstacle, we use a Motion Warping component which warps the montage’s root motion using notify states on the montage. This function updates the warp targets in the component using the ledge locations.*/
	UFUNCTION(BlueprintCallable,Category="Traversal")
	virtual void UpdateWrapTargets()const;
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="Traversal")
	void PlayAnimationMontage(const UAnimMontage*Montage,float PlayRate,float StartingPosition);
	UFUNCTION(BlueprintCallable,Category="Traversal")
	virtual void OnAnimationMontageCompletedOrInterrupted();
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="Audio")
	void PlayAudioEvent(const FGameplayTag& Value, float VolumeMultiplier,float PitchMultiplier);
///////////////////////////////////////////	

public:
	AWCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	// event on Jump - play a sound whenever the  character jumps, this could be done with audio events on the jump animations, but this way gives us more consistent timing, since motion matching can pick different entry frames of the jump animations. 
	virtual void OnJumped_Implementation() override;
	/* event on Land - play a sound whenever the character Lands, this logic also caches the Land velocity and enables a Just Landed Flag,
	 * which is used in a chooser table asset to select landing databases. We want this flag to be true for more than one frame,
	 * so that if movement conditions change upon landing, such as starting or stopping right after impact, Landing databases will still be valid*/	 
	virtual void Landed(const FHitResult& Hit) override;
	
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
	FTraversalCheckResult TraversalResult = {} ;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	bool bDoingTraversalAction = false;
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};
