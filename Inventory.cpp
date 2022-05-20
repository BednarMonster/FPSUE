// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Net/UnrealNetwork.h"
#include "InteractableActor.h"

// Sets default values for this component's properties
UInventory::UInventory()
{
	
}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UInventory::AddItem(AInteractableActor* Item)
{
	Items.Add(Item);
	for (AInteractableActor* IActor : Items)
	{
		UE_LOG(LogTemp,Warning,TEXT("Item: %s"), *IActor->GetName());
	}
	return false;
}

void UInventory::RemoveItem(AInteractableActor* Item)
{
	
}

void UInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventory, Items);
}