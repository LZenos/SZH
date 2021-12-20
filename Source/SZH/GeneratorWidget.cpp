// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorWidget.h"
#include "Generator.h"
#include "Overdrivable.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UGeneratorWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	leakBreakerButtons.Add(btn_leakBreaker_1);
	leakBreakerButtons.Add(btn_leakBreaker_2);
	leakBreakerButtons.Add(btn_leakBreaker_3);
	leakBreakerButtons.Add(btn_leakBreaker_4);
	leakBreakerButtons.Add(btn_leakBreaker_5);

	overdriveButtons.Add(btn_overdrive_1);
	overdriveButtons.Add(btn_overdrive_2);
	overdriveButtons.Add(btn_overdrive_3);
	overdriveButtons.Add(btn_overdrive_4);
	overdriveButtons.Add(btn_overdrive_5);
}

void UGeneratorWidget::InitOwner(AGenerator* Owner)
{
	generatorActor = Owner;
}

void UGeneratorWidget::UpdateGeneratorLevel(int NewLevel)
{
	txt_GeneratorLevel->SetText(FText::FromString(FString::FromInt(NewLevel)));
}
void UGeneratorWidget::UpdateElectricSurplus(int Surplus)
{
	txt_CurrentPower->SetText(FText::FromString(FString::FromInt(Surplus)));
}


void UGeneratorWidget::SwitchOperation(int ObjectIndex)
{
	bool res = generatorActor->SwitchElectricState(ObjectIndex);

	if (res)
	{
		FButtonStyle newButtonLook;
		FSlateBrush newImage;
		FSlateSound newSound;

		switch (generatorActor->GetCurrentElectricState(ObjectIndex))
		{
		case EElectricState::E_PAUSE:
			newImage.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/PlayScene/Cluster_Off.Cluster_Off")));
			break;

		case EElectricState::E_OPERATE:
			newImage.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/PlayScene/Cluster_On.Cluster_On")));
			break;
		}
		newSound.SetResourceObject(LoadObject<USoundWave>(nullptr, TEXT("/Game/Sound/UI/TurnCluster.TurnCluster")));
		newButtonLook.SetNormal(newImage);
		newButtonLook.SetHovered(newImage);
		newButtonLook.SetPressed(newImage);
		newButtonLook.SetPressedSound(newSound);

		leakBreakerButtons[ObjectIndex]->SetStyle(newButtonLook);
	}
}
void UGeneratorWidget::SetOverdrive(int ObjectIndex)
{
	bool res = generatorActor->OverdriveStart(ObjectIndex);

	if (res)
	{
		FButtonStyle newButtonLookOverdrive;
		FSlateBrush newImageOverdrive;

		newImageOverdrive.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/PlayScene/Btn_OverCharge_On.Btn_OverCharge_On")));
		newButtonLookOverdrive.SetNormal(newImageOverdrive);
		newButtonLookOverdrive.SetHovered(newImageOverdrive);
		newButtonLookOverdrive.SetPressed(newImageOverdrive);
		overdriveButtons[ObjectIndex]->SetStyle(newButtonLookOverdrive);


		FButtonStyle newButtonLookBreaker;
		FSlateBrush newImageBreaker;

		newImageBreaker.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/PlayScene/Cluster_On.Cluster_On")));
		newButtonLookBreaker.SetNormal(newImageBreaker);
		newButtonLookBreaker.SetHovered(newImageBreaker);
		newButtonLookBreaker.SetPressed(newImageBreaker);
		leakBreakerButtons[ObjectIndex]->SetStyle(newButtonLookBreaker);
	}
}

void UGeneratorWidget::ForceOffLeakBreaker(int ObjectIndex)
{
	FButtonStyle newButtonLook;
	FSlateBrush newImage;

	newImage.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/PlayScene/Cluster_Off.Cluster_Off")));
	newButtonLook.SetNormal(newImage);
	newButtonLook.SetHovered(newImage);
	newButtonLook.SetPressed(newImage);
	leakBreakerButtons[ObjectIndex]->SetStyle(newButtonLook);
}
void UGeneratorWidget::ActivateOverdriveButton(int ObjectIndex)
{
	FButtonStyle newButtonLook;
	FSlateBrush newImageOn;
	FSlateBrush newImageOff;
	FSlateSound newSound;

	newImageOn.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/PlayScene/Btn_OverCharge_On.Btn_OverCharge_On")));
	newImageOff.SetResourceObject(LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/PlayScene/Btn_OverCharge_Off.Btn_OverCharge_Off")));
	newSound.SetResourceObject(LoadObject<USoundWave>(nullptr, TEXT("/Game/Sound/UI/TurnCluster.TurnCluster")));
	newButtonLook.SetNormal(newImageOff);
	newButtonLook.SetHovered(newImageOff);
	newButtonLook.SetPressed(newImageOn);
	newButtonLook.SetPressedSound(newSound);
	overdriveButtons[ObjectIndex]->SetStyle(newButtonLook);
}