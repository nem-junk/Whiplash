#include "AWAttributeSetTester.h"
#include "WCoreTypes.h"

AWAttributeSetTester::AWAttributeSetTester()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AWAttributeSetTester::BeginPlay()
{
    Super::BeginPlay();

    // Create AttributeSet with this actor as outer
    TestAttributeSet = NewObject<UWAttributeSet>(this);
    if (!TestAttributeSet)
    {
        WHIPLASH_LOG(LogWhiplashAbility, Error, TEXT("[TESTER] Failed to create AttributeSet"));
        return;
    }

    // Push config values from tester into AttributeSet
    TestAttributeSet->StaminaDrainMag      = StaminaDrainMag;
    TestAttributeSet->StaminaDrainDuration = StaminaDrainDuration;
    TestAttributeSet->StaminaDrainInterval = StaminaDrainInterval;
    TestAttributeSet->StaminaRegenMag      = StaminaRegenMag;
    TestAttributeSet->StaminaRegenDuration = StaminaRegenDuration;
    TestAttributeSet->StaminaRegenInterval = StaminaRegenInterval;
    TestAttributeSet->RegenDelay           = RegenDelay;
    TestAttributeSet->StaminaDrainID       = StaminaDrainID;
    TestAttributeSet->StaminaRegenID       = StaminaRegenID;

    TestAttributeSet->InitializeAttributes();
    BindDelegates();

    WHIPLASH_LOG(LogWhiplashAbility, Log, TEXT("=== AttributeSet Tester Initialized ==="));
    Test_PrintCurrentValues();
}

void AWAttributeSetTester::BindDelegates()
{
    TestAttributeSet->OnHealthChanged.AddUObject(this, &AWAttributeSetTester::OnHealthChanged);
    TestAttributeSet->OnStaminaChanged.AddUObject(this, &AWAttributeSetTester::OnStaminaChanged);
    TestAttributeSet->OnOutOfHealth.AddUObject(this, &AWAttributeSetTester::OnOutOfHealth);
    TestAttributeSet->OnOutOfStamina.AddUObject(this, &AWAttributeSetTester::OnOutOfStamina);
}

// ── Delegate Callbacks ───────────────────────────────────────────

void AWAttributeSetTester::OnHealthChanged(float OldValue, float NewValue)
{
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[DELEGATE] Health: %.1f → %.1f"), OldValue, NewValue);
}

void AWAttributeSetTester::OnStaminaChanged(float OldValue, float NewValue)
{
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[DELEGATE] Stamina: %.1f → %.1f"), OldValue, NewValue);
}

void AWAttributeSetTester::OnOutOfHealth()
{
    WHIPLASH_LOG(LogWhiplashAbility, Warning,
        TEXT("[DELEGATE] ========= OUT OF HEALTH FIRED ========="));
}

void AWAttributeSetTester::OnOutOfStamina()
{
    WHIPLASH_LOG(LogWhiplashAbility, Warning,
        TEXT("[DELEGATE] ========= OUT OF STAMINA FIRED ========="));
}

// ── Utility ─────────────────────────────────────────────────────

void AWAttributeSetTester::Test_PrintCurrentValues()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log, TEXT("────────────────────────────────────────"));
    WHIPLASH_LOG(LogWhiplashAbility, Log, TEXT("[VALUES] Health:  %.1f / %.1f"),
        TestAttributeSet->GetHealth(), TestAttributeSet->GetMaxHealth());
    WHIPLASH_LOG(LogWhiplashAbility, Log, TEXT("[VALUES] Stamina: %.1f / %.1f"),
        TestAttributeSet->GetStamina(), TestAttributeSet->GetMaxStamina());
    WHIPLASH_LOG(LogWhiplashAbility, Log, TEXT("────────────────────────────────────────"));
}

void AWAttributeSetTester::Test_ResetAttributes()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log, TEXT("[RESET] Resetting all attributes"));
    TestAttributeSet->InitializeAttributes();
    Test_PrintCurrentValues();
}

