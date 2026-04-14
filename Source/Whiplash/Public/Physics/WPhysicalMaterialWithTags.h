
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Tags/WGameplayTags.h"
////////////////////////////////////////////////
#include "WPhysicalMaterialWithTags.generated.h"

class UObject;

UCLASS()
class WHIPLASH_API UWPhysicalMaterialWithTags : public UPhysicalMaterial
{
	GENERATED_BODY()
public:
	UWPhysicalMaterialWithTags(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PhysicalProperties")
	FGameplayTagContainer Tags;
};
