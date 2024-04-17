// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(InFireRange())
    {
        RotateTurret(Tank->GetActorLocation());
    }

}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();  // llama al handledestruction del BasePawn para activar los efectos de sonido y visuales
    Destroy();
}


void ATower::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));  // again, ATank es un hijo de APawn así que casteamos

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);  // NO DEVUELVE UN PUNTERO POR ESO LLAMAMOS A LA FUNCIÓN CON EL PUNTO -- CTR + SHIFT + SPACE PARA PARAMETERS HINT
}


void ATower::CheckFireCondition()
{
    if(Tank == nullptr) 
    {
        return;
    }

    if(InFireRange()  && Tank->bAlive)
    {
        Fire();
    }
}

bool ATower::InFireRange()
{
    if(Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

        if(Distance <= FireRange)
        {
            return true;
        }

    }
    return false;
}
