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
	/// ���� ���Ϳ��� ���δ��� �������� ��.��.�� �˷��ݴϴ�. ���� ���� �ÿ� ȣ��Ǿ�� �մϴ�.
	/// </summary>
	void InitOwner(AGenerator* Owner);

	/// <summary>
	/// ���� ���ܱ��� ���׷��̵� �ܰ� �ؽ�Ʈ�� NewLevel ������ �����մϴ�.
	/// </summary>
	void UpdateGeneratorLevel(int NewLevel);
	/// <summary>
	/// ������ ���¿� ���� �ؽ�Ʈ�� Surplus ������ �����մϴ�.
	/// </summary>
	void UpdateElectricSurplus(int Surplus);

	// Switch operation of electric objects. This method just need to be connected with OnClick event. ObjectIndex should be set.
	UFUNCTION(BlueprintCallable)
	void SwitchOperation(int ObjectIndex);
	// Try to overdrive. This method just need to be connected with OnClick event. ObjectIndex should be set.
	UFUNCTION(BlueprintCallable)
	void SetOverdrive(int ObjectIndex);

	/// <summary>
	/// ������ ���� ���ܱ⸦ ���������ϴ�. �Ϲ������� ���� ������Ʈ�� �����ϰ� ������ �ǵ�� ���·� ������ �� ȣ��˴ϴ�.
	/// </summary>
	void ForceOffLeakBreaker(int ObjectIndex);
	/// <summary>
	/// ������ ��ư�� �ٽ� Ȱ��ȭ��ŵ�ϴ�. �Ϲ������� ���� ������Ʈ�� �ǵ����� ��� ����Ǿ��� �� ȣ��˴ϴ�.
	/// </summary>
	void ActivateOverdriveButton(int ObjectIndex);
};
