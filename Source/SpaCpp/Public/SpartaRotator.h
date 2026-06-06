#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpartaRotator.generated.h"

UCLASS()
class SPACPP_API ASpartaRotator : public AActor
{
	GENERATED_BODY()

public:
	ASpartaRotator();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotator|Rotation")
	float BaseRotationSpeed;          // 기본 회전 속도 (도/초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotator|Rotation")
	float RandomSpeedRange;           // 랜덤 가감 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotator|Visibility")
	float VisibleDuration;            // 보이는 시간 (초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotator|Visibility")
	float HiddenDuration;             // 사라져 있는 시간 (초)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotator|Components")
	UStaticMeshComponent* MeshComp;

private:
	void HideActor();
	void ShowActor();

	float CurrentRotationSpeed;
	bool bIsVisible;
	FTimerHandle TimerHandle_Visibility;
};