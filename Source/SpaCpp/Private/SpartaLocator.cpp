#include "SpartaLocator.h"

ASpartaLocator::ASpartaLocator()
{
	PrimaryActorTick.bCanEverTick = false; // 순간이동이라 Tick 불필요

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	
	StayDuration = 2.0f;
	RandomOffsetRange = 200.0f;
	RandomStayRange = 0.5f;
	bAtPointA = true;
}

FVector ASpartaLocator::RandomLocation()
{
	FVector Location = FVector(0.0f, FMath::RandRange(-RandomOffsetRange-60, RandomOffsetRange+60),
	                           FMath::RandRange(-RandomOffsetRange+50, RandomOffsetRange));

	return Location;
}

void ASpartaLocator::BeginPlay()
{
	Super::BeginPlay();
	PointBOffset = RandomLocation();
	PointA = GetActorLocation();
	PointB = PointA + PointBOffset;

	float FirstDelay = StayDuration + FMath::RandRange(-RandomStayRange, RandomStayRange);
	GetWorldTimerManager().SetTimer(
		TimerHandle_Move, this, &ASpartaLocator::Teleport, FirstDelay, false);
}

void ASpartaLocator::Teleport()
{
	bAtPointA = !bAtPointA;
	SetActorLocation(bAtPointA ? PointA : PointB);

	// 도착 후 다음 이동까지 대기 시간에 랜덤 가미
	float NextDelay = StayDuration + FMath::RandRange(-RandomStayRange, RandomStayRange);
	GetWorldTimerManager().SetTimer(
		TimerHandle_Move, this, &ASpartaLocator::Teleport, NextDelay, false);
}
