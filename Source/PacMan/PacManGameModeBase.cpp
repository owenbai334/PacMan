// Copyright Epic Games, Inc. All Rights Reserved.


#include "PacManGameModeBase.h"
#include "PacDot.h"
#include "Enemy.h"
#include "MyPlayer.h"
#include "Engine/Public/EngineUtils.h"
#include "EnemyController.h"

APacManGameModeBase::APacManGameModeBase()
{
	CurrentState = EGameState::EMenu;
}

void APacManGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<APacDot> PacdotItr(GetWorld()); PacdotItr; ++PacdotItr)
	{
		PacDotNum++;
	}

	for (TActorIterator<AEnemy> EnemyItr(GetWorld()); EnemyItr; ++EnemyItr)
	{
		Enemies.Add(Cast<AEnemy>(*EnemyItr));
	}
}

void APacManGameModeBase::SetCurrentState(EGameState value)
{
	CurrentState = value;

	switch (CurrentState)
	{
	case EGameState::EPlay:
		for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
		{
			(*Iter)->SetMovement(true);
		}
		Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(true);
		break;
	case EGameState::EWin:
		for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
		{
			(*Iter)->Destroy();
		}
		break;
	case EGameState::EPause:
	case EGameState::ELose:
		for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
		{
			(*Iter)->SetMovement(false);
			GetWorldTimerManager().ClearAllTimersForObject(*Iter);
		}
		Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(false);
		break;
	default:
		break;
	}
}

void APacManGameModeBase::SetPacdotNum(int value)
{
	PacDotNum = value;

	if (PacDotNum == 0)
	{
		SetCurrentState(EGameState::EWin);
	}
}

void APacManGameModeBase::StartGame()
{
	if (CurrentState == EGameState::EMenu)
	{
		SetCurrentState(EGameState::EPlay);

		for (auto Iter(Enemies.CreateIterator()); Iter;++Iter)
		{
			Cast<AEnemyController>((*Iter)->GetController())->GetNewDestination();
		}
	}
}

void APacManGameModeBase::PauseGame()
{
	if (GetCurrentState() == EGameState::EPlay)
	{
		RecordEnemiesTimer();
		SetCurrentState(EGameState::EPause);
	}
	else if (GetCurrentState() == EGameState::EPause)
	{
		RestoreEnemiesTimer();
		SetCurrentState(EGameState::EPlay);
	}
}

void APacManGameModeBase::RestartGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("RestartLevel"));
}

void APacManGameModeBase::EscapeGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("Exit"));
}

void APacManGameModeBase::SetEnemiesVulnerable()
{
	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
	{
		(*Iter)->SetVulnerable();
	}
}

void APacManGameModeBase::RecordEnemiesTimer()
{
	EnemiesTimerRemaining.Empty();

	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
	{
		if ((*Iter)->bIsVulnerable == false && (*Iter)->bIsDead == false)
		{
			EnemiesTimerRemaining.Add(-1);
		}
		if ((*Iter)->bIsVulnerable == true && (*Iter)->bIsDead == false)
		{
			float VulnerableTimerRemaining = GetWorldTimerManager().GetTimerRemaining((*Iter)->VulnerableTimerHandle);
			EnemiesTimerRemaining.Add(VulnerableTimerRemaining);
		}
		if ((*Iter)->bIsDead == true)
		{
			float DeadTimerRemaining = GetWorldTimerManager().GetTimerRemaining((*Iter)->DeadTimerHandle);
			EnemiesTimerRemaining.Add(DeadTimerRemaining);
		}
	}
}

void APacManGameModeBase::RestoreEnemiesTimer()
{
	for (auto Iter(Enemies.CreateIterator()); Iter; ++Iter)
	{
		if ((*Iter)->bIsVulnerable == true && (*Iter)->bIsDead == false)
		{
			(*Iter)->SetVulnerable(true, EnemiesTimerRemaining[Iter.GetIndex()]);
		}
		if ((*Iter)->bIsDead == true)
		{
			(*Iter)->SetDead(true, EnemiesTimerRemaining[Iter.GetIndex()]);
		}
	}
}
