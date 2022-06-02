// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshWeapon");
	RootComponent = MeshComp;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DefaultWeaponName = FName("");

	WeaponPickedUp = false;
	bReplicates = true;
	bReplicateMovement = true;
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBase, WeaponPickedUp);
}


// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		SetReplicates(true);
	}
	if (DefaultWeaponName != "")
	{
		SetUpWeapon(DefaultWeaponName);
	}
}

void AWeaponBase::SetUpWeapon(FName WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString WpString = FString("AR15DT");
		FWeaponData* WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponName, WpString, true);
		if (WeaponData)
		{
			MeshComp->SetSkeletalMesh(WeaponData->WeaponMesh);
		}
	}
}

void AWeaponBase::Fire()
{

}

void AWeaponBase::OnRep_PickedUp()
{
	WeaponPickedUp = this->WeaponPickedUp;
}

void AWeaponBase::InInventory(bool bIn)
{
	if (Role == ROLE_Authority)
	{
		WeaponPickedUp = bIn;
		OnRep_PickedUp();
	}
}