#include "VictoryWidgetFinal.h"
#include "Components/Button.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

void UVictoryWidgetFinal::NativeConstruct()
{
    Super::NativeConstruct();

    // Find the Restart_Button
    UButton* RestartButton = Cast<UButton>(GetWidgetFromName(TEXT("Restart_Button")));
    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UVictoryWidgetFinal::OnRestartButtonClicked);
    }
}

void UVictoryWidgetFinal::OnRestartButtonClicked()
{
    if (APlayerController* PlayerController = GetOwningPlayer())
    {
        if (AFPSCharacter* Character = Cast<AFPSCharacter>(PlayerController->GetCharacter()))
        {
            // Call the character's function to restart the game
            Character->OnVictoryRestartButtonClicked();
        }
    }
}
