// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTrace.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "Public/DrawDebugHelpers.h"
// Sets default values for this component's properties
ULineTrace::ULineTrace()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULineTrace::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

AActor* ULineTrace::LineTraceSingle(FVector Start, FVector End)
{
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionParams;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	
	if (GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		Start,
		End,
		CollisionParams,
		CollisionQueryParams
	))
	{
		return HitResult.GetActor();
	}
	else {
		return nullptr;
	}
	
}

AActor* ULineTrace::LineTraceSingle(FVector Start, FVector End, bool ShowDebugLine)
{
	AActor* Actor = LineTraceSingle(Start,End);
	if (ShowDebugLine)
	{
		DrawDebugLine(
			GetWorld(),
			Start,End,
			FColor::Red,
			false,
			3.0f,
			0,
			5.0f
		);
	}
	return Actor;
}


// Called every frame
/*
void ULineTrace::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/

