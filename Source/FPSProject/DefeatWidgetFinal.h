#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DefeatWidgetFinal.generated.h"

UCLASS()
class FPSPROJECT_API UDefeatWidgetFinal : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnRestartButtonClicked();
};
