// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Upgradable.h"
#include "Overdrivable.h"
#include "Door.generated.h"

UCLASS()
class SZH_API ADoor : public AActor, public IInteractable, public IUpgradable, public IOverdrivable
{
	GENERATED_BODY()

public:
	ADoor();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

public:
	// The max health of door per upgradeLevel.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	TArray<float> doorHealthLevel;

	// The sound to play when upgrade object.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	USoundBase* sound_upgrade;
	// The sound of play when door is destroyed.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	USoundBase* sound_explode;

private:
	/// <summary>
	/// �� ������Ʈ�� ���� ü��
	/// </summary>
	float doorHealth;

	/// <summary>
	/// ��¦ ����ƽ �޽�
	/// </summary>
	UStaticMeshComponent* doorLeafMesh;
	/// <summary>
	/// ������ ����Ʈ ������Ʈ
	/// </summary>
	UParticleSystemComponent* overdriveEffect;


	virtual void Upgrade() override;

	virtual void Overdrive() override;

	virtual void FeedBack() override;
	virtual void OnFeedbackEnd() override;

	virtual void OnStateChange(EElectricState NewState) override;

	/// <summary>
	/// ���� ��� �ɷ��� Ȱ��ȭ��ŵ�ϴ�.
	/// </summary>
	void GuardActivate();
	/// <summary>
	/// ���� ��� �ɷ��� ��Ȱ��ȭ��ŵ�ϴ�. 
	/// </summary>
	void GuardDeactivate();

	/// <summary>
	/// ü�¹� ������ �����Ͽ� �ڽ��� ü�� ������ �����մϴ�.
	/// </summary>
	void UpdateHealthBar();

	void Death();

public:
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void Interact() override;
};
