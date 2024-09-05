#include "SlowZone.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPSCharacter.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASlowZone::ASlowZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the SlowZoneSphere component
	SlowZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SlowZoneSphere"));
	RootComponent = SlowZoneSphere;
	SlowZoneSphere->InitSphereRadius(200.0f);  // Adjust radius as needed

	// Set collision responses
	SlowZoneSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SlowZoneSphere->SetCollisionResponseToAllChannels(ECR_Overlap);

	// Bind overlap events
	SlowZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &ASlowZone::OnOverlapBegin);
	SlowZoneSphere->OnComponentEndOverlap.AddDynamic(this, &ASlowZone::OnOverlapEnd);

	// Initialize slow multiplier
	SlowMultiplier = 0.25f;  // Reduces speed by 75%
}

// Called when the game starts or when spawned
void ASlowZone::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASlowZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Draw a wireframe sphere at the location of the SlowZoneSphere
	DrawDebugSphere(GetWorld(), SlowZoneSphere->GetComponentLocation(), SlowZoneSphere->GetScaledSphereRadius(), 32, FColor::Blue, false, -1, 0, 2);
}

void ASlowZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor);  // Replace with your player class
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed *= SlowMultiplier;  // Adjust as needed
	}
}

void ASlowZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor);  // Replace with your player class
	if (PlayerCharacter)
	{
		// Start a timer to reset the speed after a delay
		GetWorld()->GetTimerManager().SetTimer(SpeedResetTimerHandle, [this, PlayerCharacter]()
			{
				ResetSpeedAfterDelay(PlayerCharacter);
			}, 4.0f, false);  // 2-second delay
	}
}

void ASlowZone::ResetSpeedAfterDelay(ACharacter* Character)
{
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed /= SlowMultiplier;  // Revert to normal speed
	}
}
