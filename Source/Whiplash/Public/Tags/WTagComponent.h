#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WGameplayTags.h"
#include "WTagComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagChanged,FGameplayTag,Tags,bool,bAdded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHIPLASH_API UWTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWTagComponent();
	
	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnTagChanged OnTagChanged;

	UFUNCTION(BlueprintCallable,Category= "Tags")
	void AddTags(FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable,Category= "Tags")
	void RemoveTags(FGameplayTag TagToRemove);
	UFUNCTION(BlueprintPure,Category= "Tags")
	bool HasTag(FGameplayTag TagToCheck) const;
	UFUNCTION(BlueprintPure,Category= "Tags")
	bool HasTagExact(FGameplayTag TagToCheck) const;
	UFUNCTION(BlueprintCallable,Category= "Tags")
	bool HasAnyTags(const FGameplayTagContainer& TagsToCheck) const;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Tags")
	TMap<FGameplayTag,int32> ActiveTags;
	
	
protected:
	virtual void BeginPlay() override;
	
	

};
