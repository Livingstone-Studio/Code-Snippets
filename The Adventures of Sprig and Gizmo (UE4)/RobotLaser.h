// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerActivatedActor.h"
#include "RobotLaser.generated.h"

UCLASS()
class BETTERGAMEPROJECT_API ARobotLaser : public APowerActivatedActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobotLaser();

	void Powerup() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAI", meta = (AllowPrivateAccess = "true"))
		AActor* target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAI", meta = (AllowPrivateAccess = "true"))
		AActor* target_vine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAI", meta = (AllowPrivateAccess = "true"))
		float damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAI", meta = (AllowPrivateAccess = "true"))
		float distance = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAI", meta = (AllowPrivateAccess = "true"))
		float line_width = 40;

	UPROPERTY()
	bool bEnabled = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
