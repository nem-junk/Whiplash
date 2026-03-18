#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/Interface.h"
/////////////////////////////////////////
#include "WTraversalComponent.generated.h"
/////////////////////////////////////////

class USplineComponent;

UENUM(BlueprintType)
enum class ETraversalActionType : uint8
{
	None = 0,
	/* traverse over a thin object and end on the ground at a similar level */
	Hurdle = 1,
	/* traverse over a thin object and end in a falling state (tall fence, or elevated obstacle with no floor on the other side)*/
	Vault = 2,
	/* traverse up and onto an object without passing over it */
	Mantle=3
};

UENUM(BlueprintType)
enum class EGait : uint8
{
	Walk = 0,
	Run = 1,
	Sprint = 2
};
USTRUCT(BlueprintType)
struct FTraversalCheckResult
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Traversal")
	ETraversalActionType ActionType = ETraversalActionType::None;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Traversal")
	bool bHasFrontLedge = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Traversal",meta = (EditCondition = "bHasFrontLedge", EditConditionHides)) /*crazy meta specifier here which greys out the property in editor for simple UI  */
	FVector FrontLedgeLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Traversal",meta=(EditCondition = "bHasFrontLedge", EditConditionHides))
	FVector FrontLedgeNormal = FVector::ZeroVector;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	bool bHasBackLedge = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal", meta=(EditCondition = "bHasBackLedge", EditConditionHides))
	FVector BackLedgeLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal",meta=(EditCondition = "bHasBackLedge", EditConditionHides))
	FVector BackLedgeNormal = FVector::ZeroVector;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal", meta=(EditCondition = "bHasBackLedge", EditConditionHides))
	float BackLedgeHeight = 0.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	bool bHasBackFloor = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal", meta=(EditCondition = "bHasBackFloor", EditConditionHides))
	FVector BackFloorLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	float ObstacleHeight = 0.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	float ObstacleDepth = 0.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	TObjectPtr<UPrimitiveComponent> HitComponent = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	TObjectPtr<const UAnimMontage> ChosenMontage = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	float StartTime = 0.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Traversal")
	float PlayRate = 0.0f;
	
	FORCEINLINE FString ToString() const
	{
		return FString::Printf(
			TEXT("Has Front Ledge: %hhd\nHas Back Ledge: %hhd\nHas Back Floor:%hhd\nObstacle Height: %f\nObstacle Depth: %f\nBack Ledge Height: %f\nChosen Animation: %s\nAnimation Start Time: %f\nAnimation Play Rate: %f"),
			bHasFrontLedge, bHasBackLedge, bHasBackFloor, ObstacleHeight, ObstacleDepth, BackLedgeHeight, IsValid(ChosenMontage) ? *ChosenMontage->GetName() : TEXT("nullptr"), StartTime, PlayRate);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHIPLASH_API UWTraversalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWTraversalComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:	

		
};
