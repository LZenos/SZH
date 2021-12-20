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
	/// 상호작용 메시지를 출력합니다.
	/// </summary>
	/// <param name="message">상호작용 메시지 타입</param>
	void DisplayMessage(EMessageType message);
};
