// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GeneratorWidget.generated.h"

/**
 *
 */
UCLASS()
class SZH_API UGeneratorWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY()
	class AGenerator* generatorActor;

	UPROPERTY()
	TArray<class UButton*> leakBreakerButtons;
	UPROPERTY()
	TArray<UButton*> overdriveButtons;

public:
	UPROPERTY(Meta = (BindWidget))
	UButton* btn_leakBreaker_1;
	UPROPERTY(Meta = (BindWidget))
	UButton* btn_leakBreaker_2;
	UPROPERTY(Meta = (BindWidget))
	UButton* btn_leakBreaker_3;
	UPROPERTY(Meta = (BindWidget))
	UButton* btn_leakBreaker_4;
	UPROPERTY(Meta = (BindWidget))
	UButton* btn_leakBreaker_5;
	UPROPERTY(Meta = (BindWidget))
	UButton* btn_overdrive_1;
	UPROPERTY(Meta = (BindWidget))
	UButton* btn_overdrive_2;
	UPROPERTY(Meta = (BindWidget))
	UButton* btn_overdrive_3;
	UPROPERTY(Meta = (BindWidget))
	UButton* btn_overdrive_4;
	UPROPERTY(Meta = (BindWidget))
	UButton* btn_overdrive_5;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* txt_GeneratorLevel;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* txt_CurrentPower;


	/// <summary>
	/// 위젯 액터에게 주인님이 누구인지 똑.똑.히 알려줍니다. 위젯 생성 시에 호출되어야 합니다.
	/// </summary>
	void InitOwner(AGenerator* Owner);

	/// <summary>
	/// 전력 차단기의 업그레이드 단계 텍스트를 NewLevel 값으로 갱신합니다.
	/// </summary>
	void UpdateGeneratorLevel(int NewLevel);
	/// <summary>
	/// 여분의 전력에 대한 텍스트를 Surplus 값으로 갱신합니다.
	/// </summary>
	void UpdateElectricSurplus(int Surplus);

	// Switch operation of electric objects. This method just need to be connected with OnClick event. ObjectIndex should be set.
	UFUNCTION(BlueprintCallable)
	void SwitchOperation(int ObjectIndex);
	// Try to overdrive. This method just need to be connected with OnClick event. ObjectIndex should be set.
	UFUNCTION(BlueprintCallable)
	void SetOverdrive(int ObjectIndex);

	/// <summary>
	/// 강제로 전력 차단기를 내려버립니다. 일반적으로 전력 오브젝트의 과부하가 끝나고 피드백 상태로 돌입할 때 호출됩니다.
	/// </summary>
	void ForceOffLeakBreaker(int ObjectIndex);
	/// <summary>
	/// 과부하 버튼을 다시 활성화시킵니다. 일반적으로 전력 오브젝트의 피드백까지 모두 종료되었을 때 호출됩니다.
	/// </summary>
	void ActivateOverdriveButton(int ObjectIndex);
};
