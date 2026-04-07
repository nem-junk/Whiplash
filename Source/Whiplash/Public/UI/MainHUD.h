
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/WStaminaBarWid.h"

#include "MainHUD.generated.h"



UCLASS()
class WHIPLASH_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	UWStaminaBarWid* StaminaWidget;
};
