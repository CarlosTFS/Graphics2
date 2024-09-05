#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "SlowZone.generated.h"

UCLASS()
class FPSPROJECT_API ASlowZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlowZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SlowZoneSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow Effect")
	float SlowMultiplier;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	FTimerHandle SpeedResetTimerHandle;

	void ResetSpeedAfterDelay(ACharacter* Character);

};
