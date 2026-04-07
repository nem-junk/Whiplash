
#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

////////////////////////////////////////
#include "WStaminaBarWid.generated.h"

UCLASS()
class WHIPLASH_API UWStaminaBarWid : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UProgressBar> StaminaLost;
	
	UFUNCTION(BlueprintCallable)
	void UpdateStaminaBar(float CurrentStamina, float MaxStamina);

};
