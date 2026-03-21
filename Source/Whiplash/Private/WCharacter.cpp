//////////??//////////////////
#include "WCharacter.h"
//////////??//////////////////
#include "AnimationWarpingLibrary.h"
#include "ChooserFunctionLibrary.h"
#include "WPlayerControllerInterface.h"
#include "KismetAnimationLibrary.h"
#include "MotionWarpingComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "WInteractionTInterface.h"

AWCharacter::AWCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationYaw = false;
	MeshComp = GetMesh();
	MeshComp->SetRelativeTransform(DefaultCharacterTransform);
	MeshComp->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	
	CMC = GetCharacterMovement();
	CMC->MaxAcceleration = 800.f;
	CMC->BrakingFrictionFactor = 1.0f;
	CMC->GroundFriction = 5.0f;
	CMC->MaxWalkSpeed = 500.0f;
	CMC->MinAnalogWalkSpeed = 150.0f;
	CMC->BrakingDecelerationWalking = 750.0f;
	CMC->bCanWalkOffLedgesWhenCrouching = true;
	CMC->PerchRadiusThreshold = 20.f;
	CMC->bUseFlatBaseForFloorChecks = true;
	CMC->JumpZVelocity = 500.f;
	CMC->AirControl = 0.25f;
	CMC->RotationRate = FRotator(0.0f, -1.0f, 0.0f);
	CMC->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	WCapsuleComponent = GetCapsuleComponent();
	WCapsuleComponent->SetCapsuleHalfHeight(86.0);
	WCapsuleComponent->SetCapsuleRadius(30.0);
	WCapsuleComponent->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	WCapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeTransform(DefaultSpringArmTransform);
	SpringArm->ProbeSize = 0.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagMaxDistance = 200.0f;
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	
	CameraStyleFirstPerson.SpringArmLength = 0.0f;
	CameraStyleFirstPerson.SocketOffset = FVector(11.0f,0.0f,9.0f);
	CameraStyleFirstPerson.TranslationLagSpeed = 50.0f;
	CameraStyleFirstPerson.FieldOfView=90.0f;
	CameraStyleFirstPerson.TransitionSpeed=5.0f;
	
	CameraStyleThirdPersonFar.SpringArmLength = 300.0f;
	CameraStyleThirdPersonFar.SocketOffset = FVector(0.0f,0.0f,20.0f);
	CameraStyleThirdPersonFar.TranslationLagSpeed = 6;
	CameraStyleThirdPersonFar.FieldOfView = 90.0f;
	CameraStyleThirdPersonFar.TransitionSpeed = 2;
	
	CameraStyleThirdPersonClose.SpringArmLength = 200.0f;
	CameraStyleThirdPersonClose.SocketOffset = FVector(0.0f,50.0f,30.0f);
	CameraStyleThirdPersonClose.TranslationLagSpeed = 15.0f;
	CameraStyleThirdPersonClose.FieldOfView = 80.0f;
	CameraStyleThirdPersonClose.TransitionSpeed = 5.0f;
}
void AWCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}
void AWCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void AWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovement();
	UpdateRotation();
	UpdateCamera(true);
}
void AWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void AWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UpdateCamera(false);
}

void AWCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
	static const FVector2D RangeA = FVector2D(0,500);
	static const FVector2D RangeB = FVector2D(0.5,1);
	PlayAudioEvent(JumpEventAudioGameplayTag,FMath::GetMappedRangeValueClamped(RangeA,
		RangeB,
		GetCharacterMovement()->Velocity.Size2D()),1);
	
}

void AWCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	LandVelocity = GetCharacterMovement()->Velocity;
	static const FVector2D RangeA = FVector2D(-500,-900);
	static const FVector2D RangeB = FVector2D(0.5,1.5);
	PlayAudioEvent(LandEventAudioGameplayTag,
		FMath::GetMappedRangeValueClamped(RangeA,
			RangeB,
			LandVelocity.Z),
			1);

	bJustLanded = true;
	GetWorldTimerManager().ClearTimer(LandTimerHandle);
	GetWorldTimerManager().SetTimer(LandTimerHandle,[&](){bJustLanded = false;},0.3,false);
}


