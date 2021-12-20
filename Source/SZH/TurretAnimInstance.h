// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Animation/AnimInstance.h"
#include "TurretAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SZH_API UTurretAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTurretAnimInstance();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool isAttackStart;

public:
	void OnAttackStart();
};
