// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Generator.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/LineBatchComponent.h"
#include "Animation/AnimBlueprint.h"
#include "TurretAnimInstance.h"
#include "ZombieHorde.h"


ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

	canInteract = false;

	upgradeLevel = 0;

	InitOverdriveMembers(5.0f, 10.0f);


	attackPointLevel.Add(5.0f);
	attackPointLevel.Add(5.0f);
	attackPointLevel.Add(10.0f);

	attackSpeedLevel.Add(0.5f);
	attackSpeedLevel.Add(1.0f);
	attackSpeedLevel.Add(1.0f);


	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TURRET_SK1(TEXT("/Game/Model/Turret/Meshes/Turret1/Skeleton/Turret_Cannon.Turret_Cannon"));
	/*static ConstructorHelpers::FClassFinder<UAnimBlueprint>
		TURRET_AB1(TEXT("/Game/Model/Turret/Meshes/Turret1/Skeleton/Turret_Cannon_AnimBP.Turret_Cannon_AnimBP_C"));*/
	static ConstructorHelpers::FObjectFinder<USoundBase>
		TURRET_SO1(TEXT("/Game/Sound/Turret/Cues/GunShot_Level1_Cue.GunShot_Level1_Cue"));
	if (TURRET_SK1.Succeeded() && /*TURRET_AB1.Succeeded() &&*/ TURRET_SO1.Succeeded())
	{
		turretMeshLevel.Add(TURRET_SK1.Object);
		//turretAnimBlueprintLevel.Add(TURRET_AB1.Class);
		turretFireSoundLevel.Add(TURRET_SO1.Object);
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TURRET_SK2(TEXT("/Game/Model/Turret/Meshes/Turret2/Skeleton/Turret_Shotgun.Turret_Shotgun"));
	/*static ConstructorHelpers::FClassFinder<UAnimBlueprint>
		TURRET_AB2(TEXT("/Game/Model/Turret/Meshes/Turret2/Skeleton/Turret_Shotgun_AnimBP.Turret_Shotgun_AnimBP_C"));*/
	static ConstructorHelpers::FObjectFinder<USoundBase>
		TURRET_SO2(TEXT("/Game/Sound/Turret/Cues/GunShot_Level2_Cue.GunShot_Level2_Cue"));
	if (TURRET_SK2.Succeeded() && /*TURRET_AB2.Succeeded() &&*/ TURRET_SO2.Succeeded())
	{
		turretMeshLevel.Add(TURRET_SK2.Object);
		//turretAnimBlueprintLevel.Add(TURRET_AB2.Class);
		turretFireSoundLevel.Add(TURRET_SO2.Object);
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TURRET_SK3(TEXT("/Game/Model/Turret/Meshes/Turret3/Skeleton/Turret_Gatling.Turret_Gatling"));
	/*static ConstructorHelpers::FClassFinder<UAnimBlueprint>
		TURRET_AB3(TEXT("/Game/Model/Turret/Meshes/Turret3/Skeleton/Turret_Gatling_AnimBP.Turret_Gatling_AnimBP"));*/
	static ConstructorHelpers::FObjectFinder<USoundBase>
		TURRET_SO3(TEXT("/Game/Sound/Turret/Cues/GunShot_Level3_Cue.GunShot_Level3_Cue"));
	if (TURRET_SK3.Succeeded() && /*TURRET_AB3.Succeeded() &&*/ TURRET_SO3.Succeeded())
	{
		turretMeshLevel.Add(TURRET_SK3.Object);
		//turretAnimBlueprintLevel.Add(TURRET_AB3.Class);
		turretFireSoundLevel.Add(TURRET_SO3.Object);
	}


	currentAttackTime = 0.0;

	isAttackEffectOn = false;
	attackEffectLifeTime = 0.0f;
	attackEffectDest = FVector::ZeroVector;
}

void ATurret::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	attackPoint = attackPointLevel[upgradeLevel];
	attackSpeed = attackSpeedLevel[upgradeLevel];

	turretGun = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	fireEffect = Cast<UParticleSystemComponent>(GetComponentByClass(UParticleSystemComponent::StaticClass()));
	fireSound = Cast<UAudioComponent>(GetComponentsByTag(UAudioComponent::StaticClass(), FName("GunFireSound"))[0]);
	upgradeSound = Cast<UAudioComponent>(GetComponentsByTag(UAudioComponent::StaticClass(), FName("UpgradeSound"))[0]);
	gatlingBarrel = Cast<UStaticMeshComponent>(GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Turret_GatlingBarrel"))[0]);

	turretGun->SetSkeletalMesh(turretMeshLevel[upgradeLevel]);
	//turretGun->SetAnimInstanceClass(turretAnimBlueprintLevel[upgradeLevel]->GeneratedClass);
	fireSound->SetSound(turretFireSoundLevel[upgradeLevel]);
	fireSound->Stop();
}
void ATurret::BeginPlay()
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

	turretGun->SetAnimInstanceClass(turretAnimBlueprintLevel[upgradeLevel]->GeneratedClass);
}

