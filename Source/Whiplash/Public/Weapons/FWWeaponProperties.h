#pragma once
#include "CoreMinimal.h"

#include "FWWeaponProperties.generated.h"
class UAnimMontage;
class UUserWidget;
class UTexture2D;

USTRUCT(BlueprintType)
struct FWeaponHandling
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Weapon|Handling")
	float RoundsPerMinute;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Handling")
	float SpreadAngleBase;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Handling")
	float SpreadAngleMultiplier_ADS;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Handling")
	float SpreadAngleMultiplier_HipFire;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Handling")
	float SpreadAngleMultiplier_Crouch;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Handling")
	float SpreadAngleMultiplier_Moving;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Handling")
	float SpreadAngleMultiplier_Falling;// remove 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Handling")
	float SpreadAngleAccumulationPerShot;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Handling")
	float SpreadAngleAccumulationDecayPerSecond;
	
	FWeaponHandling (float InRoundsPerMinute=450,
		float InSpreadAngleBase=1.0f,
		float InSpreadAngleMultiplier_ADS=0.2f,
		float InSpreadAngleMultiplier_HipFire=3.5f,
		float InSpreadAngleMultiplier_Crouch=0.85f,							/*Default Values are from Lyra, will update them in Blueprint*/
		float InSpreadAngleMultiplier_Moving=1.16670f,
		float InSpreadAngleMultiplier_Falling=2.0f,
		float InSpreadAngleAccumulationPerShot=0.098f,
		float InSpreadAngleAccumulationDecayPerSecond=11.09f):
	RoundsPerMinute(InRoundsPerMinute),
	SpreadAngleBase(InSpreadAngleBase),
	SpreadAngleMultiplier_ADS(InSpreadAngleMultiplier_ADS),
	SpreadAngleMultiplier_HipFire(InSpreadAngleMultiplier_HipFire),
	SpreadAngleMultiplier_Crouch(InSpreadAngleMultiplier_Crouch),
	SpreadAngleMultiplier_Moving(InSpreadAngleMultiplier_Moving),
	SpreadAngleMultiplier_Falling(InSpreadAngleMultiplier_Falling),
	SpreadAngleAccumulationPerShot(InSpreadAngleAccumulationPerShot),
	SpreadAngleAccumulationDecayPerSecond(InSpreadAngleAccumulationDecayPerSecond)
	{
		
	}
	
};

USTRUCT(BlueprintType)
struct FRecoilProperties
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Recoil")
	float RecoilPitch;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Recoil")
	float RecoilYawMin;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Recoil")
	float RecoilYawMax;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Recoil")
	float FirstShotRecoilMultiplier;
	
	FRecoilProperties (float InRecoilPitch=0.320f,
		float InRecoilYawMin=-0.120f,
		float InRecoilYawMax=0.280f,/*when adding to Character or Weapon Clamp the min.max values */
		float InFirstShotRecoilMultiplier=2.3f):
	RecoilPitch(InRecoilPitch),
	RecoilYawMin(InRecoilYawMin),
	RecoilYawMax(InRecoilYawMax),
	FirstShotRecoilMultiplier(InFirstShotRecoilMultiplier)
	{
		
	}
	
};

USTRUCT(BlueprintType)
struct FAmmunition
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Ammunition")
	int32 MagazineSize;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Ammunition")
	int32 BulletsPerCartridge;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Ammunition")
	int32 MaxReserveAmmo;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Ammunition")
	bool bCanHold1InChamber;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Ammunition")
	bool bIsAutomatic;
	
	FAmmunition (float InMagazineSize=12.0f,
		float InBulletsPerCartridge=1.0f,
		float InMaxReserveAmmo=48.0f,
		bool InCanHold1InChamber=true,
		bool InIsAutomatic=false):
	MagazineSize(InMagazineSize),
	BulletsPerCartridge(InBulletsPerCartridge),
	MaxReserveAmmo(InMaxReserveAmmo),
	bCanHold1InChamber(InCanHold1InChamber),
	bIsAutomatic(InIsAutomatic)
	{
		
	}
};

USTRUCT(BlueprintType)
struct FWeaponMesh
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|MEsh")
	TSubclassOf<AActor> WeaponMeshClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|MEsh") // camelcase my ASS
	FName AttachSocketName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|MEsh")
	FTransform AttachSocketTransform;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|MEsh|Animation")
	TObjectPtr<UAnimMontage> EquipMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|MEsh|Animation")
	TObjectPtr<UAnimMontage> UnEquipMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|MEsh|Animation")
	TObjectPtr<UAnimMontage> FireMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|MEsh|Animation")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
};

USTRUCT(BlueprintType)
struct FWeaponUI
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|UI")
	TSubclassOf<UUserWidget>  ReticleWidgetClass;
	/*
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|MEsh|Animation") add Division Like Widget for Weapon always wanted to do this 
	TSubClassOf<UUserWidget> DivisionLikeWidgetClass;
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|UI")
	TObjectPtr<UTexture2D> WeaponIcon;
};
