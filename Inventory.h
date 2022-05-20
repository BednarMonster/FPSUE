// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEFARM51TEST_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

protected:
	UPROPERTY(Replicated)
	TArray<class AInteractableActor*> Items;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	bool AddItem(class AInteractableActor* Item);
	void RemoveItem(class AInteractableActor* Item);
};