void ATurret::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		canInteract = true;
	}
	else if (OtherActor->ActorHasTag(FName("Zombie")))
	{
		AZombieHorde* attackTarget = Cast<AZombieHorde>(OtherActor);
		if (attackTarget != nullptr)
		{
			targetQueue.Enqueue(attackTarget);
		}
	}
}
void ATurret::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		canInteract = false;
	}
	else if (OtherActor->ActorHasTag(FName("Zombie")))
	{
		TargetDeath();
	}
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (GetElectricState())
	{
	case EElectricState::E_OPERATE:
	case EElectricState::E_OVERDRIVE:
		AimTarget();

		// 공격 및 쿨타임 진행
		if (currentAttackTime >= (1.0f / attackSpeed))
		{
			if (!targetQueue.IsEmpty())
			{
				Attack();
				currentAttackTime = 0.0f;
			}
		}
		else
		{
			currentAttackTime += DeltaTime;
		}

		DrawAttackTrace(5.0f);
		break;
	}

	TickOverdrive(DeltaTime);
}

void ATurret::Interact()
{
	if (canInteract)
	{
		Upgrade();
	}
}

void ATurret::Upgrade()
{
	if (characterActor->is_Carrying)
	{
		if (upgradeLevel < attackPointLevel.Num() - 1)
		{
			upgradeSound->Play();
			upgradeLevel++;
			attackPoint = attackPointLevel[upgradeLevel];
			attackSpeed = attackSpeedLevel[upgradeLevel];
			if (isOverdriving)
			{
				Overdrive();
			}

			// 에셋 변경
			turretGun->SetSkeletalMesh(turretMeshLevel[upgradeLevel]);
			turretGun->SetAnimInstanceClass(turretAnimBlueprintLevel[upgradeLevel]->GeneratedClass);
			fireSound->SetSound(turretFireSoundLevel[upgradeLevel]);
			fireSound->Stop();

			// 개틀링건 메쉬로 업그레이드한 경우, 총열 스태틱 메쉬 표시
			if (upgradeLevel == 2)
			{
				gatlingBarrel->SetVisibility(true);
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

void ATurret::Overdrive()
{
	attackSpeed *= 4.0f;
}

void ATurret::FeedBack()
{
	IOverdrivable::FeedBack();
	generatorActor->OverdriveEnd(GetName());

	attackSpeed = attackSpeedLevel[upgradeLevel];
}
void ATurret::OnFeedbackEnd()
{
	generatorActor->AllOverdriveProcessEnd(GetName());
}


void ATurret::Attack()
{
	fireSound->Play();
	fireEffect->Activate(true);

	isAttackEffectOn = true;
	attackEffectDest = (*targetQueue.Peek())->GetActorLocation();

	Cast<UTurretAnimInstance>(turretGun->GetAnimInstance())->OnAttackStart();

	FRotator dir = gatlingBarrel->GetRelativeRotation() + FRotator(0.0f, 0.0f, 30.0f);
	gatlingBarrel->SetRelativeRotation(dir);

	FDamageEvent damage_event;
	(*targetQueue.Peek())->TakeDamage(attackPoint, damage_event, GetWorld()->GetFirstPlayerController(), this);
}

void ATurret::DrawAttackTrace(float LifeTime)
{
	if (isAttackEffectOn)
	{
		if (attackEffectLifeTime < LifeTime)
		{
			attackEffectLifeTime++;

			GetWorld()->LineBatcher->DrawLine(
				fireEffect->GetComponentLocation(),
				attackEffectDest,
				FColor(200, 200, 0),
				0,
				5.0f,
				1.0f
			);
		}
		else
		{
			isAttackEffectOn = false;
			attackEffectLifeTime = 0.0f;
			attackEffectDest = FVector::ZeroVector;
		}
	}
}

void ATurret::AimTarget()
{
	if (!targetQueue.IsEmpty())
	{
		FVector dir_vec = (*targetQueue.Peek())->GetActorLocation() - GetActorLocation();
		FRotator dir = dir_vec.Rotation() - GetActorRotation();
		dir.Pitch = 0.0f;
		turretGun->SetRelativeRotation(dir);
	}
}

void ATurret::TargetDeath()
{
	targetQueue.Pop();
}