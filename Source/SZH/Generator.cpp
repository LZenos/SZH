// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "GeneratorWidget.h"


AGenerator::AGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	canInteract = false;

	upgradeLevel = 0;


	int epIter = 5;
	electricPowerLevel.Add(epIter++);
	electricPowerLevel.Add(epIter++);
	electricPowerLevel.Add(epIter++);
}

void AGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	electricPower = electricPowerLevel[upgradeLevel];

	openSound = Cast<UAudioComponent>(GetComponentsByTag(UAudioComponent::StaticClass(), FName("OpenSound"))[0]);
	upgradeSound = Cast<UAudioComponent>(GetComponentsByTag(UAudioComponent::StaticClass(), FName("UpgradeSound"))[0]);
}
void AGenerator::BeginPlay()
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

	// 월드에 배치된 모든 전력 소모 오브젝트를 컨테이너에 담습니다.
	for (TActorIterator<AActor> iter(GetWorld()); iter; ++iter)
	{
		IOverdrivable* stageObject = Cast<IOverdrivable>(*iter);
		if (stageObject != nullptr)
		{
			electronics.Add(stageObject);
			stageObject->InitGeneratorActorPointer(this);
		}
	}
	// 오브젝트를 UI 정보에 맞춰 정렬합니다. 끔찍한 하드코딩이지만 따로 규칙이 있는 것이 아니라서 어쩔 수 없습니다.
	for (int i = 0; i < electronics.Num(); i++)
	{
		for (int j = 0; j < electronics.Num(); j++)
		{
			if (i == j)
			{
				continue;
			}

			switch (i)
			{
			case 0:
				if (Cast<AActor>(electronics[j])->GetName() == FString("Turret_Left"))
				{
					electronics.Swap(i, j);
				}
				break;
			case 1:
				if (Cast<AActor>(electronics[j])->GetName() == FString("Door_Left"))
				{
					electronics.Swap(i, j);
				}
				break;
			case 2:
				if (Cast<AActor>(electronics[j])->GetName() == FString("DimensionGate"))
				{
					electronics.Swap(i, j);
				}
				break;
			case 3:
				if (Cast<AActor>(electronics[j])->GetName() == FString("Turret_Right"))
				{
					electronics.Swap(i, j);
				}
				break;
			case 4:
				if (Cast<AActor>(electronics[j])->GetName() == FString("Door_Right"))
				{
					electronics.Swap(i, j);
				}
				break;
			}
		}
	}

	// 위젯 블루프린트를 찾아 위젯을 생성하고 등록합니다.
	FStringClassReference tempGeneratorWidgetClassRef(TEXT("/Game/Created/Blueprints/UI/GeneratorUI_BP.GeneratorUI_BP_C"));
	if (UClass* tempGeneratorWidgetClass = tempGeneratorWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* tempGeneratorWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), tempGeneratorWidgetClass);
		generatorWidget = Cast<UGeneratorWidget>(tempGeneratorWidget);
		generatorWidget->InitOwner(this);
	}
}

void AGenerator::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		canInteract = true;
	}
}
void AGenerator::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		canInteract = false;

		if (generatorWidget->IsInViewport())
		{
			generatorWidget->RemoveFromViewport();
		}
	}
}

int AGenerator::GetTotalElectricUsage() const
{
	int total_usage = 0;
	for (int i = 0; i < electronics.Num(); i++)
	{
		total_usage += (int)electronics[i]->GetElectricState();
	}

	return total_usage;
}

void AGenerator::Interact()
{
	if (canInteract)
	{
		openSound->Play(0.5f);

		if (generatorWidget != nullptr)
		{
			if (!generatorWidget->IsInViewport())
			{
				generatorWidget->AddToViewport();
			}
			else
			{
				generatorWidget->RemoveFromViewport();
			}
		}

		Upgrade();
	}
}

