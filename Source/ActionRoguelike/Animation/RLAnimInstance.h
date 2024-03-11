/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RLAnimInstance.generated.h"


class URLAction_Effect;
class URLActionComponent;

UCLASS()
class ACTIONROGUELIKE_API URLAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Config")
	URLActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<URLAction_Effect> StunningActionEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UAnimSequenceBase* StunAnimSequence;
	
	// Is Pawn Stunned based on GameplayTag data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	bool bIsStunned;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	bool bIsTimeToEndStun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float StunAnimDuration;
};
