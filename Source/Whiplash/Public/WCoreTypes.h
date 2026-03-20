
#pragma once

#include "CoreMinimal.h"
#include "WCoreTypes.generated.h"

WHIPLASH_API DECLARE_LOG_CATEGORY_EXTERN(LogWhiplash,Log,All);

#define WHIPLASH_LOG(CategoryName, Verbosity, Format, ...) \
UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__); \
if constexpr ((::ELogVerbosity::Verbosity & ELogVerbosity::VerbosityMask) == ::ELogVerbosity::Error) \
{ \
if(GEngine) \
{ \
GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__)); \
} \
}
#define TraceType_Traversal TraceTypeQuery1

UENUM(BlueprintType)
enum class EAnalogMovementBehavior : uint8
{
	/*This is For Gamepad ill Discard this Later*/
	/* Character will move at a fixed speed regardless of stick deflection */
	FixedSpeedSingleGait = 0 UMETA(DisplayName = "FixedSpeed-SingleGait"),
	/* Character will move at a fixed walking speed with slight stick deflection,and a fixed running speed at full stick deflection*/
	FixedSpeedWalkRun = 1 UMETA(DisplayName = "FixedSpeed-WalkRun"),
	/*Full analog movement control with stick, character will remain walking or running based on gait input.*/
	VariableSpeedSingleGait = 2 UMETA(DisplayName = "VariableSpeed-SingleGait"),
	/*Full analog movement control with stick, character will switch from walk to run gait based on stick deflection.*/
	VariableSpeedWalkRun = 3 UMETA(DisplayName = "VariableSpeed-WalkRun"),
};

UENUM(BlueprintType)
enum class EMovementSituation: uint8
{
	OnGround = 0,
	InAir = 1, 
	
};
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Idle = 0,
	Moving = 1,
};
UENUM(BlueprintType)
enum class EMovementStance : uint8
{
	Stand = 0,
	Crouch = 1
};
UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	OrientToMovement = 0,
	Strafe = 1
};
USTRUCT(BlueprintType)
struct WHIPLASH_API FCameraParameters
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera)
	float SpringArmLength = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera)
	FVector SocketOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera)
	float TranslationLagSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera)
	float FieldOfView = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Camera)
	float TransitionSpeed = 5.0f;
};
