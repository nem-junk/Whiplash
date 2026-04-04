#pragma once
#include "CoreMinimal.h"
//#include "GameFramework/DamageType.h"
#include "FWDamageEvent.generated.h"

class UDamageType;

USTRUCT(BlueprintType)
struct FDamageEventStruct
{
	GENERATED_BODY()
	
	float BaseDamage;
	
	AActor* Instigator;
	
	AActor* Causer;
	
	FHitResult HitResult;
	
	TSubclassOf<UDamageType> DamageType;
	
	FDamageEventStruct(float InBaseDamage=0,
		AActor* InInstigator=nullptr,
		AActor* InCauser=nullptr,
		FHitResult InHitResult={},
		TSubclassOf<UDamageType> InDamageType=nullptr)
			:BaseDamage(InBaseDamage),
	Instigator(InInstigator),
	Causer(InCauser),
	HitResult(InHitResult),
	DamageType(InDamageType)
	{
		
	}
	
	
};