void AWCharacter::UpdateCamera(bool bInterpolate)
{
	/* If perspective changed, reattach spring arm to head socket or capsule accordingly, resize capsule, and reset spring arm transform if returning to third person */
	if (bPreviousWantsFirstPerson != bWantsFirstPerson)
	{
		static const FName HeadSocketName = FName("head");
		SpringArm->AttachToComponent(bWantsFirstPerson ? Cast<UPrimitiveComponent>(GetMesh()) : Cast<UPrimitiveComponent>(GetCapsuleComponent()),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			bWantsFirstPerson ? HeadSocketName : NAME_None);
		GetCapsuleComponent()->SetCapsuleSize(bWantsFirstPerson ? 50 : 30,86,true);
		if (!bWantsFirstPerson)
		{
			SpringArm->SetRelativeTransform(FTransform(
				FRotator(0,0,0).Quaternion(),
				FVector(0,0,20),
				FVector::OneVector));
		}
		bPreviousWantsFirstPerson = bWantsFirstPerson;
	}
	const FCameraParameters& TargetCameraParameters = bWantsFirstPerson ? CameraStyleFirstPerson :
		bWantsToStrafe ? (bWantsToAim ? CameraStyleThirdPersonAim : CameraStyleThirdPersonClose):CameraStyleThirdPersonFar;
	const float TranslationLagSpeed = bInterpolate ? TargetCameraParameters.TranslationLagSpeed : -1;
	const float TransitionSpeed = bInterpolate ? TargetCameraParameters.TransitionSpeed : -1;
	
	Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView,TargetCameraParameters.FieldOfView,
		GetWorld()->DeltaTimeSeconds,TransitionSpeed));
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength,TargetCameraParameters.SpringArmLength,
		GetWorld()->DeltaTimeSeconds,TransitionSpeed);
	SpringArm->CameraLagSpeed = FMath::FInterpTo(SpringArm->CameraLagSpeed,TargetCameraParameters.TranslationLagSpeed,
		GetWorld()->DeltaTimeSeconds,TransitionSpeed);
	SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset,TargetCameraParameters.SocketOffset,
		GetWorld()->DeltaTimeSeconds,TransitionSpeed);
}

void AWCharacter::UpdateMovement()
{
	Gait = GetDesiredGait();
	if (CMC)
	{
		CMC->MaxWalkSpeed = CMC->MaxWalkSpeedCrouched = CalculateMaxSpeed();
	}
}

void AWCharacter::UpdateRotation()
{
	if (CMC)
	{
		CMC->bUseControllerDesiredRotation = bWantsFirstPerson || bWantsToStrafe ;
		CMC->bOrientRotationToMovement = !bWantsFirstPerson && !bWantsToStrafe;
		CMC->RotationRate = CMC->IsFalling() ? (bWantsFirstPerson ? FirstPersonFallingRotationRate : ThirdPersonFallingRotationRate) : (bWantsFirstPerson ? FirstPersonNotFallingRotationRate : ThirdPersonNotFallingRotationRate);
	}
}

EGait AWCharacter::GetDesiredGait() const
{
	bool bFullMovementInput = false;
	if (IsValid(Controller))
	{
		if (const IWPlayerControllerInterface* ControllerInterface = 
			Cast<IWPlayerControllerInterface>(Controller))
		{
			bFullMovementInput = ControllerInterface->Execute_IsFullMovementInput(Controller);
		}
	}
	return(!bWantsToSprint && bWantsToWalk) || (!bWantsToSprint && !bWantsToWalk && !bFullMovementInput) ? EGait::Walk :EGait::Run;
}

float AWCharacter::CalculateMaxSpeed() const
{
	if (!StrafeSpeedMapCurve.IsNull())
	{
		const float StrafeSpeedMap = /*loadsync. will wait for the asset to load since it is a softPtr*/
			StrafeSpeedMapCurve.LoadSynchronous()->GetFloatValue(
			FMath::Abs(UKismetAnimationLibrary::CalculateDirection(CMC->Velocity,GetActorRotation())));
		const bool bStrafe = StrafeSpeedMap < 1.0f;
		const FVector& Speeds = CMC->IsCrouching() ? CrouchSpeeds : 
		Gait == EGait::Walk ? WalkSpeeds :
		Gait == EGait::Run ? RunSpeeds :
		Gait == EGait::Sprint ? SprintSpeeds :
		FVector::ZeroVector;
		static const FVector2D RangeA1 = FVector2D(0,1);
		static const FVector2D RangeA2 = FVector2D(1,2);
		return FMath::GetMappedRangeValueClamped(bStrafe ? RangeA1 : RangeA2,FVector2D(bStrafe ? Speeds.X :Speeds.Y, bStrafe ? Speeds.Y : Speeds.Z),StrafeSpeedMap);
 		
	}
	return INDEX_NONE ;
}

