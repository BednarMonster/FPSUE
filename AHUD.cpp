// Fill out your copyright notice in the Description page of Project Settings.


#include "AHUD.h"
#include "SUIWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"

void AAHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine && GEngine->GameViewport)
	{
		UE_LOG(LogTemp,Warning,TEXT("Viewport"));
		UIWidget = SNew(SUIWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(WidgetContainer,SWeakWidget).PossiblyNullContent(UIWidget.ToSharedRef()));
	}

}
