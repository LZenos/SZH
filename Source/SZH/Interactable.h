// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UObject/Interface.h"
#include "PlayerCharacter.h"
#include "MessageManager.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SZH_API IInteractable
{
	GENERATED_BODY()

protected:
	/// <summary>
	/// 상호작용 가능 여부
	/// </summary>
	bool canInteract;

	/// <summary>
	/// 플레이어 캐릭터 액터
	/// </summary>
	APlayerCharacter* characterActor;

	/// <summary>
	/// 메시지의 생성, 출력 등을 관리하는 액터
	/// </summary>
	AMessageManager* messageManager;

public:
	/// <summary>
	/// 캐릭터의 상호작용 델리게이트에 바인딩되어, 상호작용 키 입력 시에 호출됩니다. UFUNCTION 매크로가 할당되어야 합니다.
	/// </summary>
	virtual void Interact() = 0;
};
