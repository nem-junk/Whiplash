
#include "Weapons/WWeaponMesh.h"

#include "WCoreTypes.h"
#include "Components/SpotLightComponent.h"


AWWeaponMesh::AWWeaponMesh()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(RootComp);
	FlashLight = CreateDefaultSubobject<USpotLightComponent>("FlashLight");
	FlashLight->SetupAttachment(WeaponMesh,TEXT("FlashLight"));
	FlashLight->SetIntensityUnits(ELightUnits::Lumens);
	FlashLight->SetIntensity(1500.f);
	FlashLight->SetAttenuationRadius(2500.f);
	FlashLight->SetInnerConeAngle(20.f);
	FlashLight->SetOuterConeAngle(35.f);
	FlashLight->SetCastShadows(true);
	FlashLight->SetVisibility(false);
}

void AWWeaponMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWWeaponMesh::TooggleFlashLight()
{
	if (!FlashLight)
	{
		WHIPLASH_LOG(LogWhiplash,Error,TEXT("FlashLight is null"));
		return;
	}
	if (!bIsFlashLightOn)
	{
		FlashLight->SetVisibility(true);
		bIsFlashLightOn = true;
	}
	else
	{
		FlashLight->SetVisibility(false);
		bIsFlashLightOn = false;
	}
}

void AWWeaponMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWWeaponMesh::ToggleFlashLight_Implementation()
{
	TooggleFlashLight();
}

