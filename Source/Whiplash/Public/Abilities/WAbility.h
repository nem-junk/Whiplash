
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
	bool bIsRunning = false;
	UPROPERTY()
	AActor* Instigator = nullptr;
};

UCLASS(Blueprintable)
class WHIPLASH_API UWAbility : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(UWAbilityComponent* NewAbilityComponent);
	
	UPROPERTY(EditDefaultsOnly,Category="Abilities")
	bool bAutoStart;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Tags")
	FGameplayTag AbilityTag;
	
	
	
	UFUNCTION(BlueprintNativeEvent,Category="Abilities")
	void StartAbility(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent,Category="Abilities")
	void StopAbility(AActor* Instigator);
	
	UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintNativeEvent,Category="Abilities")
	bool CanStart(AActor* Instigator);
	UFUNCTION(BlueprintCallable,Category="Abilities")
	bool IsRunning() const;
	



protected:
	UPROPERTY()
	UWAbilityComponent* AbilityComponent;
	UPROPERTY()
	UWTagComponent* TagComponent;
	UPROPERTY(/*ReplicatedUsing="OnRep_"*/)
	FActionData ActionData;
	
	UFUNCTION(BlueprintCallable,Category="Abilities")
	UWAbilityComponent* GetOwningComponent() const;
	
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantTags;
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockedTags;

};
