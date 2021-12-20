// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UObject/Interface.h"
#include "Upgradable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUpgradable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SZH_API IUpgradable
{
	GENERATED_BODY()

protected:
	/// <summary>
	/// ���׷��̵� �ܰ�. 0 ~ 2 �ܰ谡 �ֽ��ϴ�.
	/// </summary>
	int upgradeLevel;

	/// <summary>
	/// ���׷��̵� ȿ������ ����� ����� ������Ʈ
	/// </summary>
	UAudioComponent* upgradeSound;


	/// <summary>
	/// ������Ʈ�� ��ȭ�մϴ�.
	/// </summary>
	virtual void Upgrade() = 0;
};
