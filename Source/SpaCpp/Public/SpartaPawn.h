#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "SpartaPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SPACPP_API ASpartaPawn : public APawn
{
	GENERATED_BODY()

public:
	ASpartaPawn();

protected:
	//컴포넌트 추가
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	//인풋컨텍스트
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ElevateAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* RollAction;

	//이동속도 및 감도조절
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSpeed = 600.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float LookSensitivity = 1.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float RollSensitivity = 1.f; //회전민감도
	UPROPERTY(EditAnywhere, Category="Movement")
	bool bInvertPitch = true; // 인버트피치(마우스를 아래로하면 캐릭터가 위로 향하는 방식)
	// 중력
	UPROPERTY(EditAnywhere, Category="Movement")
	float GravityAccel = 980.f;
	// 최대 낙하 속도
	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxFallSpeed = 2000.f;
	// 공중에서의 속도
	UPROPERTY(EditAnywhere, Category="Movement")
	float AirControlRatio = 0.4f; // 40%
	//상승,하강 힘
	UPROPERTY(EditAnywhere, Category="Movement")
	float ElevateForce = 300.f;


	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

private:
	FVector PendingMove; // Tick에서 DeltaTime과 함께 처리
	float PendingRoll = 0.f;
	float VerticalVelocity = 0.f; // 현재 낙하 속도
	bool bIsGrounded = false; // 지상 여부

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Elevate(const FInputActionValue& Value);
	void Roll(const FInputActionValue& Value);
	
	void Movement(float Deltatime);
	void Gravity(float Deltatime);
};
