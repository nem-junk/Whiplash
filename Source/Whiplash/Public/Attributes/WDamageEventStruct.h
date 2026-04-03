#pragma once
#include "CoreMinimal.h"
//#include "GameFramework/DamageType.h"
#include "WDamageEventStruct.generated.h"

class UDamageType;

USTRUCT()
struct FWDamageEventStruct
{
	GENERATED_BODY()
	UPROPERTY()
	float BaseDamage;
	UPROPERTY()
	AActor* Instigator;
	UPROPERTY()
	AActor* Causer;
	UPROPERTY()
	FHitResult HitResult;
	UPROPERTY()
	TSubclassOf<UDamageType> DamageType;
	
	FWDamageEventStruct(float InBaseDamage=0,
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
