// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Overdrivable.h"
#include "DimensionGate.generated.h"

UCLASS()
class SZH_API ADimensionGate : public AActor, public IInteractable, public IOverdrivable
{
	GENERATED_BODY()
	
public:
	ADimensionGate();

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

public:
	// The amount of resources that is provided in one time.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Supply)
	int resourceSupply;
	// The interval to supply resources. Parameter is a sec.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Supply)
	float supplyTime;

private:
	/// <summary>
	/// �ڿ��� ���� á���� ����. �ڿ��� �������� true, ��ȣ�ۿ��Ͽ� �ڿ��� ȸ���ϸ� false
	/// </summary>
	bool canSupply;

	float currentSupplyTime;


	virtual void Overdrive() override;

	virtual void FeedBack() override;
	virtual void OnFeedbackEnd() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Interact() override;

	/// <summary>
	/// �ڿ��� �����մϴ�. ������ �ڿ��� ��ȣ�ۿ��Ͽ� ȸ���Ͽ� Ȱ���� �� �ֽ��ϴ�.
	/// </summary>
	void Supply();
};
