// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "WorkBench.generated.h"

UCLASS()
class SZH_API AWorkBench : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	AWorkBench();

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

public:
	// The resource cost of an upgrade kit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
	int kitCost;


	UFUNCTION()
	virtual void Interact() override;
};
