// Fill out your copyright notice in the Description page of Project Settings.


#include "PacDot.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
APacDot::APacDot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PacdotCollection = CreateDefaultSubobject<USphereComponent>(TEXT("PacdotCollection"));
	PacdotDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacdotDisplayMesh"));

	PacdotDisplayMesh->SetupAttachment(PacdotCollection);
}

// Called when the game starts or when spawned
void APacDot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APacDot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

