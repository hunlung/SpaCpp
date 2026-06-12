// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformSpawner.h"

// Sets default values
APlatformSpawner::APlatformSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APlatformSpawner::BeginPlay()
{
	Super::BeginPlay();
	OnSpawn();
}


void APlatformSpawner::OnSpawn()
{
	if (!isSpawn)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Spawner BeginPlay!"));

	if (PlatformClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Platform Classes Found!"));
		return;
	}

	FVector CurrentLocation = StartOffset;
	for (int i = 0; i < SpawnCount; i++)
	{
		//랜덤 위치용 오프셋
		FVector Offset(
			FMath::RandRange(MinOffset.X, MaxOffset.X),
			FMath::RandRange(-MinOffset.Y, MaxOffset.Y),
			FMath::RandRange(-MinOffset.Z, MaxOffset.Z));

		const int32 SpawnIndex = FMath::RandRange(0, PlatformClasses.Num() - 1);
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(
			PlatformClasses[SpawnIndex],
			CurrentLocation + Offset.X,
			FRotator::ZeroRotator);

		CurrentLocation += Offset;
	}
}
