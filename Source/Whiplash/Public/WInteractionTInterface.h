
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WInteractionTInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWInteractionTInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WHIPLASH_API IWInteractionTInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Interface|Traversal")
	void SetInteractionTransform(const FTransform& InteractionTransform );
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Interface|Traversal")
	void GetInteractionTransform (FTransform& OutInteractionTransform ) const;
	
};

UCLASS(meta = (BlueprinThreadSafe))
class WHIPLASH_API UInteractionTInterfaceBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="BFL|Traversal")
	static UPARAM(DisplayName = "Success") bool SetInteractionTransform(UObject*InteractionTransformObject,const FTransform& InteractionTransform);
	UFUNCTION(BlueprintCallable,Category="BFL|Traversal")
	static UPARAM (DisplayName = "Success") bool GetInteractionTransform(UObject* InteractionTransformObject,UPARAM(DisplayName="InteractionTransform")FTransform& OutInteractionTransform);
	
};
