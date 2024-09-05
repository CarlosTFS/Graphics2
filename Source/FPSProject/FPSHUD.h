#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "HealthIconsWidget.h"
#include "FPSHUD.generated.h"

// Declare delegates for button clicks
DECLARE_DYNAMIC_DELEGATE(FOnPlayButtonClickedDelegate);
DECLARE_DYNAMIC_DELEGATE(FOnQuitButtonClickedDelegate);

UCLASS()
class FPSPROJECT_API AFPSHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    UTexture2D* CrosshairTexture;

    /** The class of the Health Icons widget */
    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<UHealthIconsWidget> HealthIconsClass;

    UPROPERTY()
    UHealthIconsWidget* HealthIconsWidget;

public:
    virtual void DrawHUD() override;
    virtual void BeginPlay() override;

    // Function to update health in the HUD
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealth(int32 HealthCount);

    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<UUserWidget> MainWidgetClass;  // Reference to the BP_MainWidget2 class

    UPROPERTY()
    UUserWidget* MainWidgetInstance;  // Instance of the MainWidget

    // Declare delegates for button click events
    UPROPERTY()
    FOnPlayButtonClickedDelegate OnPlayButtonClickedDelegate;

    UPROPERTY()
    FOnQuitButtonClickedDelegate OnQuitButtonClickedDelegate;

    // Functions to handle button clicks
    UFUNCTION()
    void OnPlayButtonClicked();

    UFUNCTION()
    void OnQuitButtonClicked();
};
