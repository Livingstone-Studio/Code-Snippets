// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PowerActivatedActor.h"
#include "CameraCutsceneController.h"
#include "BatteryFanActivation.generated.h"

UCLASS()
class BETTERGAMEPROJECT_API ABatteryFanActivation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABatteryFanActivation();

	UFUNCTION()
	void Overlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAI", meta = (AllowPrivateAccess = "true"))
		AActor* battery;

	UPROPERTY()
		UBoxComponent* box;

	UPROPERTY()
	bool bActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Completion Settings", meta = (AllowPrivateAccess = "true"))
	ACameraCutsceneController* cutscene_controller;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Completion Settings", meta = (AllowPrivateAccess = "true"))
	int cutscene_info_index = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Completion Settings")
	TArray<APowerActivatedActor*> fans;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
