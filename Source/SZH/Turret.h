// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Upgradable.h"
#include "Overdrivable.h"
#include "Turret.generated.h"

UCLASS()
class SZH_API ATurret : public AActor, public IInteractable, public IUpgradable, public IOverdrivable
{
	GENERATED_BODY()
	
public:	
	ATurret();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

public:
	// The amount of damage on one hit per upgrade level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TArray<float> attackPointLevel;
	// The speed of attack per upgrade level. Attack this times per one sec.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TArray<float> attackSpeedLevel;

	// The Skeletal Mesh of turret gun-head per upgrade level.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Model)
	TArray<USkeletalMesh*> turretMeshLevel;
	// The Animation Blueprint of turret. This should be matched with Skeletal Mesh.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Model)
	TArray<UAnimBlueprint*> turretAnimBlueprintLevel;
	// The Sound of fire per upgrade level.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	TArray<USoundBase*> turretFireSoundLevel;

private:
	/// <summary>
	/// 터렛의 현재 공격력
	/// </summary>
	float attackPoint;
	/// <summary>
	/// 터렛의 현재 공격속도
	/// </summary>
	float attackSpeed;

	float currentAttackTime;

	bool isAttackEffectOn;
	float attackEffectLifeTime;
	FVector attackEffectDest;

	/// <summary>
	/// 터렛의 포신 부분 스켈레톤 메쉬. 업그레이드마다 변경
	/// </summary>
	USkeletalMeshComponent* turretGun;
	/// <summary>
	/// 격발 이펙트
	/// </summary>
	UParticleSystemComponent* fireEffect;
	/// <summary>
	/// 격발 사운드
	/// </summary>
	UAudioComponent* fireSound;
	/// <summary>
	/// 개틀링 총열 스태틱 메쉬
	/// </summary>
	UStaticMeshComponent* gatlingBarrel;

	/// <summary>
	/// 표적 큐. 순서대로 공격 진행
	/// </summary>
	TQueue<class AZombieHorde*> targetQueue;


	virtual void Upgrade() override;
	
	virtual void Overdrive() override;

	virtual void FeedBack() override;
	virtual void OnFeedbackEnd() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Interact() override;

	/// <summary>
	/// 공격을 진행합니다. 격발 이펙트 표시, 사운드 출력, 피해 전달을 진행합니다.
	/// </summary>
	void Attack();
	/// <summary>
	/// 발사된 총알의 궤적을 LifeTime 프레임 수 동안 그립니다.
	/// </summary>
	void DrawAttackTrace(float LifeTime);
	/// <summary>
	/// 표적 큐가 비어있지 않으면 터렛의 방향을 표적 방향으로 고정합니다.
	/// </summary>
	void AimTarget();
	/// <summary>
	/// 타겟이 사망하면 호출됩니다. 타겟 큐를 Dequeue합니다.
	/// </summary>
	void TargetDeath();
};
