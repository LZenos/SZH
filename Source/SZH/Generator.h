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
	/// �ִ� ���� ���޷�
	/// </summary>
	int electricPower;

	/// <summary>
	/// ���� ���ܱ� UI�� ���� �������Ʈ
	/// </summary>
	UPROPERTY()
	class UGeneratorWidget* generatorWidget;

	/// <summary>
	/// ���ܱ� ����/�ݱ� ���� ��� ������Ʈ
	/// </summary>
	UAudioComponent* openSound;

	/// <summary>
	/// ���⸦ �Ҹ��ϴ� ��� �������� ������Ʈ �����̳�
	/// </summary>
	TArray<class IOverdrivable*> electronics;


	virtual void Upgrade() override;

	/// <summary>
	/// ȣ���� ������ �� ���� ��뷮�� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>�� ���� ��뷮</returns>
	int GetTotalElectricUsage() const;

public:
	UFUNCTION()
	virtual void Interact() override;

	/// <summary>
	/// �־��� index�� �ش��ϴ� �������� ������Ʈ�� ���� ���¸� ��ȯ�մϴ�. �ε��� ������ ���ܱ��� UI ������ �����մϴ�.
	/// </summary>
	/// <returns>������Ʈ�� ���� ����. �۵� ���� �����̰ų�, �Է��� �߸��� ��쿡�� 0�� ��ȯ</returns>
	EElectricState GetCurrentElectricState(int index) const;

	/// <summary>
	/// index�� �ش��ϴ� ���� ������Ʈ�� ���� ���¸� ����-�۵� ���̿��� ��ȣ ��ȯ�մϴ�.
	/// </summary>
	/// <param name="index">���� ������Ʈ �ε���. UI ������ ����</param>
	/// <returns>��ȯ�� �����ϸ� true��, �����ϸ� false�� ��ȯ</returns>
	bool SwitchElectricState(int index);
	/// <summary>
	/// �ش� index�� ���� ������Ʈ�� �����Ͻ�ŵ�ϴ�.
	/// </summary>
	/// <param name="index">���� ������Ʈ �ε���. UI ������ ����</param>
	/// <returns>�����Ͽ� �����ϸ� true��, �����ϸ� false�� ��ȯ</returns>
	bool OverdriveStart(int index);

	/// <summary>
	/// �����ϰ� ����Ǿ��� �� ȣ��Ǿ�� �մϴ�. ������ ���ܱⰡ ������ ���¸� �����ϱ� ���� ���˴ϴ�.
	/// </summary>
	/// <param name="ObjectName">���� ������Ʈ �̸�</param>
	void OverdriveEnd(FString ObjectName);
	/// <summary>
	/// ������, �ǵ����� ��� ����� �Ŀ� ȣ��Ǿ�� �մϴ�. ������ ���ŵ� ������ �����ϴ� �뵵�� ���˴ϴ�.
	/// </summary>
	/// <param name="ObjectName">���� ������Ʈ �̸�</param>
	void AllOverdriveProcessEnd(FString ObjectName);
};
