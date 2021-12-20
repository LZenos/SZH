// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageManager.h"


AMessageManager::AMessageManager()
{
 	PrimaryActorTick.bCanEverTick = false;

}

void AMessageManager::BeginPlay()
{
	Super::BeginPlay();
	
	FStringClassReference tempWidgetClassRef(TEXT("/Game/Created/Blueprints/UI/InteractMessageUI_BP.InteractMessageUI_BP_C"));
	if (UClass* tempWidgetClass = tempWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* tempWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), tempWidgetClass);
		messageWidget = Cast<UMessageWidget>(tempWidget);
		messageWidget->AddToViewport();
	}
}

void AMessageManager::DisplayMessage(EMessageType message)
{
	if (!messageWidget->IsInViewport())
	{
		messageWidget->AddToViewport();
	}

	messageWidget->Display(message);
}