// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"
#include "FPSCharacter.h"  // Include the header file for AFPSCharacter
#include "EnemyTower.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.

		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

		// Set the sphere's collision profile name to "Projectile".

		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

		// Event called when component hits something.

		CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);

		// Set the sphere's collision radius.

		CollisionComponent->InitSphereRadius(15.0f);

		// Set the root component to be the collision component.

		RootComponent = CollisionComponent;


	}

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.

		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

		ProjectileMovementComponent->InitialSpeed = 3000.0f;

		ProjectileMovementComponent->MaxSpeed = 3000.0f;

		ProjectileMovementComponent->bRotationFollowsVelocity = true;

		ProjectileMovementComponent->bShouldBounce = true;

		ProjectileMovementComponent->Bounciness = 0.3f;

		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!ProjectileMeshComponent)

	{

		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Sphere.Sphere'"));

		if (Mesh.Succeeded())

		{

			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);

		}

	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Script/Engine.Material'/Game/SphereMaterial.SphereMaterial'"));

	if (Material.Succeeded())

	{

		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);

	}

	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	// Delete the projectile after 3 seconds.
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function that initializes the projectile's velocity in the shoot direction.

void AFPSProjectile::FireInDirection(const FVector& ShootDirection)

{

	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;

}

// Function that is called when the projectile hits something.

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}

	if (OtherActor != this && OtherActor->IsA(AFPSCharacter::StaticClass()))
	{
		AFPSCharacter* HitPlayer = Cast<AFPSCharacter>(OtherActor);
		if (HitPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bullet hit the player!"));

			// Adjust this value based on your game's needs.
			int32 DamageAmount = 1;

			HitPlayer->TakeDamage(DamageAmount);
		}
	}

	if (OtherActor != this && OtherActor->IsA(AEnemyTower::StaticClass()))
	{
		AEnemyTower* HitTower = Cast<AEnemyTower>(OtherActor);
		if (HitTower)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bullet hit the enemy tower!"));

			// Reduce tower life by 1
			HitTower->SetTowerLife(HitTower->GetTowerLife() - 1);

			// Check if tower life is zero or below, and destroy the tower if so
			if (HitTower->GetTowerLife() <= 0)
			{
				
                // Update TurretCurrent
                AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
                if (PlayerCharacter)
                {
                    PlayerCharacter->TurretCurrent++;
					// Check if victory conditions are met
					PlayerCharacter->CheckVictory();
                }
				HitTower->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("Enemy tower destroyed!"));
			}
		}
	}

	Destroy();
}


