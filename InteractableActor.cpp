// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComp;
	ObjectPickedUp = false;
	bReplicates = true;
	bReplicateMovement = true;
}

void AInteractableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractableActor, ObjectPickedUp);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		SetReplicates(true);
	}
}

void AInteractableActor::OnRep_PickedUp()
{
	this->MeshComp->SetHiddenInGame(ObjectPickedUp);
	this->SetActorEnableCollision(!ObjectPickedUp);
}

void AInteractableActor::InInventory(bool bIn)
{
	if (Role == ROLE_Authority)
	{
		ObjectPickedUp = bIn;
		OnRep_PickedUp();
	}
}

// Called every frame
/*
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

