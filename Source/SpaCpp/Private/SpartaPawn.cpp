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
    }
}

void ASpartaPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PendingMove.IsNearlyZero())
    {
        FVector Delta = GetActorForwardVector() * PendingMove.X * MoveSpeed * DeltaTime
                      + GetActorRightVector()   * PendingMove.Y * MoveSpeed * DeltaTime;
        AddActorWorldOffset(Delta, true); // 바라보는 방향으로 이동하게 하기 위해 월드오프셋으로 변경
        PendingMove = FVector2D::ZeroVector;
    }
}

void ASpartaPawn::Move(const FInputActionValue& Value)
{
    // 입력값만 저장, 실제 이동은 Tick에서
    PendingMove = Value.Get<FVector2D>();
}

void ASpartaPawn::Look(const FInputActionValue& Value)
{
    const FVector2D LookInput = Value.Get<FVector2D>() * LookSensitivity;

    // Yaw: Pawn 전체를 좌우 회전
    AddActorLocalRotation(FRotator(0.f, LookInput.X, 0.f));

    // Pitch: SpringArm만 상하 회전 (Pitch 각도 누적 후 Clamp)
    CurrentPitch = FMath::Clamp(CurrentPitch - LookInput.Y, -PitchLimit, PitchLimit);
    SpringArmComp->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));
}