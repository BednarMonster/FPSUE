// Fill out your copyright notice in the Description page of Project Settings.


#include "SUIWidget.h"

#define LOCTEXT_NAMESPACE "UI"

void SUIWidget::Construct(const FArguments& InArgs)
{
	
	const FMargin ContentPadding = FMargin(500.0f, 300.f);

	Pickable = LOCTEXT("Pickable", "Press F to interact");

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.ColorAndOpacity(FColor::Black)
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(ContentPadding)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Text(Pickable)
				]
			]
	];

}

#undef LOCTEXT_NAMESPACE