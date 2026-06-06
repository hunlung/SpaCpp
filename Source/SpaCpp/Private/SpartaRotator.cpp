#include "SpartaRotator.h"

ASpartaRotator::ASpartaRotator()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	BaseRotationSpeed = 90.0f;
	RandomSpeedRange  = 30.0f;
	VisibleDuration   = 3.0f;
	HiddenDuration    = 2.0f;
	bIsVisible        = true;
}

void ASpartaRotator::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시 속도에 랜덤 가미
	CurrentRotationSpeed = BaseRotationSpeed + FMath::RandRange(-RandomSpeedRange, RandomSpeedRange);

	GetWorldTimerManager().SetTimer(
		TimerHandle_Visibility, this, &ASpartaRotator::HideActor, VisibleDuration, false);
}

void ASpartaRotator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsVisible)
	{
		AddActorLocalRotation(FRotator(0.0f, CurrentRotationSpeed * DeltaTime, 0.0f));
	}
}

//액터 숨기기
void ASpartaRotator::HideActor()
{
	bIsVisible = false;
	MeshComp->SetVisibility(false); //숨기기

	//콜리전용으로 제작한 자식스태틱 메시들 찾기
	TArray<UStaticMeshComponent*> Meshes;
	GetComponents<UStaticMeshComponent>(Meshes);
	for (UStaticMeshComponent* Mesh : Meshes)
	{
		if (Mesh == MeshComp) continue;   // MeshComp는 건너뜀(없기때문)
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 자식메시들 콜리전 제거
	}

	GetWorldTimerManager().SetTimer(
		TimerHandle_Visibility, this, &ASpartaRotator::ShowActor, HiddenDuration, false);
}

//사라진 액터 복구
void ASpartaRotator::ShowActor()
{
	bIsVisible = true;
	MeshComp->SetVisibility(true);

	TArray<UStaticMeshComponent*> Meshes;
	GetComponents<UStaticMeshComponent>(Meshes);
	for (UStaticMeshComponent* Mesh : Meshes)
	{
		if (Mesh == MeshComp) continue;   // MeshComp는 건너뜀
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	CurrentRotationSpeed = BaseRotationSpeed + FMath::RandRange(-RandomSpeedRange, RandomSpeedRange);

	GetWorldTimerManager().SetTimer(
		TimerHandle_Visibility, this, &ASpartaRotator::HideActor, VisibleDuration, false);
}