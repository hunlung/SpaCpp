// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformSpawner.generated.h"

UCLASS()
class SPACPP_API APlatformSpawner : public AActor
{
	GENERATED_BODY()

private:

public:
	// Sets default values for this actor's properties
	APlatformSpawner();
	UPROPERTY(EditAnywhere, Category = "Spawn")
	bool isSpawn = true;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<AActor>> PlatformClasses;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 SpawnCount = 5;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector StartOffset = FVector{0, 0, 110};
	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector MinOffset = FVector(300, -80, 70);
	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector MaxOffset = FVector{600, 80, 70};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void OnSpawn();
};
