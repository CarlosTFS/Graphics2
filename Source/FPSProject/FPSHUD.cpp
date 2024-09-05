#include "FPSHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "HealthIconsWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"  // For quitting the game
#include "Kismet/GameplayStatics.h"

// Draw the crosshair
void AFPSHUD::DrawHUD()
{
    Super::DrawHUD();

    if (CrosshairTexture)
    {
        FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
        FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));
        FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->GetResource(), FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);
    }
}

// Initialize HUD and widgets
void AFPSHUD::BeginPlay()
{
    Super::BeginPlay();

    // Add the Health Icons Widget to the viewport
    if (HealthIconsClass)
    {
        HealthIconsWidget = CreateWidget<UHealthIconsWidget>(GetWorld(), HealthIconsClass);
        if (HealthIconsWidget)
        {
            HealthIconsWidget->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("HealthIconsWidget created and added to viewport."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create HealthIconsWidget."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HealthIconsClass is not set in FPSHUD."));
    }

    // Add the BP_MainWidget2 to the viewport
    if (MainWidgetClass)
    {
        MainWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
        if (MainWidgetInstance)
        {
            MainWidgetInstance->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("BP_MainWidget2 created and added to viewport."));

            // Set input mode to UI only and show the mouse cursor
           /*APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PlayerController)
            {
               FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(MainWidgetInstance->TakeWidget());
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = true; // Ensure the mouse cursor is visible
            }*/

            // Set focus to the widget
            //MainWidgetInstance->SetFocus();

            // Find the buttons in the widget
            UButton* PlayButton = Cast<UButton>(MainWidgetInstance->GetWidgetFromName(TEXT("Play_Button_1")));
            UButton* QuitButton = Cast<UButton>(MainWidgetInstance->GetWidgetFromName(TEXT("Quit_Button_1")));

            if (PlayButton)
            {
                UE_LOG(LogTemp, Log, TEXT("Play_Button_1 found."));
                PlayButton->OnClicked.AddDynamic(this, &AFPSHUD::OnPlayButtonClicked);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Play_Button_1 not found!"));
            }

            if (QuitButton)
            {
                UE_LOG(LogTemp, Log, TEXT("Quit_Button_1 found."));
                QuitButton->OnClicked.AddDynamic(this, &AFPSHUD::OnQuitButtonClicked);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Quit_Button_1 not found!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create BP_MainWidget2."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MainWidgetClass is not set in FPSHUD."));
    }
}

// Update health icons based on the health count
void AFPSHUD::UpdateHealth(int32 HealthCount)
{
    if (HealthIconsWidget)
    {
        // Clamp HealthCount to be within a valid range
        HealthCount = FMath::Clamp(HealthCount, 0, 3);

        // Set visibility of each health icon based on HealthCount
        HealthIconsWidget->HealthIcon1->SetVisibility(HealthCount > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        HealthIconsWidget->HealthIcon2->SetVisibility(HealthCount > 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        HealthIconsWidget->HealthIcon3->SetVisibility(HealthCount > 2 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

        // Optional: Add logging for debugging
        UE_LOG(LogTemp, Log, TEXT("Health updated: %d"), HealthCount);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HealthIconsWidget is not valid."));
    }
}

void AFPSHUD::OnPlayButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Play button clicked."));

    if (MainWidgetInstance)
    {
        MainWidgetInstance->SetVisibility(ESlateVisibility::Hidden);  // Hide the widget
        UE_LOG(LogTemp, Log, TEXT("MainWidget hidden."));
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        PlayerController->SetPause(false);
    }

    // Resume the game if it's paused
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->SetPause(false);
        UE_LOG(LogTemp, Log, TEXT("Game resumed."));
    }

    // Optionally, trigger delegate if needed
    // OnPlayButtonClickedDelegate.ExecuteIfBound();
}


// Handle Quit button click
void AFPSHUD::OnQuitButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Quit button clicked."));
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);

    // Optionally, trigger delegate if needed
    // OnQuitButtonClickedDelegate.ExecuteIfBound();
}
