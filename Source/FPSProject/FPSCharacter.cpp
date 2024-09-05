// Copyright Epic Games, Inc. All Rights Reserved.
#include "FPSCharacter.h"
#include "FPSHUD.h" // Include FPSHUD header
#include "Kismet/GameplayStatics.h" 

// Sets default values
AFPSCharacter::AFPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	// Only the owning player sees this mesh.
	FPSMesh->SetOnlyOwnerSee(true);

	// Attach the FPS mesh to the FPS camera.
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// Disable some environmental shadows to preserve the illusion of having a single mesh.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// The owning player doesn't see the regular (third-person) body mesh.
	GetMesh()->SetOwnerNoSee(true);

	// Initialize health
	Health = 3; // Set initial health (number of icons)
	TurretGoal = 2; // Set turret goal
	TurretCurrent = 0; // Initialize turret count
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);
	// Put up a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	// Set up "look" bindings.
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}

void AFPSCharacter::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}


void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::Fire()

{

	// Attempt to fire a projectile.

	if (ProjectileClass)

	{

		// Get the camera transform.

		FVector CameraLocation;

		FRotator CameraRotation;

		GetActorEyesViewPoint(CameraLocation, CameraRotation);



		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.

		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);



		// Transform MuzzleOffset from camera space to world space.

		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);



		// Skew the aim to be slightly upwards.

		FRotator MuzzleRotation = CameraRotation;

		MuzzleRotation.Pitch += 10.0f;



		UWorld* World = GetWorld();

		if (World)

		{

			FActorSpawnParameters SpawnParams;

			SpawnParams.Owner = this;

			SpawnParams.Instigator = GetInstigator();



			// Spawn the projectile at the muzzle.

			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

			if (Projectile)

			{

				// Set the projectile's initial trajectory.

				FVector LaunchDirection = MuzzleRotation.Vector();

				Projectile->FireInDirection(LaunchDirection);

			}

		}

	}

}

void AFPSCharacter::TakeDamage(int32 DamageAmount)
{
	Health -= DamageAmount;
	Health = FMath::Max(Health, 0);

	UE_LOG(LogTemp, Warning, TEXT("Player health after taking damage: %d"), Health);

	AFPSHUD* HUD = Cast<AFPSHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (HUD)
	{
		HUD->UpdateHealth(Health);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HUD is null!"));
	}

	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is dead"));

		// Display the Defeat widget
		if (DefeatWidgetFinalClass)
		{
			UUserWidget* DefeatWidget = CreateWidget<UUserWidget>(GetWorld(), DefeatWidgetFinalClass);
			if (DefeatWidget)
			{
				DefeatWidget->AddToViewport();
				DefeatWidget->SetVisibility(ESlateVisibility::Visible);
				UE_LOG(LogTemp, Log, TEXT("DefeatWidget added to viewport and set to visible."));

				// Pause the game and show the cursor
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				if (PlayerController)
				{
					PlayerController->SetPause(true);
					PlayerController->bShowMouseCursor = true;
					UE_LOG(LogTemp, Log, TEXT("Game paused and mouse cursor shown."));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create DefeatWidget."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("DefeatWidgetFinalClass is null!"));
		}
	}
}


void AFPSCharacter::CheckVictory()
{
	if (TurretCurrent >= TurretGoal)
	{
		if (VictoryWidgetFinalClass)
		{
			UUserWidget* VictoryWidget = CreateWidget<UUserWidget>(GetWorld(), VictoryWidgetFinalClass);
			if (VictoryWidget)
			{
				VictoryWidget->AddToViewport();
				VictoryWidget->SetVisibility(ESlateVisibility::Visible);
				UE_LOG(LogTemp, Log, TEXT("VictoryWidget added to viewport and made visible."));

				// Pause the game and show the cursor
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				if (PlayerController)
				{
					PlayerController->SetPause(true);
					PlayerController->bShowMouseCursor = true;
					UE_LOG(LogTemp, Log, TEXT("Game paused and mouse cursor shown."));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create VictoryWidget."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("VictoryWidgetFinalClass is null!"));
		}
	}
}


int32 AFPSCharacter::GetHealth() const
{
	return Health;
}

void AFPSCharacter::SetHealth(int32 NewHealth)
{
	Health = NewHealth;
}

void AFPSCharacter::OnVictoryRestartButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Victory Restart button clicked. Restarting the game."));

	// Restart the game
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName())); // Restart the current level
}

void AFPSCharacter::OnDefeatRestartButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Defeat Restart button clicked. Restarting the game."));

	// Restart the game
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName())); // Restart the current level
}
