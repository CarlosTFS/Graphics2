// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSProjectile.h"

#include "FPSCharacter.generated.h"

UCLASS()
class FPSPROJECT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles input for moving forward and backward.
	UFUNCTION()
	void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
	void MoveRight(float Value);

	// Sets jump flag when key is pressed.
	UFUNCTION()
	void StartJump();

	// Clears jump flag when key is released.
	UFUNCTION()
	void StopJump();

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	// First-person mesh (arms), visible only to the owning player.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FPSMesh;

	// Function that handles firing projectiles.
	UFUNCTION()
	void Fire();

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// Health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	int32 Health; // This will be used to track the player's health.

	// Called when the player takes damage
	void TakeDamage(int32 DamageAmount);

	// Defeat widget
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DefeatWidgetFinalClass;

	// Victory widget
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> VictoryWidgetFinalClass;

	// Turret goal and current count
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turrets")
	int32 TurretGoal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turrets")
	int32 TurretCurrent;

	// Handle Victory
	void CheckVictory();

	// Gets the player's health
	int32 GetHealth() const;

	// Sets the player's health
	void SetHealth(int32 NewHealth);

	// Add these functions to your AFPSCharacter class
	UFUNCTION()
	void OnVictoryRestartButtonClicked();

	UFUNCTION()
	void OnDefeatRestartButtonClicked();

};
