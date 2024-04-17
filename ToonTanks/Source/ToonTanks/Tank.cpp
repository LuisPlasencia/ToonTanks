// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATank::ATank()
{
    // :: scope resolution operator
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Sprint Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);  // en funciones con virtual delante (override) es siempre aconsejable usar el super

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);  // this apunta a la instancia del objeto en el juego
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn); 
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire); 
}


void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility, 
            false,
            HitResult);
        // DrawDebugSphere(
        //     GetWorld(), 
        //     HitResult.ImpactPoint, 
        //     25.f,
        //     12,
        //     FColor::Red,
        //     false,
        //     -1.f);    // -1 = solo en un frame
        RotateTurret(HitResult.ImpactPoint);

    }
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;
}


void ATank::BeginPlay()
{
    Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());  // esto lo podemos hacer poruqe GetController devuelve un objeto APlayerController almacenado en un puntero AController, el cual es padre de APlayerController así que podemos extraerselo, no podriamos hacerlo al revés porque solo podemos castear a un hijo del padre y no a un padre del hijo

}

void ATank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector; // aqui llamamos a una variable estática, es lo mismo que decir FVector DeltaLocation(0.f);
    // X = Value * DeltaTime * Speed    
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);
}


void ATank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator; // = FVector DeltaLocation(0.f);   // PITCH (Y AXIS) , YAW (Z AXIS), ROLL (X AXIS)
    // Yaw = Value * DeltaTime * TurnRate
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);

}