// ── Health Tests ─────────────────────────────────────────────────

void AWAttributeSetTester::Test_ApplyDamage()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] ApplyDamage: %.1f"), TestDamageAmount);
    TestAttributeSet->ApplyDamage(TestDamageAmount);
    Test_PrintCurrentValues();
}

void AWAttributeSetTester::Test_ApplyHealing()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] ApplyHealing: %.1f"), TestHealAmount);
    TestAttributeSet->ApplyHealing(TestHealAmount);
    Test_PrintCurrentValues();
}

void AWAttributeSetTester::Test_KillPlayer()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] Applying lethal damage 9999 — OnOutOfHealth should fire ONCE"));
    TestAttributeSet->ApplyDamage(9999.f);
    Test_PrintCurrentValues();
}

void AWAttributeSetTester::Test_DamageWhileDead()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] Applying damage while dead — OnOutOfHealth should NOT fire again"));
    TestAttributeSet->ApplyDamage(50.f);
    Test_PrintCurrentValues();
}

void AWAttributeSetTester::Test_HealWhileDead()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] Healing while dead — Health should come back up"));
    TestAttributeSet->ApplyHealing(50.f);
    Test_PrintCurrentValues();
}

void AWAttributeSetTester::Test_OverHeal()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] OverHeal 9999 — Health should clamp to MaxHealth"));
    TestAttributeSet->ApplyHealing(9999.f);
    Test_PrintCurrentValues();
}

// ── Stamina Tests ────────────────────────────────────────────────

void AWAttributeSetTester::Test_StartDrain()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] Starting Stamina Drain — watch log for OnStaminaChanged"));
    TestAttributeSet->StartStaminaDrain();
}

void AWAttributeSetTester::Test_StopDrain()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] Stopping Drain — regen delay starts (%.1fs)"), RegenDelay);
    TestAttributeSet->StopStaminaDrain();
}

void AWAttributeSetTester::Test_DrainToZero()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] Draining Stamina directly to 0 — OnOutOfStamina should fire ONCE"));
    TestAttributeSet->ApplyDamage(0.f); // health untouched
    // Directly drain stamina via instant modifier
    FAttributeModifier DrainAll(
        -9999.f, 0.f, 0.f,
        EModifierOperation::Add,
        EAttributeTarget::Stamina,
        FGameplayTag::EmptyTag);
    TestAttributeSet->ApplyModifier(DrainAll);
    Test_PrintCurrentValues();
}

void AWAttributeSetTester::Test_StartRegenManually()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] Starting regen manually — skipping delay"));
    TestAttributeSet->StartStaminaRegen();
}

// ── Modifier Tests ───────────────────────────────────────────────

void AWAttributeSetTester::Test_InstantHealthModifier()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] Instant +20 Health modifier — should apply immediately"));
    FAttributeModifier InstantHeal(
        20.f, 0.f, 0.f,
        EModifierOperation::Add,
        EAttributeTarget::Health,
        FGameplayTag::EmptyTag);
    TestAttributeSet->ApplyModifier(InstantHeal);
    Test_PrintCurrentValues();
}

void AWAttributeSetTester::Test_TimedHealthModifier()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] Poison: -10 Health every 1s for 5s — watch log"));
    FAttributeModifier Poison(
        -10.f, 5.f, 1.f,
        EModifierOperation::Add,
        EAttributeTarget::Health,
        FGameplayTag::EmptyTag);
    TestAttributeSet->ApplyModifier(Poison);
}

void AWAttributeSetTester::Test_MultiplyModifier()
{
    if (!TestAttributeSet) return;
    WHIPLASH_LOG(LogWhiplashAbility, Log,
        TEXT("[TEST] Multiply Health x0.5 instant — Health should halve"));
    FAttributeModifier HalfHealth(
        0.5f, 0.f, 0.f,
        EModifierOperation::Multiply,
        EAttributeTarget::Health,
        FGameplayTag::EmptyTag);
    TestAttributeSet->ApplyModifier(HalfHealth);
    Test_PrintCurrentValues();
}