// Fill out your copyright notice in the Description page of Project Settings.


#include "DimensionGate.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Generator.h"
#include "Particles/ParticleSystemComponent.h"
#include "SZHGameModeBase.h"


ADimensionGate::ADimensionGate()
{
	PrimaryActorTick.bCanEverTick = true;

	canInteract = false;

	InitOverdriveMembers(20.0f, 7.0f);


	resourceSupply = 65;
	supplyTime = 20.0f;

	currentSupplyTime = 0.0f;

	canSupply = false;
}

void ADimensionGate::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<APlayerCharacter> iter(GetWorld()); iter; ++iter)
	{
		iter->InteractionWithStageObject.AddUFunction(this, FName("Interact"));
		characterActor = *iter;
	}
	for (TActorIterator<AMessageManager> iter(GetWorld()); iter; ++iter)
	{
		messageManager = *iter;
		break;
	}
}

void ADimensionGate::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		canInteract = true;
	}
}
void ADimensionGate::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		canInteract = false;
	}
}

void ADimensionGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (GetElectricState())
	{
	case EElectricState::E_OPERATE:
	case EElectricState::E_OVERDRIVE:
		if (!canSupply)
		{
			if (currentSupplyTime >= supplyTime)
			{
				Supply();
				currentSupplyTime = 0.0f;
			}
			else
			{
				currentSupplyTime += DeltaTime;
			}
		}
		break;
	}

	TickOverdrive(DeltaTime);
}

void ADimensionGate::Interact()
{
	if (canInteract)
	{
		if (canSupply)
		{
			ASZHGameModeBase* gmBase = Cast<ASZHGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			if (gmBase != nullptr)
			{
				bool res = gmBase->IncreaseResource(resourceSupply);
				if (res)
				{
					canSupply = false;
					Cast<UParticleSystemComponent>(GetComponentByClass(UParticleSystemComponent::StaticClass()))->DeactivateSystem();
				}
			}
		}
		else
		{
			// 자원 미준비 UI 출력
			messageManager->DisplayMessage(EMessageType::E_RESOURCE_NOT_READY);
		}
	}
}

void ADimensionGate::Overdrive()
{
	supplyTime /= 2.0f;
}

void ADimensionGate::FeedBack()
{
	IOverdrivable::FeedBack();
	generatorActor->OverdriveEnd(GetName());

	supplyTime *= 2.0f;
}
void ADimensionGate::OnFeedbackEnd()
{
	generatorActor->AllOverdriveProcessEnd(GetName());
}


void ADimensionGate::Supply()
{
	canSupply = true;
	Cast<UParticleSystemComponent>(GetComponentByClass(UParticleSystemComponent::StaticClass()))->ActivateSystem();
}