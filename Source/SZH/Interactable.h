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
	/// ��ȣ�ۿ� ���� ����
	/// </summary>
	bool canInteract;

	/// <summary>
	/// �÷��̾� ĳ���� ����
	/// </summary>
	APlayerCharacter* characterActor;

	/// <summary>
	/// �޽����� ����, ��� ���� �����ϴ� ����
	/// </summary>
	AMessageManager* messageManager;

public:
	/// <summary>
	/// ĳ������ ��ȣ�ۿ� ��������Ʈ�� ���ε��Ǿ�, ��ȣ�ۿ� Ű �Է� �ÿ� ȣ��˴ϴ�. UFUNCTION ��ũ�ΰ� �Ҵ�Ǿ�� �մϴ�.
	/// </summary>
	virtual void Interact() = 0;
};
