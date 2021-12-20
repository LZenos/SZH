// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UMessageWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	msgType = EMessageType::E_UPGRADE_MAX;
	alpha = 0.0f;
	decrease = 0.5f;

	img_background->SetOpacity(alpha);
	txt_upgradeMax->SetOpacity(alpha);
	txt_requireKit->SetOpacity(alpha);
	txt_carryingKit->SetOpacity(alpha);
	txt_requireResource->SetOpacity(alpha);
	txt_destroyed->SetOpacity(alpha);
	txt_resourceNotReady->SetOpacity(alpha);
}

void UMessageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (alpha > 0.0f)
	{
		alpha -= (InDeltaTime * decrease);
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
		}

		img_background->SetOpacity(alpha * 0.7f);
		switch (msgType)
		{
		case EMessageType::E_UPGRADE_MAX:
			txt_upgradeMax->SetOpacity(alpha);
			break;

		case EMessageType::E_REQUIRE_KIT:
			txt_requireKit->SetOpacity(alpha);
			break;

		case EMessageType::E_CARRYING_KIT:
			txt_carryingKit->SetOpacity(alpha);
			break;

		case EMessageType::E_REQUIRE_RESOURCE:
			txt_requireResource->SetOpacity(alpha);
			break;

		case EMessageType::E_DESTROYED:
			txt_destroyed->SetOpacity(alpha);
			break;

		case EMessageType::E_RESOURCE_NOT_READY:
			txt_resourceNotReady->SetOpacity(alpha);
		}
	}
	else
	{
		alpha = 0.0f;
	}
}


void UMessageWidget::Display(EMessageType message)
{
	alpha = 0.0f;
	img_background->SetOpacity(alpha);
	txt_upgradeMax->SetOpacity(alpha);
	txt_requireKit->SetOpacity(alpha);
	txt_carryingKit->SetOpacity(alpha);
	txt_requireResource->SetOpacity(alpha);
	txt_destroyed->SetOpacity(alpha);
	txt_resourceNotReady->SetOpacity(alpha);
	
	msgType = message;
	alpha = 1.0f;
}