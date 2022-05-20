// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UENUM(BlueprintType)
enum class EInteractableType : uint8 {
	Statue UMETA(DisplayName = "Statue"),
	UE4Statue UMETA(DisplayName = "UE4Statue")
};

UCLASS()
class THEFARM51TEST_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

	UPROPERTY(EditAnywhere, Category = "Enums")
	EInteractableType InteractableType;
protected:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
