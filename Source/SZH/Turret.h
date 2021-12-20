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
	/// �ͷ��� ���� ���ݷ�
	/// </summary>
	float attackPoint;
	/// <summary>
	/// �ͷ��� ���� ���ݼӵ�
	/// </summary>
	float attackSpeed;

	float currentAttackTime;

	bool isAttackEffectOn;
	float attackEffectLifeTime;
	FVector attackEffectDest;

	/// <summary>
	/// �ͷ��� ���� �κ� ���̷��� �޽�. ���׷��̵帶�� ����
	/// </summary>
	USkeletalMeshComponent* turretGun;
	/// <summary>
	/// �ݹ� ����Ʈ
	/// </summary>
	UParticleSystemComponent* fireEffect;
	/// <summary>
	/// �ݹ� ����
	/// </summary>
	UAudioComponent* fireSound;
	/// <summary>
	/// ��Ʋ�� �ѿ� ����ƽ �޽�
	/// </summary>
	UStaticMeshComponent* gatlingBarrel;

	/// <summary>
	/// ǥ�� ť. ������� ���� ����
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
	/// ������ �����մϴ�. �ݹ� ����Ʈ ǥ��, ���� ���, ���� ������ �����մϴ�.
	/// </summary>
	void Attack();
	/// <summary>
	/// �߻�� �Ѿ��� ������ LifeTime ������ �� ���� �׸��ϴ�.
	/// </summary>
	void DrawAttackTrace(float LifeTime);
	/// <summary>
	/// ǥ�� ť�� ������� ������ �ͷ��� ������ ǥ�� �������� �����մϴ�.
	/// </summary>
	void AimTarget();
	/// <summary>
	/// Ÿ���� ����ϸ� ȣ��˴ϴ�. Ÿ�� ť�� Dequeue�մϴ�.
	/// </summary>
	void TargetDeath();
};
