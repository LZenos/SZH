// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretAnimInstance.h"

UTurretAnimInstance::UTurretAnimInstance()
{
	isAttackStart = false;
}

void UTurretAnimInstance::OnAttackStart()
{
	isAttackStart = true;
}