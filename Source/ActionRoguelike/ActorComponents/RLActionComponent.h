// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RLActionComponent.generated.h"

class URLAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, URLActionComponent*, OwningComp, URLAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URLActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;

public:	
	URLActionComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(TSubclassOf<URLAction> ActionClass, AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(URLAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

protected:
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

public:
	/* Returns first occurrence of action matching the class provided */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	URLAction* GetAction(TSubclassOf<URLAction> ActionClass) const;

public:
	// You need add header include because it is not a pointer (that's why forward declaration is not enough),
	// so compiler needs to know size of struct
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
protected:
	UPROPERTY(BlueprintReadOnly,Replicated)
	TArray<URLAction*> Actions;
	
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<URLAction>> DefaultActions;

protected:
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
};
