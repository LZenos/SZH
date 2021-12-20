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
	/// 업그레이드 단계. 0 ~ 2 단계가 있습니다.
	/// </summary>
	int upgradeLevel;

	/// <summary>
	/// 업그레이드 효과음을 출력할 오디오 컴포넌트
	/// </summary>
	UAudioComponent* upgradeSound;


	/// <summary>
	/// 오브젝트를 강화합니다.
	/// </summary>
	virtual void Upgrade() = 0;
};
