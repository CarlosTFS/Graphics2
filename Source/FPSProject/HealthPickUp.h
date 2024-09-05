#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "HealthPickUp.generated.h"

UCLASS()
class FPSPROJECT_API AHealthPickUp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHealthPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sphere collision component
	UPROPERTY(VisibleAnywhere, Category = Pickup)
	USphereComponent* CollisionComponent;

	// Static mesh component for visual representation (optional)
	UPROPERTY(VisibleAnywhere, Category = Pickup)
	UStaticMeshComponent* PickupMeshComponent;

	// Function that is called when the component hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
