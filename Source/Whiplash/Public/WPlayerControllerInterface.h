
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WPlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WHIPLASH_API IWPlayerControllerInterface
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="Input|Action")
	bool IsFullMovementInput()const;
};
