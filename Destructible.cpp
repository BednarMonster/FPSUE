// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible.h"
#include "DestructibleComponent.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADestructible::ADestructible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	DesComp = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Component"));
	DesComp->SetupAttachment(RootComponent);
	DesComp->SetNotifyRigidBodyCollision(true);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Component"));
	TriggerComponent->SetupAttachment(DesComp);

	MaxHealth = 1.0f;

	DamageAmount = 1.0f;
	DefaultImpulse = 1.0f;

	bReplicates = true;
	bReplicateMovement = true;
}

void ADestructible::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADestructible, DesComp);
	DOREPLIFETIME(ADestructible, TriggerComponent);
}


void ADestructible::ApplyDamage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,__FUNCTION__);
	if (!IsDestroyed && OtherComp)
	{
		CurrentHealth-=1.f;
		if (CurrentHealth <= 0.f)
		{
			IsDestroyed = true;
			Destroy();
		}
	}
}

void ADestructible::Trigger(UPrimitiveComponent* OverlapedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, __FUNCTION__);
	if (!IsDestroyed && OtherComp)
	{
		CurrentHealth -= 1.f;
		if (CurrentHealth <= 0.f)
		{
			IsDestroyed = true;
			Destroy();
		}
	}
}

void ADestructible::Destroy()
{
	if (IsDestroyed && CurrentHealth <=0)
	{
		//DesComp->SetNotifyRigidBodyCollision(false);
		//DesComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

// Called when the game starts or when spawned
void ADestructible::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		SetReplicates(true);
	}
	DesComp->OnComponentHit.AddDynamic(this, &ADestructible::ApplyDamage);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestructible::Trigger);

	CurrentHealth = MaxHealth;
}

// Called every frame
void ADestructible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

