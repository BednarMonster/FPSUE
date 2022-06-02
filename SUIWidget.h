// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"


class  SUIWidget : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SUIWidget){}
	SLATE_ARGUMENT(TWeakObjectPtr<class AHUD>, OwningHUD)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	TWeakObjectPtr<class UGameHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override { return true; };

	FText Pickable;
};
