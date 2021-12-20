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
	/// 자원이 가득 찼는지 여부. 자원이 가득차면 true, 상호작용하여 자원을 회수하면 false
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
	/// 자원을 제공합니다. 제공된 자원은 상호작용하여 회수하여 활용할 수 있습니다.
	/// </summary>
	void Supply();
};
