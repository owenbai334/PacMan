// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "PacDot.h"
#include "Kismet/GameplayStatics.h"
#include "PacManGameModeBase.h"
#include "Engine/Public/TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	StartLocation = GetActorLocation();
	Lifes = 3;
	NormalSpeed = 600.0f;
	InvincibleTime = 3.0f;
	GameModeRef = Cast<APacManGameModeBase>(UGameplayStatics::GetGameMode(this));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnPacmanBeginOverlay);
	SetMovement(true);
	
}

void AMyPlayer::Injured()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Invincible"));
	Lifes--;
	UE_LOG(LogTemp, Warning, TEXT("%d"), Lifes);
	if (Lifes == 0)
	{
		GameModeRef->SetCurrentState(EGameState::ELose);
	}
	else
	{
		SetActorLocation(StartLocation);
		GetWorldTimerManager().SetTimer(ResettingTimerHandle, this, &AMyPlayer::Resetting, InvincibleTime, false);
	}
}

void AMyPlayer::Resetting()
{
	GetWorldTimerManager().ClearTimer(ResettingTimerHandle);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}

void AMyPlayer::SetMovement(bool bCanMove)
{
	if (bCanMove)
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void AMyPlayer::StartGame()
{
	GameModeRef->StartGame();
}

void AMyPlayer::PauseGame()
{
	GameModeRef->PauseGame();
}

void AMyPlayer::RestartGame()
{
	GameModeRef->RestartGame();
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

	PlayerInputComponent->BindAction("StartGame", IE_Pressed, this, &AMyPlayer::StartGame);
	PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &AMyPlayer::PauseGame);
	PlayerInputComponent->BindAction("RestartGame", IE_Pressed, this, &AMyPlayer::RestartGame);

	PlayerInputComponent->BindAxis("MoveX", this, &AMyPlayer::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &AMyPlayer::MoveY);

}

void AMyPlayer::MoveX(float value)
{
	if (GameModeRef->GetCurrentState() == EGameState::EPlay)
	{
		Velocity.X = value;
		Velocity.Y = 0;

		if (value > 0)
		{
			SetActorRotation(FRotator(0, 0, 0));
		}
		else if (value < 0)
		{
			SetActorRotation(FRotator(0, -180, 0));
		}

		AddMovementInput(Velocity);
	}
}

void AMyPlayer::MoveY(float value)
{
	if (GameModeRef->GetCurrentState() == EGameState::EPlay)
	{
		Velocity.X = 0;
		Velocity.Y = value;

		if (value > 0)
		{
			SetActorRotation(FRotator(0, 90, 0));
		}
		else if (value < 0)
		{
			SetActorRotation(FRotator(0, -90, 0));
		}
		AddMovementInput(Velocity);
	}	
}

void AMyPlayer::OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GameModeRef->GetCurrentState() == EGameState::EPlay)
	{
		if (OtherActor->IsA(APacDot::StaticClass()))
		{
			APacDot* CurrentDot = Cast<APacDot>(OtherActor);
			if (CurrentDot->BIsSuperPacDot)
			{
				GameModeRef->SetEnemiesVulnerable();
			}
			OtherActor->Destroy();
			GameModeRef->SetPacdotNum(GameModeRef->GetPacdotNum() - 1);
		}
	}
}

