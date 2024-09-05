#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "HealthIconsWidget.generated.h"

UCLASS()
class FPSPROJECT_API UHealthIconsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
     // Override the NativeConstruct function from UUserWidget
    virtual void NativeConstruct() override;
    // Declare UImage variables with UPROPERTY
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* HealthIcon1;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* HealthIcon2;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* HealthIcon3;

    // Declare a method to update health icons
    void UpdateHealthIcons(int32 HealthCount);
};
