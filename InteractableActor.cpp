// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComp;
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

// Called every frame
/*
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

