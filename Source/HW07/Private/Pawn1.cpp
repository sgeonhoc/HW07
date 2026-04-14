#include "Pawn1.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APawn1::APawn1()
{
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    RootComponent = CapsuleComp;
    CapsuleComp->SetSimulatePhysics(false);
    CapsuleComp->InitCapsuleSize(34.0f, 88.0f);

    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
    MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 400.0f;
    SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
    SpringArmComp->bUsePawnControlRotation = false;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp);
}

void APawn1::BeginPlay()
{
    Super::BeginPlay(); // 여기서 APawn1의 부모인 Super를 호출합니다.

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }
}

void APawn1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APawn1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APawn1::Move);
        EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APawn1::Look);
    }
}

void APawn1::Move(const FInputActionValue& Value)
{
    FVector2D MoveVector = Value.Get<FVector2D>();
    float DeltaTime = GetWorld()->GetDeltaSeconds();

    if (Controller != nullptr)
    {
        FVector ForwardDirection = GetActorForwardVector() * MoveVector.Y;
        FVector RightDirection = GetActorRightVector() * MoveVector.X;
        FVector DeltaLocation = (ForwardDirection + RightDirection) * MoveSpeed * DeltaTime;

        AddActorLocalOffset(DeltaLocation, true);
    }
}

void APawn1::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    float DeltaTime = GetWorld()->GetDeltaSeconds();

    float YawAmount = LookAxisVector.X * LookSpeed * DeltaTime;
    AddActorWorldRotation(FRotator(0.0f, YawAmount, 0.0f));

    float PitchAmount = LookAxisVector.Y * LookSpeed * DeltaTime;
    FRotator NewArmRotation = SpringArmComp->GetRelativeRotation();

    NewArmRotation.Pitch += PitchAmount;
    NewArmRotation.Yaw = 0.0f;
    NewArmRotation.Roll = 0.0f;
    NewArmRotation.Pitch = FMath::Clamp(NewArmRotation.Pitch, -80.0f, 80.0f);

    SpringArmComp->SetRelativeRotation(NewArmRotation);
}
