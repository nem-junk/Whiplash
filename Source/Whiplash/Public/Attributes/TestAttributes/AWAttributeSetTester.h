#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attributes/WAttributeSet.h"
#include "AWAttributeSetTester.generated.h"

UCLASS()
class WHIPLASH_API AWAttributeSetTester : public AActor
{
    GENERATED_BODY()

public:
    AWAttributeSetTester();

    UPROPERTY()
    UWAttributeSet* TestAttributeSet;

    // ── Health Test Config ───────────────────────
    UPROPERTY(EditAnywhere, Category="Test|Health")
    float TestDamageAmount = 30.f;

    UPROPERTY(EditAnywhere, Category="Test|Health")
    float TestHealAmount = 20.f;

    // ── Stamina Config ───────────────────────────
    UPROPERTY(EditAnywhere, Category="Test|StaminaConfig")
    float StaminaDrainMag = 15.f;

    UPROPERTY(EditAnywhere, Category="Test|StaminaConfig")
    float StaminaDrainDuration = -1.f;

    UPROPERTY(EditAnywhere, Category="Test|StaminaConfig")
    float StaminaDrainInterval = 0.1f;

    UPROPERTY(EditAnywhere, Category="Test|StaminaConfig")
    float StaminaRegenMag = 10.f;

    UPROPERTY(EditAnywhere, Category="Test|StaminaConfig")
    float StaminaRegenDuration = -1.f;

    UPROPERTY(EditAnywhere, Category="Test|StaminaConfig")
    float StaminaRegenInterval = 0.1f;

    UPROPERTY(EditAnywhere, Category="Test|StaminaConfig")
    float RegenDelay = 1.5f;

    UPROPERTY(EditAnywhere, Category="Test|StaminaConfig")
    FGameplayTag StaminaDrainID;

    UPROPERTY(EditAnywhere, Category="Test|StaminaConfig")
    FGameplayTag StaminaRegenID;

    // ── Health Tests ─────────────────────────────
    UFUNCTION(CallInEditor, Category="Test|Health")
    void Test_ApplyDamage();

    UFUNCTION(CallInEditor, Category="Test|Health")
    void Test_ApplyHealing();

    UFUNCTION(CallInEditor, Category="Test|Health")
    void Test_KillPlayer();

    UFUNCTION(CallInEditor, Category="Test|Health")
    void Test_DamageWhileDead();

    UFUNCTION(CallInEditor, Category="Test|Health")
    void Test_HealWhileDead();

    UFUNCTION(CallInEditor, Category="Test|Health")
    void Test_OverHeal();

    // ── Stamina Tests ────────────────────────────
    UFUNCTION(CallInEditor, Category="Test|Stamina")
    void Test_StartDrain();

    UFUNCTION(CallInEditor, Category="Test|Stamina")
    void Test_StopDrain();

    UFUNCTION(CallInEditor, Category="Test|Stamina")
    void Test_DrainToZero();

    UFUNCTION(CallInEditor, Category="Test|Stamina")
    void Test_StartRegenManually();

    // ── Modifier Tests ───────────────────────────
    UFUNCTION(CallInEditor, Category="Test|Modifiers")
    void Test_InstantHealthModifier();

    UFUNCTION(CallInEditor, Category="Test|Modifiers")
    void Test_TimedHealthModifier();

    UFUNCTION(CallInEditor, Category="Test|Modifiers")
    void Test_MultiplyModifier();

    // ── Utility ──────────────────────────────────
    UFUNCTION(CallInEditor, Category="Test|Utility")
    void Test_PrintCurrentValues();

    UFUNCTION(CallInEditor, Category="Test|Utility")
    void Test_ResetAttributes();

protected:
    virtual void BeginPlay() override;

private:
    void BindDelegates();
    void OnHealthChanged(float OldValue, float NewValue);
    void OnStaminaChanged(float OldValue, float NewValue);
    void OnOutOfHealth();
    void OnOutOfStamina();
};