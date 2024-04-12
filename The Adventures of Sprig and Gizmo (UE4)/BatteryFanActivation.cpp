// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryFanActivation.h"
#include "BetterGameProject/Managers/BGP_GameInstance.h"
#include "PowerActivatedActor.h"
#include <Kismet\GameplayStatics.h>

// Sets default values
ABatteryFanActivation::ABatteryFanActivation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABatteryFanActivation::Overlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != battery)
		return;
	
	OtherActor->SetActorLocation(GetActorLocation());
	OtherActor->SetActorRotation(GetActorRotation());
	OtherActor->GetRootComponent()->SetMobility(EComponentMobility::Stationary);

	for (APowerActivatedActor* fan : fans)
		if (fan)
			fan->Powerup();

	if (cutscene_controller)
		cutscene_controller->PlayCameraEvent(Pan, cutscene_info_index);
}

// Called when the game starts or when spawned
void ABatteryFanActivation::BeginPlay()
{
	Super::BeginPlay();

	for (UActorComponent* comp : GetComponents())
	{
		UBoxComponent* col = Cast<UBoxComponent>(comp);
		if (col)
			box = col;

	}

	if (box)
		box->OnComponentBeginOverlap.AddDynamic(this, &ABatteryFanActivation::Overlap);
}

// Called every frame
void ABatteryFanActivation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
