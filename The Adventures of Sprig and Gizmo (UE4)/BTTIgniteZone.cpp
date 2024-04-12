// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTIgniteZone.h"

#include "ElectrifyZone.h"
#include "BrokenWire.h"
#include "BossPawn.h"
#include "BossController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "CameraCutsceneController.h"
#include <BetterGameProject\Managers\BGP_GameInstance.h>


UBTTIgniteZone::UBTTIgniteZone(FObjectInitializer const& object_initailizer)
{

}

EBTNodeResult::Type UBTTIgniteZone::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
    AAIController* controller = owner_comp.GetAIOwner();
    ABossController* cont = nullptr;
    if (controller)
        cont = Cast<ABossController>(controller);
    APawn* pawn = nullptr;
    if (cont)
        pawn = cont->GetPawn();
    ABossPawn* boss = nullptr;
    if (pawn)
        boss = Cast<ABossPawn>(pawn);

    if (!boss)
        return EBTNodeResult::Failed;


    ACameraCutsceneController* CutsceneController = boss->GetGameInstance<UBGP_GameInstance>()->GetCutsceneController();
    if (CutsceneController)
        if (CutsceneController->IsActive())
            return EBTNodeResult::Failed;

    // Get All Zones
    TArray<ABrokenWire*> zones = boss->GetBrokenWire();
    if (zones.Num() == 0)
        return EBTNodeResult::Failed;

    std::vector<APawn*> players;
    players.push_back(boss->GetGameInstance<UBGP_GameInstance>()->GetNatureCharacter());
    players.push_back(boss->GetGameInstance<UBGP_GameInstance>()->GetTechCharacter());
    if (players.size() == 0)
        return EBTNodeResult::Failed;

    int target_zone_index = FMath::RandRange(0, zones.Num() - 1);

    float target_player_roll = FMath::FRandRange(0.0f, 1.0f);
    if (target_player_roll < player_weighting)
    {
        // Targetting player
        int random_target = FMath::RandRange(0, players.size() - 1);
        float closestDistance = 0;
        float distance = 0;
        for (int i = 0; i < zones.Num(); ++i)
        {
            if (!zones[i])
                continue;
            FVector zone_target = zones[i]->GetActorLocation();
            if (zones[i]->GetEndPoweredActor())
                zone_target = zones[i]->GetEndPoweredActor()->GetActorLocation();
            distance = FVector::Distance(players[random_target]->GetActorLocation(), zone_target);
            if ((i == 0) || (distance < closestDistance && !(i == 0)))
            {
                closestDistance = distance;
                target_zone_index = i;
            }
        }
    }

    // Select one at random
    if (!zones[target_zone_index])
        return EBTNodeResult::Failed;

    // Highlight it
    AElectrifyZone* target_zone = Cast<AElectrifyZone>(zones[target_zone_index]->GetEndPoweredActor());

    if (!target_zone)
        return EBTNodeResult::Failed;

    target_zone->Highlight(boss->GetIgnitionIntensity());

    zones[target_zone_index]->SpawnElectricity(1000.0f);

    // return succeeded
    return EBTNodeResult::Succeeded;
}
