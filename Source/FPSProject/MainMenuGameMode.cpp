// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuHudClass == nullptr)
    {
        static ConstructorHelpers::FClassFinder<UUserWidget> MenuHudBPClass(TEXT("/All/Game/HUD/MainMenu"));
        if (MenuHudBPClass.Succeeded())
        {
            MainMenuHudClass = MenuHudBPClass.Class;
        }
    }

    if (MainMenuHudClass)
    {
        UUserWidget* MainMenuHud = CreateWidget<UUserWidget>(GetWorld(), MainMenuHudClass);
        if (MainMenuHud)
        {
            MainMenuHud->AddToViewport();
        }
    }
}
