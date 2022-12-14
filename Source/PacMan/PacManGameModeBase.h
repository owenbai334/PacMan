// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacManGameModeBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGameState:uint8
{
	EMenu,
	EPlay,
	EPause,
	EWin,
	ELose
};

UCLASS()
class PACMAN_API APacManGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APacManGameModeBase();
	
	void StartGame();
	void PauseGame();
	void RestartGame();
	void EscapeGame();

	void SetEnemiesVulnerable();
	void RecordEnemiesTimer();
	void RestoreEnemiesTimer();

	int GetPacdotNum() const;
	void SetPacdotNum(int value);

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int PacDotNum;

	EGameState GetCurrentState() const;
	void SetCurrentState(EGameState value);

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	EGameState CurrentState;

protected:
	virtual void BeginPlay() override;

private:
	TArray<class AEnemy*> Enemies;
	TArray<float> EnemiesTimerRemaining;
	
};

FORCEINLINE int APacManGameModeBase::GetPacdotNum() const
{
	return PacDotNum;
}

FORCEINLINE EGameState APacManGameModeBase::GetCurrentState() const
{
	return CurrentState;
}