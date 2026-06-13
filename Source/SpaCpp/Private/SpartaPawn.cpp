#include "SpartaPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ASpartaPawn::ASpartaPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	//빙의시키기
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// 캡슐을 rootcomponent로 설정
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetSimulatePhysics(false);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = false; // 직접 회전 제어하므로 false

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
}

void ASpartaPawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (InputMappingContext)
				Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ASpartaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpartaPawn::Move);

		if (LookAction)
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASpartaPawn::Look);
		if (ElevateAction)
			EIC->BindAction(ElevateAction, ETriggerEvent::Triggered, this, &ASpartaPawn::Elevate);
		if (RollAction)
			EIC->BindAction(RollAction, ETriggerEvent::Triggered, this, &ASpartaPawn::Roll);
	}
}

void ASpartaPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Movement(DeltaTime);
	Gravity(DeltaTime);
}

//Tick에서 움직임을 담당하는 함수
void ASpartaPawn::Movement(float Deltatime)
{
	if (!FMath::IsNearlyZero(PendingMove.X) || !FMath::IsNearlyZero(PendingMove.Y))
	{
		//지상 - 공중 분기
		float ControlRatio = bIsGrounded ? 1.f : AirControlRatio;
		FVector Delta = (GetActorForwardVector() * PendingMove.X
					   + GetActorRightVector()   * PendingMove.Y)
					   * MoveSpeed * ControlRatio * Deltatime;
		AddActorWorldOffset(Delta, true);
		PendingMove.X = 0.f;  // X, Y만 초기화
		PendingMove.Y = 0.f;  // Z는 Gravity()가 처리한다.
	}

	if (!FMath::IsNearlyZero(PendingRoll))
	{
		AddActorLocalRotation(FRotator(0.f, 0.f, PendingRoll)); //롤은 델타타임 x
		PendingRoll = 0.f;
	}
}

//Tick에서 중력을 담당하는 함수
void ASpartaPawn::Gravity(float Deltatime)
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, CapsuleComp->GetScaledCapsuleHalfHeight() + 5.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//땅인지 확인하기
	bIsGrounded = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (bIsGrounded && VerticalVelocity < 0.f) // 떨어지는 중일 때만 초기화
		VerticalVelocity = 0.f;

	//하늘에서 상승,하강 가능
	if (!FMath::IsNearlyZero(PendingMove.Z) && !bIsGrounded)
	{
		VerticalVelocity = PendingMove.Z * ElevateForce; 
		PendingMove.Z = 0.f;
	}

	if (!bIsGrounded)
	{
		VerticalVelocity -= GravityAccel * Deltatime;
		VerticalVelocity = FMath::Max(VerticalVelocity, -MaxFallSpeed);
	}
	AddActorWorldOffset(FVector(0, 0, VerticalVelocity * Deltatime), true);
}


///키보드를 통해 작동하는 함수들
void ASpartaPawn::Move(const FInputActionValue& Value)
{
	// 입력값만 저장, 실제 이동은 Tick에서
	const FVector2D Input = Value.Get<FVector2D>();
	PendingMove.X = Input.X; // 전후
	PendingMove.Y = Input.Y; // 좌우
}

void ASpartaPawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>() * LookSensitivity;
	float PitchInput = bInvertPitch ? LookInput.Y : -LookInput.Y;
	AddActorLocalRotation(FRotator(PitchInput, LookInput.X, 0.f));
}

void ASpartaPawn::Roll(const FInputActionValue& Value)
{
	PendingRoll = Value.Get<float>() * RollSensitivity;
}

void ASpartaPawn::Elevate(const FInputActionValue& Value)
{
	PendingMove.Z = Value.Get<float>(); // 상하는 PendingMove.Z에
}
