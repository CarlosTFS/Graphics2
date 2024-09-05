#include "HealthPickUp.h"
#include "FPSCharacter.h"  // Include the header file for AFPSCharacter
#include "FPSHUD.h"  // Include the header file for AFPSHUD

// Sets default values
AHealthPickUp::AHealthPickUp()
{
	// Set this actor to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the collision component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Pickup")); // Use appropriate profile

	// Set up collision responses
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); // Adjust as needed
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore); // Ignore all by default
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Respond to Pawn channel

	// Bind the OnHit function to the collision component's OnComponentHit event
	CollisionComponent->OnComponentHit.AddDynamic(this, &AHealthPickUp::OnHit);

	// Set the sphere's collision radius
	CollisionComponent->InitSphereRadius(15.0f);

	// Set the root component to be the collision component
	RootComponent = CollisionComponent;

	// Initialize the static mesh component (optional)
	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Path/To/Your/StaticMesh"));
	if (Mesh.Succeeded())
	{
		PickupMeshComponent->SetStaticMesh(Mesh.Object);
	}
	PickupMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHealthPickUp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHealthPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Function that is called when the component hits something
void AHealthPickUp::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    // Check if the actor that hit this pickup is the player character
    if (OtherActor && OtherActor->IsA(AFPSCharacter::StaticClass()))
    {
        AFPSCharacter* HitPlayer = Cast<AFPSCharacter>(OtherActor);
        if (HitPlayer)
        {
            // Increment player's health if it's 2 or below
            if (HitPlayer->GetHealth() < 3)
            {
                int32 NewHealth = HitPlayer->GetHealth() + 1;
                HitPlayer->SetHealth(NewHealth);

                // Log a message for debugging
                UE_LOG(LogTemp, Warning, TEXT("Health PickUp hit the player!"));
                UE_LOG(LogTemp, Warning, TEXT("Player's health after pickup: %d"), NewHealth);

                // Update the HUD with the new health value
                APlayerController* PlayerController = Cast<APlayerController>(HitPlayer->GetController());
                if (PlayerController)
                {
                    AFPSHUD* HUD = Cast<AFPSHUD>(PlayerController->GetHUD());
                    if (HUD)
                    {
                        HUD->UpdateHealth(NewHealth);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Failed to get HUD instance."));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to get PlayerController instance."));
                }

                // Destroy the HealthPickUp actor
                Destroy();
            }
        }
    }
}
