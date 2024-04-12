// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerActivatedActor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class BETTERGAMEPROJECT_API AMovingPlatform : public APowerActivatedActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

	void Powerup() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovingPlatform", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingPlatform", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> actor_points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovingPlatform", meta = (AllowPrivateAccess = "true"))
	float move_speed = 125.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovingPlatform", meta = (AllowPrivateAccess = "true"))
	TArray<FVector> points;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovingPlatform", meta = (AllowPrivateAccess = "true"))
	int current_point = 0;

	UPROPERTY()
	bool bEnabled = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
