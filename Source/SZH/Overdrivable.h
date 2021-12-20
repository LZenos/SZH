// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UObject/Interface.h"
#include "Overdrivable.generated.h"

UENUM(BlueprintType)
enum class EElectricState : uint8
{
	E_PAUSE UMETA(DisplayName = "Pause"),
	E_OPERATE UMETA(DisplayName = "Operate"),
	E_OVERDRIVE UMETA(DisplayName = "Overdrive")
};

UINTERFACE(MinimalAPI)
class UOverdrivable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SZH_API IOverdrivable
{
	GENERATED_BODY()

private:
	/// <summary>
	/// ��ü ��� ���� ����. ���� ���� �ı��� ���� ������
	/// </summary>
	bool available;

protected:
	/// <summary>
	/// ������ ���� �ð�. ����: 1��
	/// </summary>
	float overdriveTime;
	/// <summary>
	/// �ǵ��(����) ���� �ð�. ����: 1��
	/// </summary>
	float feedbackTime;

	/// <summary>
	/// ���� ��뷮. 0: ����, 1: �⺻, 2: ������
	/// </summary>
	EElectricState electricState;
	/// <summary>
	/// ������ ���� ����
	/// </summary>
	bool isOverdriving;
	/// <summary>
	/// �ǵ��(����) ���� ����
	/// </summary>
	bool isFeedback;
	float currentOverdriveTime;

	/// <summary>
	/// �ش� ���� ��ü�� �����ϴ� ���ܱ� ��ü
	/// </summary>
	class AGenerator* generatorActor;


	/// <summary>
	/// Ư���� ��� ������ ���� ������ ��ü�� �⺻���� ��������� �ʱ�ȭ ���ݴϴ�. �����ڿ��� ȣ���ϴ� ���� �����ϴ�.
	/// </summary>
	/// <param name="overdrive_time">������ ���ӽð�</param>
	/// <param name="feedback_time">�ǵ��(����) ���ӽð�</param>
	void InitOverdriveMembers(float overdrive_time, float feedback_time);
	/// <summary>
	/// �����Ͽ� �ǵ�� ���� �ð��� ����մϴ�. Tick �Լ� ������ ȣ��Ǿ�� �մϴ�.
	/// </summary>
	void TickOverdrive(float DeltaTime);

	/// <summary>
	/// �����ϸ� �����մϴ�.
	/// </summary>
	virtual void Overdrive() = 0;
	/// <summary>
	/// �ǵ���� �����մϴ�.
	/// </summary>
	virtual void FeedBack();
	/// <summary>
	/// �ǵ��(����)�� ����Ǿ��� �� ȣ��˴ϴ�.
	/// </summary>
	virtual void OnFeedbackEnd() {}

	/// <summary>
	/// ���°� ������ �� ȣ��˴ϴ�.
	/// </summary>
	/// <param name="NewState">��ȸ�� ����</param>
	virtual void OnStateChange(EElectricState NewState) {}

	/// <summary>
	/// �ش� ���� ��ü�� ����� �� ���� ���·� ����ϴ�. ���� �ı� � ���˴ϴ�.
	/// </summary>
	void ShutDown();
	/// <summary>
	/// �ش� ���� ��ü�� ��� ���� ���θ� ��ȯ�մϴ�. ���� �������� ���� ���Դϴ�.
	/// </summary>
	/// <returns></returns>
	bool IsAvailable() const;

public:
	/// <summary>
	/// �ش� ���� ��ü�� ������ ���� ���ܱ� ��ü�� ����մϴ�.
	/// </summary>
	void InitGeneratorActorPointer(AGenerator* actor);
	
	/// <summary>
	/// ���� ���� ���¸� �޾ƿɴϴ�.
	/// </summary>
	EElectricState GetElectricState() const;
	/// <summary>
	/// ���� ��� ���¸� �����մϴ�.
	/// </summary>
	/// <param name="state">���� ������ ����</param>
	/// <returns>���� �� true, ���� �� false ��ȯ</returns>
	bool SetElectricState(EElectricState state);
};
