
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeapon.generated.h"

UINTERFACE(MinimalAPI,Blueprintable)
class UIWeapon : public UInterface
{
	GENERATED_BODY()
};

class WHIPLASH_API IIWeapon
{
	GENERATED_BODY()
public:
	// 0 hipfire '' 1 fully aimed 
	virtual float GetAimingAlpha() const =0;
	virtual bool IsCrouching() const =0;
	virtual bool IsFalling() const =0;
	virtual float GetMovementSpeed() const =0;



};
