// F
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

/////////////////////////////////////////
#include "WAbilityComponent.generated.h"
/////////////////////////////////////////

class UWAbility;
class UWTagComponent;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WHIPLASH_API UWAbilityComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class UWAbility;
	friend class AWCharacter;

public:

		
	UWAbilityComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable,Category="Abilities")
	void AddAbility(AActor* Instigator,TSubclassOf<UWAbility> AbilityClass);
	
	UFUNCTION(BlueprintCallable,Category="Abilities")
	UWAbility* GetAbility (TSubclassOf<UWAbility> AbilityClass);

	UFUNCTION(BlueprintCallable,Category="Abilities")
	bool StartAbilityByTag(AActor* Instigator,FGameplayTag AbilityName);
	
	UFUNCTION(BlueprintCallable,Category="Abilities")
	bool StopAbilityByTag(AActor*Instigator, FGameplayTag AbilityName);
	
	UFUNCTION(BlueprintCallable,Category="Abilities")
	void RemoveAbility(UWAbility* AbilityToRemove);

protected:
	UPROPERTY(BlueprintReadOnly,Category="Tags")
	UWTagComponent* TagComponent;
	UPROPERTY(BlueprintReadOnly,Category="Abilities")
	TArray<UWAbility*> Abilities;
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UWAbility>> DefaultAbilities;
	
	
	
	virtual void BeginPlay() override;
	
/*for replication */
	/*UFUNCTION(Server,Reliable)
	void ServerStartAction(AActor*Instigator, FName ActionName);
	UFUNCTION(Server,Reliable)
	void ServerStopAction(AActor*Instigator, FName ActionName);*/
	
	

};
