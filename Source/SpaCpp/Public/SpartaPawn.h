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
	UPROPERTY(VisibleAnywhere) UCapsuleComponent*      CapsuleComp;
	UPROPERTY(VisibleAnywhere) USkeletalMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere) USpringArmComponent*    SpringArmComp;
	UPROPERTY(VisibleAnywhere) UCameraComponent*       CameraComp;

	//인풋컨텍스트
	UPROPERTY(EditAnywhere, Category="Input") UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, Category="Input") UInputAction*         MoveAction;
	UPROPERTY(EditAnywhere, Category="Input") UInputAction*         LookAction;

	//이동속도 및 감도조절
	UPROPERTY(EditAnywhere, Category="Movement") float MoveSpeed       = 600.f;
	UPROPERTY(EditAnywhere, Category="Movement") float LookSensitivity = 1.f;
	UPROPERTY(EditAnywhere, Category="Movement") float PitchLimit      = 80.f;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

private:
	FVector2D PendingMove;  // Tick에서 DeltaTime과 함께 처리
	float     CurrentPitch = 0.f;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};