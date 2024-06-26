// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    if(DeadActor == Tank)
    {
        Tank->HandleDestruction();
        // if(Tank->GetTankPlayerController())
        // {
        //     Tank->DisableInput(Tank->GetTankPlayerController());
        //     Tank->GetTankPlayerController()->bShowMouseCursor = false;
        // }
        if(ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    } 
    else if(ATower* DestroyedTower = Cast<ATower>(DeadActor)) 
    {
        DestroyedTower->HandleDestruction();
        --TargetTowers;
        if(TargetTowers == 0)
        {
            GameOver(true);
        }
    }
}

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();

}

void AToonTanksGameMode::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if(ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;

        // Timer delegate en vez de callback a función en este caso ya que la función a la que queremos llamar está en playercontroller no en la clase actual y requiere de parámetros de entrada
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, 
            true
        );
        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle,
            PlayerEnableTimerDelegate,
            StartDelay,
            false
        );
    }
}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}