// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTIgniteZone.generated.h"

/**
 * 
 */
UCLASS()
class BETTERGAMEPROJECT_API UBTTIgniteZone : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTIgniteZone(FObjectInitializer const& object_initailizer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAI", meta = (AllowPrivateAccess = "true", ClampMin = 0.0f, ClampMax = 1.0f))
	float player_weighting;
};
