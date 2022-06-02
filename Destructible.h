// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleComponent.h"
#include "Destructible.generated.h"

UCLASS()
class THEFARM51TEST_API ADestructible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructible();

	UFUNCTION()
	void ApplyDamage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);
	UFUNCTION()
	void Trigger(UPrimitiveComponent* OverlapedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void Destroy();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Destructible)
	class UBoxComponent* TriggerComponent;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category= Destructible)
	class UDestructibleComponent* DesComp;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Destructible)
	bool IsDestroyed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destructible)
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destructible)
	float CurrentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destructible)
	float DamageAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destructible)
	float DefaultImpulse;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
