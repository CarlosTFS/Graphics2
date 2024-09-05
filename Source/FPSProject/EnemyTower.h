#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "FPSProjectile.h"
#include "EnemyTower.generated.h"

UCLASS()
class FPSPROJECT_API AEnemyTower : public AActor
{
    GENERATED_BODY()

public:
    AEnemyTower();
    void ReduceTowerLife();
    int32 GetTowerLife() const;
    int32 towerLife = 3;
    void SetTowerLife(int32 NewTowerLife);

    bool bIsPlayerDetected; // To keep track of whether the player is detected
    FTimerHandle FireTimerHandle; // To handle the firing timer

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Add this line

public:
    virtual void Tick(float DeltaTime) override;

  

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* TurretBody;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* TurretFace; // Change this to UStaticMeshComponent

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAISenseConfig_Sight* SightConfig;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<AFPSProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FVector MuzzleOffset;

    void SetupPerceptionSystem();
    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    void FireAtPlayer();
};