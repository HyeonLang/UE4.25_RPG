// Fill out your copyright notice in the Description page of Project Settings.


#include "CIconWidget.h"

#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Engine/StreamableManager.h"
#include "Global.h"
#include "Interfaces/CIconInterface.h"



bool UCIconWidget::Initialize()
{
	bool result = Super::Initialize();

	return result;
}

void UCIconWidget::SetIcon(UTexture2D* Icon)
{
	if (Icon)
		IconImage->SetBrushFromTexture(Icon);
}

