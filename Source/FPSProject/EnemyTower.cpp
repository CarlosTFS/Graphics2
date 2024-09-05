#include "EnemyTower.h"
#include "Components/StaticMeshComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "FPSProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AEnemyTower::AEnemyTower()
{
    PrimaryActorTick.bCanEverTick = true;

    // Initialize TurretBody component
    TurretBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBody"));
    RootComponent = TurretBody;

    // Initialize TurretFace component
    TurretFace = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretFace"));
    TurretFace->SetupAttachment(TurretBody);

    // Initialize AI Perception components
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    // Setup perception system
    SetupPerceptionSystem();


    // Initialize MuzzleOffset (top of TurretFace)
    MuzzleOffset = FVector(0.0f, 0.0f, 150.0f);  // Adjust as needed
}

void AEnemyTower::BeginPlay()
{
    Super::BeginPlay();

    if (AIPerceptionComponent)
    {
        AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyTower::OnPerceptionUpdated);
    }

    // Initialize detection flag
    bIsPlayerDetected = false;
}



void AEnemyTower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TurretFace)
    {
        // Visualize the sight detection radius
        DrawDebugSphere(GetWorld(), TurretFace->GetComponentLocation(), SightConfig->SightRadius, 12, FColor::Green, false, -1, 0, 2);

         // Visualize the TurretFace's location
        DrawDebugSphere(GetWorld(), TurretFace->GetComponentLocation(), 10.0f, 12, FColor::Blue, false, 5.0f);

        // Calculate and draw the muzzle location
        FVector MuzzleLocation = TurretFace->GetComponentLocation() + MuzzleOffset;
        DrawDebugSphere(GetWorld(), MuzzleLocation, 10.0f, 12, FColor::Red, false, -1, 0, 2);
    }
}

void AEnemyTower::SetupPerceptionSystem()
{
    if (SightConfig)
    {
        SightConfig->SightRadius = 1000.0f;
        SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.0f;
        SightConfig->PeripheralVisionAngleDegrees = 90.0f;
        SightConfig->SetMaxAge(5.0f);

        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

        AIPerceptionComponent->ConfigureSense(*SightConfig);
        AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
    }
}

void AEnemyTower::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("Perception Updated! Actor: %s, Successfully Sensed: %s"), *Actor->GetName(), Stimulus.WasSuccessfullySensed() ? TEXT("Yes") : TEXT("No"));

    if (Stimulus.WasSuccessfullySensed())
    {
        if (!bIsPlayerDetected)
        {
            // Player detected, start the timer
            bIsPlayerDetected = true;
            GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AEnemyTower::FireAtPlayer, 1.0f, true);
        }
    }
    else
    {
        if (bIsPlayerDetected)
        {
            // Player lost, stop the timer
            bIsPlayerDetected = false;
            GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
        }
    }
}



void AEnemyTower::FireAtPlayer()
{
    if (ProjectileClass)
    {
        FVector MuzzleLocation = TurretFace->GetComponentLocation() + MuzzleOffset;
        UE_LOG(LogTemp, Warning, TEXT("MuzzleLocation: %s"), *MuzzleLocation.ToString());

        AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerPawn)
        {
            FVector Direction = (PlayerPawn->GetActorLocation() - MuzzleLocation).GetSafeNormal();
            FRotator MuzzleRotation = Direction.Rotation();
            // Debugging output
            UE_LOG(LogTemp, Warning, TEXT("Muzzle Location: %s, Direction: %s"), *MuzzleLocation.ToString(), *Direction.ToString());

            UWorld* World = GetWorld();
            if (World)
            {
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.Instigator = GetInstigator();

                UE_LOG(LogTemp, Warning, TEXT("Spawning Projectile at %s"), *MuzzleLocation.ToString());

                AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
                if (Projectile)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Projectile spawned successfully"));
                    Projectile->FireInDirection(Direction);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to spawn projectile"));
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ProjectileClass is not set"));
    }
}



int32 AEnemyTower::GetTowerLife() const
{
    return towerLife;
}

void AEnemyTower::SetTowerLife(int32 NewTowerLife)
{
    towerLife = NewTowerLife;
}

void AEnemyTower::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // Clear the timer when the actor is destroyed or removed from the scene
    GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}