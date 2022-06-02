// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Net/UnrealNetwork.h"
#include "InteractableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UInventory::UInventory()
{
	bReplicates = true;
}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventory, Items, COND_OwnerOnly);
}

bool UInventory::AddItem(AInteractableActor* Item)
{
	Items.Add(Item);
	Item->InInventory(true);
	Item->SetActorHiddenInGame(true);
	MultiPickedupItem(Item);
	for (AInteractableActor* IActor : Items)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, __FUNCTION__);
		UE_LOG(LogTemp, Warning, TEXT("Item: %s"), *IActor->GetName());
	}
	return false;
}

bool UInventory::MultiPickedupItem_Validate(AInteractableActor* Item)
{
	return true;
}

void UInventory::MultiPickedupItem_Implementation(AInteractableActor* Item)
{
	Item->SetActorEnableCollision(false);
}

void UInventory::RemoveItem(AInteractableActor* Item)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		FVector Location = GetOwner()->GetActorLocation();
		FVector EndRay = Location;
		EndRay.Z -= 500.0f;
		FHitResult Hitresult;
		FCollisionObjectQueryParams ObjQuery;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());
		GetWorld()->LineTraceSingleByObjectType(
		OUT Hitresult,
		Location,
		EndRay,
		ObjQuery,
		QueryParams
		);

		if (Hitresult.ImpactPoint != FVector::ZeroVector)
		{
			Location = Hitresult.ImpactPoint;
		}
		Item->SetActorLocation(Location);

		Item->InInventory(false);

	}
	Items.Empty();
}

TArray<class AInteractableActor*> UInventory::GetItems()
{
	return Items;
}

int32 UInventory::GetItemsAmount()
{
	return Items.Num()-1;
}
