// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include <Kismet\KismetMathLibrary.h>

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (mesh)
		RootComponent = mesh;
}

void AMovingPlatform::Powerup()
{
	bEnabled = true;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	points.Empty();
	for (int i = 0; i < actor_points.Num(); ++i)
	{
		if (!actor_points[i])
			continue;
		points.Add(actor_points[i]->GetActorLocation());
		actor_points[i]->Destroy();
	}
	actor_points.Empty();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bEnabled || points.Num() == 0)
		return;

	FVector nextLocation = FMath::VInterpConstantTo(GetActorLocation(), points[current_point], DeltaTime, move_speed);
	SetActorLocation(nextLocation);

	if (FVector::Distance(GetActorLocation(), points[current_point]) < 5.0f)
	{
		current_point++;
		if (current_point >= points.Num())
			current_point = 0;
	}
}

