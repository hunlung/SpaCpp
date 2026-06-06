#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpartaLocator.generated.h"

UCLASS()
class SPACPP_API ASpartaLocator : public AActor
{
	GENERATED_BODY()

public:
	ASpartaLocator();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locator|Movement")
	FVector PointBOffset;             // 에디터에서 B 지점 오프셋 지정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locator|Movement")
	float StayDuration;               // 각 지점에서 머무는 시간 (초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locator|Movement")
	float RandomStayRange;            // 머무는 시간 랜덤 가감 범위

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Locator|Components")
	UStaticMeshComponent* MeshComp;

private:
	void Teleport();

	FVector PointA;
	FVector PointB;
	bool bAtPointA;
	FTimerHandle TimerHandle_Move;
};