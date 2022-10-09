// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyDisplayMesh"));
	EnemyDisplayMesh->SetupAttachment(GetCapsuleComponent());

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	NormalSpeed = 300.0f;
	VulnerableSpeed = 100.0f;

	bIsDead = false;
	bIsVulnerable = false;

	DeadTime = 5.0f;
	VulnerableTime = 3.0f;

	StartLocation = GetActorLocation();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnEnemyBeginOverlay);
	SetMovement(true);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnEnemyBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AMyPlayer::StaticClass()))
	{
		if (bIsDead)
		{
			return;
		}

		if (bIsVulnerable)
		{
			SetDead();
		}
		else
		{
			Cast<AMyPlayer>(OtherActor)->Injured();
		}
	}
}

void AEnemy::SetVulnerable()
{

}

void AEnemy::SetDead()
{

}

void AEnemy::SetNormal()
{

}

void AEnemy::SetMovement(bool bCanMove)
{
	if (bCanMove)
	{
		if (!bIsVulnerable && !bIsDead)
		{
			GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		}
		if (bIsVulnerable && !bIsDead)
		{
			GetCharacterMovement()->MaxWalkSpeed = VulnerableSpeed;
		}
		if (bIsDead)
		{
			GetCharacterMovement()->MaxWalkSpeed = 0;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

