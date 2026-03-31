
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
/////////////////////////////////
#include "WAbility.generated.h"
/////////////////////////////////

class UWorld;
class UWAbilityComponent;
class UWTagComponent;
class UTexture2D;

USTRUCT()
struct FActionData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	bool bIsRunning;
	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class WHIPLASH_API UWAbility : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(UWAbilityComponent* NewAbilityComponent);
	
	UPROPERTY(EditDefaultsOnly,Category="Abilities")
	bool bAutoStart;
	
	UFUNCTION(BlueprintNativeEvent,Category="Abilities")
	bool CanStart(AActor* Instigator);
	UFUNCTION(BlueprintCallable,Category="Abilities")
	bool IsRunning() const;



protected:
	UPROPERTY()
	UWAbilityComponent* AbilityComponent;
	UPROPERTY(/*ReplicatedUsing="OnRep_"*/)
	FActionData ActionData;
	
	UFUNCTION(BlueprintCallable,Category="Abilities")
	UWAbilityComponent* GetOwningComponent() const;
	
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantTags;
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockedTags;

};
