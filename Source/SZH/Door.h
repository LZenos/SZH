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
	/// 문 오브젝트의 현재 체력
	/// </summary>
	float doorHealth;

	/// <summary>
	/// 문짝 스태틱 메쉬
	/// </summary>
	UStaticMeshComponent* doorLeafMesh;
	/// <summary>
	/// 과부하 이펙트 컴포넌트
	/// </summary>
	UParticleSystemComponent* overdriveEffect;


	virtual void Upgrade() override;

	virtual void Overdrive() override;

	virtual void FeedBack() override;
	virtual void OnFeedbackEnd() override;

	virtual void OnStateChange(EElectricState NewState) override;

	/// <summary>
	/// 문의 방어 능력을 활성화시킵니다.
	/// </summary>
	void GuardActivate();
	/// <summary>
	/// 문의 방어 능력을 비활성화시킵니다. 
	/// </summary>
	void GuardDeactivate();

	/// <summary>
	/// 체력바 위젯에 접근하여 자신의 체력 정보를 갱신합니다.
	/// </summary>
	void UpdateHealthBar();

	void Death();

public:
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void Interact() override;
};
