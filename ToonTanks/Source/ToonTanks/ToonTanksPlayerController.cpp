// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"
#include "GameFramework/Pawn.h"

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    if(bPlayerEnabled)
    {
        GetPawn()->EnableInput(this);   // get pawn nos devuelve al jugador al que esta poseyendo el player controller 
    }
    else 
    {
        GetPawn()->DisableInput(this);
    }
    bShowMouseCursor = bPlayerEnabled;
}