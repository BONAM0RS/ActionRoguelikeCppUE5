// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RLActionComponent.generated.h"


class URLAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URLActionComponent : public UActorComponent
{
	GENERATED_BODY()

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

public:
	// You need add header include because it is not a pointer (that's why forward declaration is not enough),
	// so compiler needs to know size of struct
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
protected:
	UPROPERTY()
	TArray<URLAction*> Actions;
	
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<URLAction>> DefaultActions;
	
};
