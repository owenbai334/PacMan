// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Enemy.h"
#include "NavigationSystem.h"

void AEnemyController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	ControllerEnemy = Cast<AEnemy>(PawnToPossess);
}

void AEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	GetNewDestination();
}

void AEnemyController::GetNewDestination()
{
	UNavigationSystemV1* NavMesh = UNavigationSystemV1::GetCurrent(this);
	if (NavMesh)
	{
		FNavLocation RandomLocation;
		const bool bNewDestinationFond =  NavMesh->GetRandomReachablePointInRadius(ControllerEnemy->GetActorLocation(), 10000, RandomLocation);
		if (bNewDestinationFond)
		{
			MoveToLocation(RandomLocation.Location,50.0f);
		}
	}
}
