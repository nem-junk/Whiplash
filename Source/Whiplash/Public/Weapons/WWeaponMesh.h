
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Interface/IFlashLight.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "WWeaponMesh.generated.h"

class USpotLightComponent;

UCLASS()
class WHIPLASH_API AWWeaponMesh : public AActor ,public IIFlashLight
{
	GENERATED_BODY()

public:
	AWWeaponMesh();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USceneComponent> RootComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USpotLightComponent> FlashLight;

protected:
	virtual void BeginPlay() override;
	void TooggleFlashLight();
	bool bIsFlashLightOn = false;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void ToggleFlashLight_Implementation() override;
};
