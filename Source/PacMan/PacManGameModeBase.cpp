// Copyright Epic Games, Inc. All Rights Reserved.


#include "PacManGameModeBase.h"
#include "PacDot.h"
#include "Enemy.h"
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
	case EGameState::EMenu:
		break;
	case EGameState::EPlay:
		break;
	case EGameState::EPause:
		break;
	case EGameState::EWin:
		break;
	case EGameState::ELose:
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

}

void APacManGameModeBase::RestartGame()
{

}