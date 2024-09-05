#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VictoryWidgetFinal.generated.h"

UCLASS()
class FPSPROJECT_API UVictoryWidgetFinal : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnRestartButtonClicked();
};
