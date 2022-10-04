// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "PacDot.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Velocity = FVector(0, 0, 0);
	PacmanDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanDisplayMesh"));
	PacmanDisplayMesh->SetupAttachment(GetCapsuleComponent());

}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnPacmanBeginOverlay);
	
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &AMyPlayer::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &AMyPlayer::MoveY);

}

void AMyPlayer::MoveX(float value)
{
	Velocity.X = value;
	Velocity.Y = 0;

	if (value > 0)
	{
		SetActorRotation(FRotator(0,0,0));
	}
	else if (value < 0)
	{
		SetActorRotation(FRotator(0,-180,0));
	}

	AddMovementInput(Velocity);
}

void AMyPlayer::MoveY(float value)
{
	Velocity.X = 0;
	Velocity.Y = value;

	if (value > 0)
	{
		SetActorRotation(FRotator(0,90,0));
	}
	else if(value < 0)
	{
		SetActorRotation(FRotator(0,-90,0));
	}
	AddMovementInput(Velocity);
}

void AMyPlayer::OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APacDot::StaticClass()))
	{
		OtherActor->Destroy();
	}
}

