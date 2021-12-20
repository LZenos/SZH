// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Generator.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "DoorHealthBar.h"


ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(TEXT("Door"));

	canInteract = false;

	upgradeLevel = 0;

	InitOverdriveMembers(7.0f, 10.0f);


	doorHealthLevel.Add(500.0f);
	doorHealthLevel.Add(650.0f);
	doorHealthLevel.Add(900.0f);


	static ConstructorHelpers::FObjectFinder<USoundBase>
		DOOR_UP(TEXT("/Game/Sound/Upgrade/Upgrade.Upgrade"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		DOOR_DIE(TEXT("/Game/StarterContent/Audio/Cue/Explosion_Cue.Explosion_Cue"));
	if (DOOR_UP.Succeeded() && DOOR_DIE.Succeeded())
	{
		sound_upgrade = DOOR_UP.Object;
		sound_explode = DOOR_DIE.Object;
	}
}

void ADoor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	doorHealth = doorHealthLevel[upgradeLevel];

	doorLeafMesh = Cast<UStaticMeshComponent>(GetComponentsByTag(UStaticMeshComponent::StaticClass(), TEXT("DoorLeaf"))[0]);
	overdriveEffect = Cast<UParticleSystemComponent>(GetComponentsByTag(UParticleSystemComponent::StaticClass(), TEXT("Particle_Overdrive"))[0]);
	upgradeSound = Cast<UAudioComponent>(GetComponentByClass(UAudioComponent::StaticClass()));

	upgradeSound->SetSound(sound_upgrade);
	upgradeSound->Stop();
}
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	UpdateHealthBar();

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

void ADoor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		canInteract = true;
	}
}
void ADoor::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		canInteract = false;
	}
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickOverdrive(DeltaTime);
}

float ADoor::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (doorHealth > 0.0f)
	{
		if (isOverdriving || isFeedback)
		{
			damage = 0.0f;
		}

		doorHealth -= damage;
		if (doorHealth <= 0.0f)
		{
			doorHealth = 0.0f;
			Death();
		}
		UpdateHealthBar();
	}
	else
	{
		damage = 0.0f;
	}

	return damage;
}

void ADoor::Interact()
{
	if (canInteract)
	{
		Upgrade();
	}
}

void ADoor::Upgrade()
{
	if (IsAvailable())
	{
		if (characterActor->is_Carrying)
		{
			if (upgradeLevel < doorHealthLevel.Num() - 1)
			{
				upgradeSound->Play();
				upgradeLevel++;
				doorHealth = doorHealthLevel[upgradeLevel];
				if (isOverdriving)
				{
					Overdrive();
				}
				UpdateHealthBar();

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
	else
	{
		// 업그레이드 불가능 경고 UI 출력
		messageManager->DisplayMessage(EMessageType::E_DESTROYED);
	}
}

void ADoor::Overdrive()
{
	overdriveEffect->ActivateSystem();
}

void ADoor::FeedBack()
{
	IOverdrivable::FeedBack();
	generatorActor->OverdriveEnd(GetName());

	overdriveEffect->DeactivateSystem();
	GuardDeactivate();
}
void ADoor::OnFeedbackEnd()
{
	generatorActor->AllOverdriveProcessEnd(GetName());
}

void ADoor::OnStateChange(EElectricState NewState)
{
	switch (NewState)
	{
	case EElectricState::E_PAUSE:
		GuardDeactivate();
		break;

	case EElectricState::E_OPERATE:
	case EElectricState::E_OVERDRIVE:
		GuardActivate();
		break;
	}
}


void ADoor::GuardActivate()
{
	if (!ActorHasTag(TEXT("Door")))
	{
		Tags.Add(TEXT("Door"));
		doorLeafMesh->SetRelativeRotation(doorLeafMesh->GetRelativeRotation() + FRotator(0.0f, 90.0f, 0.0f));
	}
}
void ADoor::GuardDeactivate()
{
	if (ActorHasTag(TEXT("Door")))
	{
		Tags.Remove(TEXT("Door"));
		doorLeafMesh->SetRelativeRotation(doorLeafMesh->GetRelativeRotation() + FRotator(0.0f, -90.0f, 0.0f));
	}
}

void ADoor::UpdateHealthBar()
{
	UDoorHealthBar* healthWidget = Cast<UDoorHealthBar>(Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()))->GetUserWidgetObject());
	if (healthWidget != nullptr)
	{
		healthWidget->healthProgress = doorHealth / doorHealthLevel[upgradeLevel];
	}
}

void ADoor::Death()
{
	SetElectricState(EElectricState::E_PAUSE);
	generatorActor->OverdriveEnd(GetName());
	ShutDown();

	UParticleSystemComponent* deathEffect =
		Cast<UParticleSystemComponent>(GetComponentsByTag(UParticleSystemComponent::StaticClass(), TEXT("Particle_Death"))[0]);
	if (deathEffect != nullptr)
	{
		deathEffect->Activate();
	}

	upgradeSound->SetSound(sound_explode);
	upgradeSound->Play();

	doorLeafMesh->DestroyComponent();
}