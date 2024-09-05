#include "DefeatWidgetFinal.h"
#include "Components/Button.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

void UDefeatWidgetFinal::NativeConstruct()
{
    Super::NativeConstruct();

    // Find the Restart_Button
    UButton* RestartButton = Cast<UButton>(GetWidgetFromName(TEXT("Restart_Button")));
    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UDefeatWidgetFinal::OnRestartButtonClicked);
    }
}

void UDefeatWidgetFinal::OnRestartButtonClicked()
{
    if (APlayerController* PlayerController = GetOwningPlayer())
    {
        if (AFPSCharacter* Character = Cast<AFPSCharacter>(PlayerController->GetCharacter()))
        {
            // Call the character's function to restart the game
            Character->OnDefeatRestartButtonClicked();
        }
    }
}
