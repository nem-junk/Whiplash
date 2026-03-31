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

public:
	UWAbilityComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable,Category="Abilities")
	void AddAbility(AActor* Instigator,TSubclassOf<UWAbility> AbilityClass);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;


protected:
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadOnly,Category="Tags")
	UWTagComponent* TagComponent;
	UPROPERTY(BlueprintReadOnly,Category="Abilities")
	TArray<UWAbility*> Abilities;
	
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UWAbility>> DefaultAbilities;
	
	
	

};
