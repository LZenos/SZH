// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkBench.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "SZHGameModeBase.h"


AWorkBench::AWorkBench()
{
	PrimaryActorTick.bCanEverTick = false;

	kitCost = 100;
}

void AWorkBench::BeginPlay()
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

void AWorkBench::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		canInteract = true;
	}
}
void AWorkBench::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		canInteract = false;
	}
}

void AWorkBench::Interact()
{
	if (canInteract)
	{
		ASZHGameModeBase* gmBase = Cast<ASZHGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gmBase != nullptr)
		{
			bool res = gmBase->IncreaseResource(-kitCost);
			if (res && !characterActor->is_Carrying)
			{
				// ���׷��̵� ŰƮ ����
				characterActor->is_Carrying = true;
			}
			else
			{
				if (characterActor->is_Carrying)
				{
					// �̹� ŰƮ ��� �ִٴ� UI ���
					messageManager->DisplayMessage(EMessageType::E_CARRYING_KIT);
				}
				else if (!res)
				{
					// �ڿ� ���� UI ���
					messageManager->DisplayMessage(EMessageType::E_REQUIRE_RESOURCE);
				}
			}
		}
	}
}