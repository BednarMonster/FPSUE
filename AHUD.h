// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AHUD.generated.h"

/**
 * 
 */
UCLASS()
class THEFARM51TEST_API AAHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	TSharedPtr<class SUIWidget> UIWidget;
	TSharedPtr<class SWidget> WidgetContainer;

	UPROPERTY(VisibleAnywhere)
		class UTextBlock* InteractableInfo;

	virtual void BeginPlay() override;
};