void AGenerator::Upgrade()
{
	if (characterActor != nullptr)
	{
		if (characterActor->is_Carrying)
		{
			if (upgradeLevel < electricPowerLevel.Num() - 1)
			{
				upgradeSound->Play(0.5f);
				upgradeLevel++;
				electricPower = electricPowerLevel[upgradeLevel];

				if (generatorWidget != nullptr)
				{
					generatorWidget->UpdateGeneratorLevel(upgradeLevel + 1);
					generatorWidget->UpdateElectricSurplus(electricPower - GetTotalElectricUsage());
				}

				characterActor->is_Carrying = false;
			}
			else
			{
				// 업그레이드 최대 UI 출력
				messageManager->DisplayMessage(EMessageType::E_UPGRADE_MAX);
			}
		}
		else
		{
			// 키트 미 소지 경고 UI 출력
			messageManager->DisplayMessage(EMessageType::E_REQUIRE_KIT);
		}
	}
}

EElectricState AGenerator::GetCurrentElectricState(int index) const
{
	if (index < electronics.Num())
	{
		return electronics[index]->GetElectricState();
	}
	else
	{
		return EElectricState::E_PAUSE;
	}
}

bool AGenerator::SwitchElectricState(int index)
{
	bool res = false;

	if (index < electronics.Num())
	{
		switch (GetCurrentElectricState(index))
		{
		case EElectricState::E_PAUSE:
			if (electricPower - GetTotalElectricUsage())
			{
				res = electronics[index]->SetElectricState(EElectricState::E_OPERATE);
			}
			break;

		case EElectricState::E_OPERATE:
			res = electronics[index]->SetElectricState(EElectricState::E_PAUSE);
			break;
		}
	}

	if (res)
	{
		generatorWidget->UpdateElectricSurplus(electricPower - GetTotalElectricUsage());
	}

	return res;
}
bool AGenerator::OverdriveStart(int index)
{
	bool res = false;

	if (index < electronics.Num() &&
		(electricPower - GetTotalElectricUsage()) >= (2 - (int)GetCurrentElectricState(index)))
	{
		res = electronics[index]->SetElectricState(EElectricState::E_OVERDRIVE);
	}

	if (res)
	{
		generatorWidget->UpdateElectricSurplus(electricPower - GetTotalElectricUsage());
	}

	return res;
}

void AGenerator::OverdriveEnd(FString ObjectName)
{
	int index = -1;
	// ObjectName에 따라 인덱스 설정
	{
		if (ObjectName == FString("Turret_Left"))
		{
			index = 0;
		}
		else if (ObjectName == FString("Door_Left"))
		{
			index = 1;
		}
		else if (ObjectName == FString("DimensionGate"))
		{
			index = 2;
		}
		else if (ObjectName == FString("Turret_Right"))
		{
			index = 3;
		}
		else if (ObjectName == FString("Door_Right"))
		{
			index = 4;
		}
	}
	if (index < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Objects are not named properly. Please check objects name and re-name them properly."));
		UE_LOG(LogTemp, Warning, TEXT("%s: Turret_Left"), *Cast<AActor>(electronics[0])->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s: Door_Left"), *Cast<AActor>(electronics[1])->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s: DimensionGate"), *Cast<AActor>(electronics[2])->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s: Turret_Right"), *Cast<AActor>(electronics[3])->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s: Door_Right"), *Cast<AActor>(electronics[4])->GetName());
		return;
	}

	generatorWidget->UpdateElectricSurplus(electricPower - GetTotalElectricUsage());
	generatorWidget->ForceOffLeakBreaker(index);
}
void AGenerator::AllOverdriveProcessEnd(FString ObjectName)
{
	int index = -1;
	// ObjectName에 따라 인덱스 설정
	{
		if (ObjectName == FString("Turret_Left"))
		{
			index = 0;
		}
		else if (ObjectName == FString("Door_Left"))
		{
			index = 1;
		}
		else if (ObjectName == FString("DimensionGate"))
		{
			index = 2;
		}
		else if (ObjectName == FString("Turret_Right"))
		{
			index = 3;
		}
		else if (ObjectName == FString("Door_Right"))
		{
			index = 4;
		}
	}
	if (index < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Objects are not named properly. Please check objects name and re-name them properly."));
		UE_LOG(LogTemp, Warning, TEXT("%s: Turret_Left"), *Cast<AActor>(electronics[0])->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s: Door_Left"), *Cast<AActor>(electronics[1])->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s: DimensionGate"), *Cast<AActor>(electronics[2])->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s: Turret_Right"), *Cast<AActor>(electronics[3])->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s: Door_Right"), *Cast<AActor>(electronics[4])->GetName());
		return;
	}

	generatorWidget->ActivateOverdriveButton(index);
}