
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WComponentInterface.generated.h"

class UWTagComponent;
class UWAttributeSet;
class UWAbilityComponent;
UINTERFACE()
class UWComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WHIPLASH_API IWComponentInterface
{
	GENERATED_BODY()

public:
	virtual UWTagComponent* GetTagComponent() const =0;
	virtual UWAttributeSet* GetAttributeSet() const =0;
	virtual UWAbilityComponent* GetAbilityComponent() const=0;
};
