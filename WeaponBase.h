// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "WeaponBase.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere)
	FString WeaponName;
};

UCLASS()
class THEFARM51TEST_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	class UDataTable* WeaponDataTable;

	UPROPERTY(ReplicatedUsing = OnRep_PickedUp)
	bool WeaponPickedUp;



public:	
	UPROPERTY(EditAnywhere)
	FName DefaultWeaponName;

	UFUNCTION()
	void OnRep_PickedUp();
	void InInventory(bool bIn);

	void SetUpWeapon(FName WeaponName);
	void Fire();
};
