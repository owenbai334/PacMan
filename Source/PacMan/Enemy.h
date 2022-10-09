// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class PACMAN_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	bool bIsDead;
	bool bIsVulnerable;

	FTimerHandle DeadTimerHandle;
	FTimerHandle VulnerableTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VulnerableTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* EnemyDisplayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VulnerableSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void OnEnemyBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetVulnerable(bool bRestartTimer = false,float Timer = 0.0f);
	void SetDead(bool bRestartTimer = false, float Timer = 0.0f);
	void SetNormal();
	void SetMovement(bool bCanMove);

private:

	FVector StartLocation;
};
