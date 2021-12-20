// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Upgradable.h"
#include "Overdrivable.h"
#include "Generator.generated.h"

UCLASS()
class SZH_API AGenerator : public AActor, public IInteractable, public IUpgradable
{
	GENERATED_BODY()

public:
	AGenerator();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

public:
	// The amont of electronic power to supply all StageObjects per upgrade level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ElectricSupply)
	TArray<int> electricPowerLevel;

private:
	/// <summary>
	/// 최대 전원 공급량
	/// </summary>
	int electricPower;

	/// <summary>
	/// 전기 차단기 UI인 위젯 블루프린트
	/// </summary>
	UPROPERTY()
	class UGeneratorWidget* generatorWidget;

	/// <summary>
	/// 차단기 열기/닫기 사운드 출력 컴포넌트
	/// </summary>
	UAudioComponent* openSound;

	/// <summary>
	/// 전기를 소모하는 모든 스테이지 오브젝트 컨테이너
	/// </summary>
	TArray<class IOverdrivable*> electronics;


	virtual void Upgrade() override;

	/// <summary>
	/// 호출한 시점의 총 전력 사용량을 반환합니다.
	/// </summary>
	/// <returns>총 전력 사용량</returns>
	int GetTotalElectricUsage() const;

public:
	UFUNCTION()
	virtual void Interact() override;

	/// <summary>
	/// 주어진 index에 해당하는 스테이지 오브젝트의 현재 상태를 반환합니다. 인덱스 순서는 차단기의 UI 순서와 동일합니다.
	/// </summary>
	/// <returns>오브젝트의 현재 상태. 작동 중지 상태이거나, 입력이 잘못된 경우에도 0을 반환</returns>
	EElectricState GetCurrentElectricState(int index) const;

	/// <summary>
	/// index에 해당하는 전력 오브젝트의 전력 상태를 정지-작동 사이에서 상호 전환합니다.
	/// </summary>
	/// <param name="index">전력 오브젝트 인덱스. UI 순서와 동일</param>
	/// <returns>전환에 성공하면 true를, 실패하면 false를 반환</returns>
	bool SwitchElectricState(int index);
	/// <summary>
	/// 해당 index의 전력 오브젝트를 과부하시킵니다.
	/// </summary>
	/// <param name="index">전력 오브젝트 인덱스. UI 순서와 동일</param>
	/// <returns>과부하에 성공하면 true를, 실패하면 false를 반환</returns>
	bool OverdriveStart(int index);

	/// <summary>
	/// 과부하가 종료되었을 때 호출되어야 합니다. 위젯에 차단기가 내려간 상태를 갱신하기 위해 사용됩니다.
	/// </summary>
	/// <param name="ObjectName">전력 오브젝트 이름</param>
	void OverdriveEnd(FString ObjectName);
	/// <summary>
	/// 과부하, 피드백까지 모두 종료된 후에 호출되어야 합니다. 위젯에 갱신된 정보를 전달하는 용도로 사용됩니다.
	/// </summary>
	/// <param name="ObjectName">전력 오브젝트 이름</param>
	void AllOverdriveProcessEnd(FString ObjectName);
};
