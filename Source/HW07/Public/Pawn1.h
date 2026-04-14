#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Pawn1.generated.h"

UCLASS()
class HW07_API APawn1 : public APawn
{
    GENERATED_BODY()

public:
    APawn1();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UCapsuleComponent* CapsuleComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USkeletalMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UCameraComponent* CameraComp;

    // 입력 설정 (이 이름들이 중요합니다)
    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* LookAction;

    // 이동 처리 함수
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    // 속도 변수
    float MoveSpeed = 600.0f;
    float LookSpeed = 60.0f;
};