void AWCharacter::TryTraversalAction(float TraceForwardDistance, bool& bOutTraversalCheckFailed,
	bool& bOutMontageSelectionFailed)
{
	/* cache imp. values for use later in func */
	
	const double StartTime = FPlatformTime::Seconds();/*only for debug*/
	const FVector& ActorLocation = GetActorLocation();
	const FVector& ActorForwardVector = GetActorForwardVector();
	const float&   CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	const float&   CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	static const FString DrawDebugLevelConsoleVariableName = TEXT("DDCvar.Traversal.DrawDebugLevel");
	const int32& DrawDebugLevel = UKismetSystemLibrary::GetConsoleVariableIntValue(DrawDebugLevelConsoleVariableName);
	static const FString DrawDebugDurationConsoleVariableName = TEXT("DDCvar.Traversal.DrawDebugDuration");
	const float& DrawDebugDuration = UKismetSystemLibrary::GetConsoleVariableFloatValue(DrawDebugDurationConsoleVariableName);
	FTraversalCheckResult TraversalCheckResult;
	///////////////////////////////////////////
	/* find trace from actor location to find the Traversal Obstacle */
	
	FHitResult Hit;
	UKismetSystemLibrary::CapsuleTraceSingle(this,
		ActorLocation,(ActorLocation+ActorForwardVector * TraceForwardDistance),30,60,
		TraceType_Traversal,false,{},
		DrawDebugLevel >=2 ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,Hit,true,
		FLinearColor::Black,FLinearColor::White,DrawDebugDuration);
	
	if (!Hit.bBlockingHit || (!Hit.GetActor() && !Hit.GetActor()->Implements<UTraversalObstacleInterface>() ))
	{
		bOutTraversalCheckFailed = true;
		bOutMontageSelectionFailed = false;
		return;
	}
	ITraversalObstacleInterface* Obstacle = Cast<ITraversalObstacleInterface>(Hit.GetActor());
	if (Obstacle)
	{
		Obstacle->Execute_GetLedgeTransforms(Hit.GetActor(),Hit.ImpactPoint,ActorLocation,TraversalCheckResult);
	}
	if (DrawDebugLevel >= 1 )
	{
		if (TraversalCheckResult.bHasFrontLedge)
		{
			DrawDebugSphere(GetWorld(),TraversalCheckResult.FrontLedgeLocation,10,12,FLinearColor::Black.ToFColor(true),false,
				DrawDebugDuration,SDPG_World,1);
		}
		if (TraversalCheckResult.bHasBackLedge)
		{
			DrawDebugSphere(GetWorld(),TraversalCheckResult.BackLedgeLocation,10,12,FLinearColor::Black.ToFColor(true),false,DrawDebugDuration,
				SDPG_World,1);
		}
	}
	const FVector HasRoomCheckFrontLedgeLocation = TraversalCheckResult.FrontLedgeLocation+
		TraversalCheckResult.FrontLedgeNormal*(CapsuleRadius + 2)+
			FVector::ZAxisVector*(CapsuleHalfHeight+2);
	
	UKismetSystemLibrary::CapsuleTraceSingle(this,ActorLocation,HasRoomCheckFrontLedgeLocation,CapsuleRadius,CapsuleHalfHeight,
		TraceType_Traversal,false,{},DrawDebugLevel>=3 ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		Hit,true,FLinearColor::Black,FLinearColor::White,DrawDebugDuration);
	if (Hit.bBlockingHit||Hit.bStartPenetrating)
	{
		TraversalCheckResult.bHasFrontLedge = false;
		bOutTraversalCheckFailed = true;
		bOutMontageSelectionFailed = false;
		return;
	}
	TraversalCheckResult.ObstacleHeight = FMath::Abs((ActorLocation - FVector::ZAxisVector*CapsuleHalfHeight-TraversalCheckResult.FrontLedgeLocation).Z);
	
	const FVector HasRoomCheckBackLedgeLocation = TraversalCheckResult.BackLedgeLocation+ 
		TraversalCheckResult.BackLedgeNormal*(CapsuleRadius + 2)+ FVector::ZAxisVector*(CapsuleHalfHeight+2);
	if (UKismetSystemLibrary::CapsuleTraceSingle(this,HasRoomCheckFrontLedgeLocation,HasRoomCheckBackLedgeLocation,
		CapsuleRadius,CapsuleHalfHeight,TraceType_Traversal,false,{},DrawDebugLevel>=3 ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		Hit,true,FLinearColor::Black,FLinearColor::White,DrawDebugDuration))
	{
		//found something blocking 
		TraversalCheckResult.ObstacleDepth = (Hit.ImpactPoint - TraversalCheckResult.FrontLedgeLocation).Size2D();
		TraversalCheckResult.bHasBackLedge = false;
	}
	else
	{
		TraversalCheckResult.ObstacleDepth = (TraversalCheckResult.FrontLedgeLocation - TraversalCheckResult.BackLedgeLocation).Size2D();
		
		const FVector EndTraceLocation = TraversalCheckResult.BackLedgeLocation+TraversalCheckResult.BackLedgeNormal*(CapsuleRadius + 2)-
			FVector::ZAxisVector * (TraversalCheckResult.ObstacleHeight - CapsuleHalfHeight+50);
		UKismetSystemLibrary::CapsuleTraceSingle(this,HasRoomCheckBackLedgeLocation,EndTraceLocation,CapsuleRadius,CapsuleHalfHeight,TraceType_Traversal,false,{},
			DrawDebugLevel>=3 ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,Hit,true,FLinearColor::Black,FLinearColor::White,DrawDebugDuration);
		
		if (Hit.bBlockingHit)
		{
			TraversalCheckResult.bHasBackFloor = true;
			TraversalCheckResult.BackFloorLocation = Hit.ImpactPoint;
			TraversalCheckResult.BackLedgeHeight = FMath::Abs((Hit.ImpactPoint-TraversalCheckResult.BackLedgeLocation).Z);
		}
		else
		{
			TraversalCheckResult.bHasBackFloor = false;
		}
	}
	// check what type of traversal action to do 
	
	if (TraversalCheckResult.bHasFrontLedge && TraversalCheckResult.bHasBackLedge && !TraversalCheckResult.bHasBackFloor && 
		UKismetMathLibrary::InRange_FloatFloat(TraversalCheckResult.ObstacleHeight,50,125) && TraversalCheckResult.ObstacleDepth < 59)
	{
		TraversalCheckResult.ActionType = ETraversalActionType::Vault;
	}
	else if ( TraversalCheckResult.bHasFrontLedge && TraversalCheckResult.bHasBackLedge && TraversalCheckResult.bHasBackFloor && 
		UKismetMathLibrary::InRange_FloatFloat(TraversalCheckResult.ObstacleHeight,50,125) && TraversalCheckResult.ObstacleDepth < 59 && 
		TraversalCheckResult.BackLedgeHeight > 50)
	{ 
		TraversalCheckResult.ActionType = ETraversalActionType::Hurdle;
	}
	else if ( TraversalCheckResult.bHasFrontLedge && UKismetMathLibrary::InRange_FloatFloat(TraversalCheckResult.ObstacleHeight,50,275) && 
		TraversalCheckResult.ObstacleDepth >= 59)
	{
		TraversalCheckResult.ActionType = ETraversalActionType::Mantle;
	}
	else
	{
		TraversalCheckResult.ActionType = ETraversalActionType::None;
	}
	if (TraversalCheckResult.ActionType == ETraversalActionType::None)
	{
		bOutTraversalCheckFailed = true;
		bOutMontageSelectionFailed = false;
		return;
	}
	// to AnimationBP 
	IWInteractionTInterface* InteractableObject = Cast<IWInteractionTInterface>(GetMesh()->GetAnimInstance());
	if (InteractableObject == nullptr && !GetMesh()->GetAnimInstance()->Implements<UWInteractionTInterface>())
	{
		bOutTraversalCheckFailed = true;
		bOutMontageSelectionFailed = false;
		return;
	}
	const FTransform InteractionTransform = 
		FTransform(FRotationMatrix::MakeFromZ(TraversalCheckResult.FrontLedgeNormal).ToQuat(),TraversalCheckResult.FrontLedgeLocation,FVector::OneVector);
	
	if (InteractableObject)
	{
		InteractableObject->Execute_SetInteractionTransform(GetMesh()->GetAnimInstance(), InteractionTransform);
	}
	/* if the interactable object is not valid call interface function directly here*/
	
	FTraversalChooserParameters ChooserParameters;
	ChooserParameters.ActionType = TraversalCheckResult.ActionType;
	ChooserParameters.Gait = Gait;
	ChooserParameters.Speed = GetCharacterMovement()->Velocity.Size2D();
	ChooserParameters.ObstacleHeight = TraversalCheckResult.ObstacleHeight;
	ChooserParameters.ObstacleDepth = TraversalCheckResult.ObstacleDepth;
	
	FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();
	Context.AddStructParam(ChooserParameters);
	//MakeEvaluateChooser()->wraps the chooser table in an evaluatable form
	//EvaluateObjectChooserBaseMulti()->runs the chooser with the context given, returns all the montages in this case that pass the conditions as a TArray 
	TArray<UObject*> AnimationAssets = UChooserFunctionLibrary::EvaluateObjectChooserBaseMulti(Context,UChooserFunctionLibrary::MakeEvaluateChooser(TraversalAnimationChooserTable.LoadSynchronous()),
		UAnimMontage::StaticClass());
	
	/*perform a motion match on all the montages that were chosen by the chooser to find the best result. this match will elect the best montage and the best entry frame(start time ) bases on the distance to the ledge, and the current Characters pose. if for some reason no montage was found (motion matching failed, pherhaps due to an invalid database or isse with the schema ), print a warning and exit the function*/
	static const FName PoseHistoryName  = TEXT("PoseHistory");
	FPoseSearchBlueprintResult Result;
	UPoseSearchLibrary:: MotionMatch(GetMesh()->GetAnimInstance(),AnimationAssets,
		PoseHistoryName,FPoseSearchContinuingProperties(),FPoseSearchFutureProperties(),Result);
	TObjectPtr<const UAnimMontage> AnimationMontage = Cast<UAnimMontage>(Result.SelectedAnim);
	if (!IsValid(AnimationMontage))
	{
		UKismetSystemLibrary::PrintString(this,TEXT("FailedToFindMontage"),
			true,false,FLinearColor::Red,DrawDebugDuration);
		bOutTraversalCheckFailed = false;
		bOutMontageSelectionFailed = true;
		return;
	}
	TraversalCheckResult.ChosenMontage = AnimationMontage;
	TraversalCheckResult.StartTime = Result.SelectedTime;
	TraversalCheckResult.PlayRate = Result.WantedPlayRate;
	
	TraversalResult = TraversalCheckResult;
	UpdateWrapTargets();
	PlayAnimationMontage(TraversalResult.ChosenMontage,TraversalResult.PlayRate,TraversalResult.StartTime);
	bDoingTraversalAction = true;
	GetCapsuleComponent()->IgnoreComponentWhenMoving(TraversalResult.HitComponent,true);
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	
	if (DrawDebugLevel >=1)
	{
		UKismetSystemLibrary::PrintString(this,TraversalCheckResult.ToString(),true,false,FLinearColor::Blue,DrawDebugDuration);
		UKismetSystemLibrary::PrintString(this,UEnum::GetValueAsString(TraversalCheckResult.ActionType),true,false,FLinearColor::Blue,DrawDebugDuration);
		const FString PerfString = FString::Printf(TEXT("Execution Time: %f seconds"),FPlatformTime::Seconds()-StartTime);
		UKismetSystemLibrary::PrintString(this,PerfString,true,true,FLinearColor::White,DrawDebugDuration);
	}

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
