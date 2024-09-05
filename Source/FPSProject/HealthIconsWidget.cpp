#include "HealthIconsWidget.h"
#include "Components/Image.h"

#include "HealthIconsWidget.h"

void UHealthIconsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Check that the widget components are properly initialized
    if (HealthIcon1)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthIcon1 is initialized."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HealthIcon1 is null!"));
    }

    if (HealthIcon2)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthIcon2 is initialized."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HealthIcon2 is null!"));
    }

    if (HealthIcon3)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthIcon3 is initialized."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HealthIcon3 is null!"));
    }
}