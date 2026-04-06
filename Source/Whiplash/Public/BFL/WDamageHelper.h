// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Attributes/FWDamageEvent.h"
#include "WDamageHelper.generated.h"

/**
 * 
 */
UCLASS()
class WHIPLASH_API UWDamageHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category="Whiplash|Damage")
	static float CalculateDamage(const FDamageEventStruct& DamageEvent);
};
