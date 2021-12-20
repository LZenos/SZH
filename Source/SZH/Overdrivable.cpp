// Fill out your copyright notice in the Description page of Project Settings.


#include "Overdrivable.h"


void IOverdrivable::InitOverdriveMembers(float overdrive_time, float feedback_time)
{
	electricState = EElectricState::E_OPERATE;

	available = true;

	overdriveTime = overdrive_time;
	feedbackTime = feedback_time;

	isOverdriving = false;
	isFeedback = false;
	currentOverdriveTime = 0.0f;
}

void IOverdrivable::TickOverdrive(float DeltaTime)
{
	if (isOverdriving)
	{
		if (currentOverdriveTime >= overdriveTime)
		{
			FeedBack();
			currentOverdriveTime = 0.0f;
		}
		else
		{
			currentOverdriveTime += DeltaTime;
		}
	}
	else if (isFeedback)
	{
		if (currentOverdriveTime >= feedbackTime)
		{
			isFeedback = false;
			OnFeedbackEnd();
			currentOverdriveTime = 0.0f;
		}
		else
		{
			currentOverdriveTime += DeltaTime;
		}
	}
}

void IOverdrivable::FeedBack()
{
	isOverdriving = false;
	isFeedback = true;
	electricState = EElectricState::E_PAUSE;
}

void IOverdrivable::ShutDown()
{
	available = false;
}
bool IOverdrivable::IsAvailable() const
{
	return available;
}


void IOverdrivable::InitGeneratorActorPointer(AGenerator* actor)
{
	generatorActor = actor;
}

EElectricState IOverdrivable::GetElectricState() const
{
	return electricState;
}
bool IOverdrivable::SetElectricState(EElectricState state)
{
	if (IsAvailable() && !isOverdriving && !isFeedback)
	{
		electricState = state;
		switch (electricState)
		{
		case EElectricState::E_PAUSE:
			break;

		case EElectricState::E_OPERATE:
			break;

		case EElectricState::E_OVERDRIVE:
			isOverdriving = true;
			Overdrive();
			break;
		}
		OnStateChange(electricState);

		return true;
	}
	else
	{
		if (available)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed setting electric mode. It's not available..."));
		}
		if (isOverdriving)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed setting electric mode. It's overdriving now..."));
		}
		if (isFeedback)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed setting electric mode. It's feedbacking now..."));
		}
		
		return false;
	}
}