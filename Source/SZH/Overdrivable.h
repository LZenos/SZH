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
	/// 객체 사용 가능 여부. 오직 문의 파괴를 위해 생성됨
	/// </summary>
	bool available;

protected:
	/// <summary>
	/// 과부하 지속 시간. 단위: 1초
	/// </summary>
	float overdriveTime;
	/// <summary>
	/// 피드백(정전) 지속 시간. 단위: 1초
	/// </summary>
	float feedbackTime;

	/// <summary>
	/// 전력 사용량. 0: 정전, 1: 기본, 2: 과부하
	/// </summary>
	EElectricState electricState;
	/// <summary>
	/// 과부하 진행 여부
	/// </summary>
	bool isOverdriving;
	/// <summary>
	/// 피드백(정전) 진행 여부
	/// </summary>
	bool isFeedback;
	float currentOverdriveTime;

	/// <summary>
	/// 해당 전력 객체를 관리하는 차단기 객체
	/// </summary>
	class AGenerator* generatorActor;


	/// <summary>
	/// 특히나 멤버 변수가 많은 과부하 객체의 기본적인 멤버변수를 초기화 해줍니다. 생성자에서 호출하는 것이 좋습니다.
	/// </summary>
	/// <param name="overdrive_time">과부하 지속시간</param>
	/// <param name="feedback_time">피드백(정전) 지속시간</param>
	void InitOverdriveMembers(float overdrive_time, float feedback_time);
	/// <summary>
	/// 과부하와 피드백 지속 시간을 계산합니다. Tick 함수 내에서 호출되어야 합니다.
	/// </summary>
	void TickOverdrive(float DeltaTime);

	/// <summary>
	/// 과부하를 진행합니다.
	/// </summary>
	virtual void Overdrive() = 0;
	/// <summary>
	/// 피드백을 진행합니다.
	/// </summary>
	virtual void FeedBack();
	/// <summary>
	/// 피드백(정전)이 종료되었을 때 호출됩니다.
	/// </summary>
	virtual void OnFeedbackEnd() {}

	/// <summary>
	/// 상태가 변했을 때 호출됩니다.
	/// </summary>
	/// <param name="NewState">변회된 상태</param>
	virtual void OnStateChange(EElectricState NewState) {}

	/// <summary>
	/// 해당 전력 객체를 사용할 수 없는 상태로 만듭니다. 문의 파괴 등에 사용됩니다.
	/// </summary>
	void ShutDown();
	/// <summary>
	/// 해당 전력 객체의 사용 가능 여부를 반환합니다. 오직 문에서만 사용될 것입니다.
	/// </summary>
	/// <returns></returns>
	bool IsAvailable() const;

public:
	/// <summary>
	/// 해당 전력 객체를 관리할 전력 차단기 객체를 등록합니다.
	/// </summary>
	void InitGeneratorActorPointer(AGenerator* actor);
	
	/// <summary>
	/// 현재 전력 상태를 받아옵니다.
	/// </summary>
	EElectricState GetElectricState() const;
	/// <summary>
	/// 전력 사용 상태를 설정합니다.
	/// </summary>
	/// <param name="state">새로 설정할 상태</param>
	/// <returns>성공 시 true, 실패 시 false 반환</returns>
	bool SetElectricState(EElectricState state);
};
