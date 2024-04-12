// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotLaser.h"
#include "Vine.h"
#include "Components/LineBatchComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <BetterGameProject\Character\BaseCharacter.h>


// Sets default values
ARobotLaser::ARobotLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ARobotLaser::Powerup()
{
	bEnabled = true;
}

// Called when the game starts or when spawned
void ARobotLaser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobotLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bEnabled || !target_vine)
		return;

	UWorld* world = GetWorld();
	if (!world)
		return;

	if (target)
	{
		distance = FVector::Distance(GetActorLocation(), target->GetActorLocation());
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation());
		SetActorRotation(rot);
	}

	FVector startPos = GetActorLocation() + GetActorForwardVector() * 200;
	FVector endPos = GetActorLocation() + GetActorForwardVector() * distance;

	FHitResult result;
	FCollisionQueryParams params;
	FCollisionResponseParams col_params;
	if (world->LineTraceSingleByChannel(result, startPos, endPos, ECollisionChannel::ECC_Pawn,  params, col_params))
	{
		AActor* hit_target = result.GetActor();

		world->LineBatcher->DrawLine(startPos, result.ImpactPoint,
			FLinearColor::Red, SDPG_World, line_width, 0.0f);
		
		if (!hit_target)
			return;

		AVine* vine = Cast<AVine>(hit_target);
		ABaseCharacter* character = Cast<ABaseCharacter>(hit_target);
		if (vine == target_vine)
		{
			vine->DamageVine(damage, result.ImpactPoint);
		}
		else if (character)
		{
			FDamageEvent dmgEvent;
			character->TakeDamage(damage, dmgEvent, nullptr, this);
		}
	}
	else 
	{
		world->LineBatcher->DrawLine(startPos, endPos,
			FLinearColor::Red, SDPG_World, line_width, 0.0f);
	}
}

