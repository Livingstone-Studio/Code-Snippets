// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerActivatedActor.generated.h"

UCLASS()
class BETTERGAMEPROJECT_API APowerActivatedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerActivatedActor();

	UFUNCTION(BlueprintCallable)
	virtual void Powerup() {};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
