// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerActivatedActor.h"

// Sets default values
APowerActivatedActor::APowerActivatedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APowerActivatedActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerActivatedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

