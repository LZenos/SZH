// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageWidget.generated.h"

UENUM(BlueprintType)
enum class EMessageType : uint8
{
	E_UPGRADE_MAX UMETA(DisplayName = "UpgradeMax"),
	E_REQUIRE_KIT UMETA(DisplayName = "RequireKit"),
	E_CARRYING_KIT UMETA(DisplayName = "CarryingKit"),
	E_REQUIRE_RESOURCE UMETA(DisplayName = "RequireResource"),
	E_DESTROYED UMETA(DisplayName = "Destroyed"),
	E_RESOURCE_NOT_READY UMETA(DisplayName = "ResrouceNotReady")
};

UCLASS()
class SZH_API UMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	EMessageType msgType;

	float alpha;
	float decrease;

public:
	UPROPERTY(Meta = (BindWidget))
	class UImage* img_background;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* txt_upgradeMax;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* txt_requireKit;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* txt_carryingKit;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* txt_requireResource;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* txt_destroyed;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* txt_resourceNotReady;


	void Display(EMessageType message);
};
