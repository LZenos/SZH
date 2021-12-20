// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MessageWidget.h"
#include "MessageManager.generated.h"

UCLASS()
class SZH_API AMessageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AMessageManager();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UMessageWidget* messageWidget;

public:
	/// <summary>
	/// ��ȣ�ۿ� �޽����� ����մϴ�.
	/// </summary>
	/// <param name="message">��ȣ�ۿ� �޽��� Ÿ��</param>
	void DisplayMessage(EMessageType message);
};
