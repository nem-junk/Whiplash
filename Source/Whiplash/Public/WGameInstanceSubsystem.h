
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WGameInstanceSubsystem.generated.h"

/**
 * this is how you create a global space for console variables in game instance, 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeBoolConsoleVariable,bool,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeIntConsoleVariable,int32,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeFloatConsoleVariable,float,NewValue);
//IConsoleManager::Get().FindConsoleVariable()-searches the engine's console variable registry by name. returns a pointer if found,or null
//SetOnChangedCallback()->registers a function to call whenever this console variable's value changes.
//FConsoleVariableDelegate::CreateUObject(->) creates a delegate bound to the OnChange function/ 
#define REGISTER_CONSOLE_VARIABLE_CHANGE(ConsoleVariableName,Name)\
	if(IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT(ConsoleVariableName)))\
	{Var->AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this,&UWGameInstanceSubsystem::OnChange##Name));}\
	else{check(false);}

UCLASS()
class WHIPLASH_API UWGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	
	//each one here is called when its corresponding console variable changes. It reads the new value from IConsoleVariable using getbool,getint,getfloat then broadcasts it through the multicast delegate
	FORCEINLINE void OnChangeDrawMeshTrajectory(IConsoleVariable* Var) {OnChangeDrawMeshTrajectoryEvent.Broadcast(Var->GetBool());}	
	FORCEINLINE void OnChangeMMDrawQuery(IConsoleVariable* Var) { OnChangeMMDrawQueryEvent.Broadcast(Var->GetBool());}
	FORCEINLINE void OnChangeMMDrawMatch(IConsoleVariable* Var) { OnChangeMMDrawMatchEvent.Broadcast(Var->GetBool());}
	FORCEINLINE void OnChangeDrawCharacterDebugShapes(IConsoleVariable* Var) { OnChangeDrawCharacterDebugShapesEvent.Broadcast(Var->GetBool());}
	FORCEINLINE void OnChangeMMUseStateMachine(IConsoleVariable* Var){OnChangeMMUseStateMachineEvent.Broadcast(Var->GetBool());}
	FORCEINLINE void OnChangeMMUseSets(IConsoleVariable* Var) { OnChangeMMUseSetsEvent.Broadcast(Var->GetBool()); }
	FORCEINLINE void OnChangeDrawDebugLevel(IConsoleVariable* Var) { OnChangeDrawDebugLevelEvent.Broadcast(Var->GetInt()); }
	FORCEINLINE void OnChangeDrawDebugDuration(IConsoleVariable* Var) { OnChangeDrawDebugDurationEvent.Broadcast(Var->GetFloat()); }
	FORCEINLINE void OnChangeMMDatabaseLOD(IConsoleVariable* Var) { OnChangeMMDatabaseLODEvent.Broadcast(Var->GetInt()); }
	FORCEINLINE void OnChangeDrawVisLogShapesForFoleySounds(IConsoleVariable* Var) { OnChangeDrawVisLogShapesForFoleySoundsEvent.Broadcast(Var->GetBool()); }
	FORCEINLINE void OnChangeOffsetRootBoneEnable(IConsoleVariable* Var) { OnChangeOffsetRootBoneEnableEvent.Broadcast(Var->GetBool()); }
	
	
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	FORCEINLINE virtual bool ShouldCreateSubsystem(UObject* Outer) const override {return true;}
	
	UPROPERTY(BlueprintAssignable)
	FOnChangeBoolConsoleVariable OnChangeDrawMeshTrajectoryEvent;
	UPROPERTY(BlueprintAssignable)
	FOnChangeBoolConsoleVariable OnChangeMMDrawQueryEvent;
	UPROPERTY(BlueprintAssignable)
	FOnChangeBoolConsoleVariable OnChangeMMDrawMatchEvent;
	UPROPERTY(BlueprintAssignable)
	FOnChangeBoolConsoleVariable OnChangeDrawCharacterDebugShapesEvent;
	UPROPERTY(BlueprintAssignable)
	FOnChangeBoolConsoleVariable OnChangeMMUseStateMachineEvent;
	UPROPERTY(BlueprintAssignable)
	FOnChangeBoolConsoleVariable OnChangeMMUseSetsEvent;
	UPROPERTY(BlueprintAssignable)
	FOnChangeIntConsoleVariable OnChangeDrawDebugLevelEvent;
	UPROPERTY(BlueprintAssignable)
	FOnChangeFloatConsoleVariable OnChangeDrawDebugDurationEvent;
	UPROPERTY(BlueprintAssignable)
	FOnChangeIntConsoleVariable OnChangeMMDatabaseLODEvent;
	UPROPERTY(BlueprintAssignable)
	FOnChangeBoolConsoleVariable OnChangeDrawVisLogShapesForFoleySoundsEvent;
	UPROPERTY(BlueprintAssignable)
	FOnChangeBoolConsoleVariable OnChangeOffsetRootBoneEnableEvent;
};
