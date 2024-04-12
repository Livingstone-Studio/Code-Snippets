// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraCutsceneController.h"
#include <BetterGameProject\Managers\BGP_GameInstance.h>
#include <Kismet\GameplayStatics.h>
#include <Blueprint\WidgetBlueprintLibrary.h>

// Sets default values
ACameraCutsceneController::ACameraCutsceneController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraCutsceneController::BeginPlay()
{
	Super::BeginPlay();
		
}

// Called every frame
void ACameraCutsceneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// <summary>
/// Plays either a sequence or pan defined in the cutscene controller using the index position of it in it's respective list.
/// </summary>
/// <param name="type">Whether it is a sequencer sequence or a camera pan.</param>
/// <param name="index">The index of it in it's respective cutscene controller list (counting from 0).</param>
void ACameraCutsceneController::PlayCameraEvent(TEnumAsByte<ECameraEventType> type, int index)
{
	switch (type)
	{
	case Sequence:
		if (index >= 0 && index < sequencer_info.Num())
		{
			PlaySequence(index);
		}
			break;
	case Pan:
		if (index >= 0 && index < cam_pan_info.Num())
			PlayCameraPan(index);
		break;
	}
}

/// <summary>
/// Attempts to set up the game to run a sequencer cutscene, not to be called.
/// </summary>
/// <param name="index">The index of the sequence to be played.</param>
void ACameraCutsceneController::PlaySequence(int index, bool bAffectsMovement)
{
	current_sequence = &sequencer_info[index];
	if (!current_sequence)
		return;
	if (!current_sequence->sequence || bActive)
		return;

	OnSequencerStart();

	bActive = true;
	sequence_actor = nullptr;
	sequence_player = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), current_sequence->sequence,
		FMovieSceneSequencePlaybackSettings(), sequence_actor);

	if (current_sequence->bCinematic)
	{
		APlayerController* firstController = GetGameInstance<UBGP_GameInstance>()->GetFirstController();
		APlayerController* secondController = GetGameInstance<UBGP_GameInstance>()->GetSecondController();

		if (firstController && secondController && bAffectsMovement) {
			firstController->SetCinematicMode(true, true, true);
			secondController->SetCinematicMode(true, true, true);

		}

		UGameplayStatics::SetForceDisableSplitscreen(GetWorld(), true);
		ToggleSequencerMode(*current_sequence, true);
	}

	FQualifiedFrameTime CutsceneLength;
	if (sequence_player)
	{
		CutsceneLength = sequence_player->GetDuration();
		sequence_player->Play();

		FTimerHandle handle;
		FTimerDelegate del;
		del.BindUFunction(this, FName("EndSequence"));
		GetWorld()->GetTimerManager().SetTimer(handle, del, current_sequence->sequence_length, false);
	}

}

/// <summary>
/// Cleans up any changes from the sequencer cutscene.
/// </summary>
void ACameraCutsceneController::EndSequence()
{
	if (!current_sequence || !bActive || !sequence_actor)
		return;

	UGameplayStatics::SetForceDisableSplitscreen(GetWorld(), false);

	ToggleSequencerMode(*current_sequence, false);

	APlayerController* firstController = GetGameInstance<UBGP_GameInstance>()->GetFirstController();
	APlayerController* secondController = GetGameInstance<UBGP_GameInstance>()->GetSecondController();

	if (firstController && secondController) {
		firstController->SetCinematicMode(false, true, true);
		secondController->SetCinematicMode(false, true, true);
	}

	if (sequence_actor)
		sequence_actor->Destroy();

	if (current_sequence->load_level_name.Len() > 0)
		UGameplayStatics::OpenLevel(this, FName(*current_sequence->load_level_name));

	current_sequence = nullptr;
	bActive = false;
	OnSequencerEnd();

}

/// <summary>
/// Attempts to set up the game to run a camera pan, not to be called.
/// </summary>
/// <param name="index">The index of the pan to be played.</param>
void ACameraCutsceneController::PlayCameraPan(int index)
{
	current_pan = &cam_pan_info[index];
	if (!current_pan)
		return;

	if (!current_pan->pan_cam || bActive)
		return;
	bActive = true;
	UBGP_GameInstance* instance = GetGameInstance<UBGP_GameInstance>();
	if (!instance)
		return;
	APlayerController* playerOne = instance->GetFirstController();
	APlayerController* playerTwo = instance->GetSecondController();
	if (!playerOne || !playerTwo)
		return;

	OnPanStart();

	camera_store_one = playerOne->GetViewTarget();
	camera_store_two = playerTwo->GetViewTarget();

	playerOne->SetViewTargetWithBlend((AActor*)current_pan->pan_cam, current_pan->cam_blend_time);
	playerTwo->SetViewTargetWithBlend((AActor*)current_pan->pan_cam, current_pan->cam_blend_time);

	playerOne->SetCinematicMode(true, true, true);
	playerTwo->SetCinematicMode(true, true, true);

	if (current_pan->cam_blend_time == 0.0f)
		StartPanLinger();
	else
	{
		FTimerDelegate del;
		FTimerHandle handle;
		del.BindUFunction(this, FName("StartPanLinger"));
		GetWorld()->GetTimerManager().SetTimer(handle, del, current_pan->cam_blend_time, false);
	}

	TogglePanMode(*current_pan, false);
	bool bState = false;
	FTimerDelegate del;
	FTimerHandle handle;
	del.BindUFunction(this, FName("EndCameraPan"), bState);
	GetWorld()->GetTimerManager().SetTimer(handle, del, current_pan->linger_time, false);
}

/// <summary>
/// Cleans up any changes from the pan.
/// </summary>
void ACameraCutsceneController::EndCameraPan(bool bImmediate)
{
	if (!current_pan || !bActive)
		return;
	UBGP_GameInstance* instance = GetGameInstance<UBGP_GameInstance>();
	if (!instance)
		return;

	APlayerController* playerOne = instance->GetFirstController();
	APlayerController* playerTwo = instance->GetSecondController();
	if (!playerOne || !playerTwo)
		return;

	bPanLingering = false;

	if (current_pan->camera_shake)
	{
		playerOne->ClientStopCameraShake(current_pan->camera_shake);
		playerTwo->ClientStopCameraShake(current_pan->camera_shake);
	}

	playerOne->SetViewTargetWithBlend(camera_store_one, current_pan->cam_blend_time);
	playerTwo->SetViewTargetWithBlend(camera_store_two, current_pan->cam_blend_time);

	if (current_pan->widget)
	{
		current_pan->widget->RemoveFromParent();
	}

	OnPanEnd();
	if (current_pan->load_level_name.Len() > 0 || bImmediate || current_pan->cam_blend_time == 0.0f)
	{
		TogglePanMode(*current_pan, true);

		if (current_pan->load_level_name.Len() > 0)
			UGameplayStatics::OpenLevel(this, FName(*current_pan->load_level_name));

		current_pan = nullptr;
		bActive = false;
		return;
	}

	FTimerDelegate del;
	bool bState = true;
	del.BindUFunction(this, FName("TogglePanMode"), *current_pan, bState);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, del, current_pan->cam_blend_time, false);

	current_pan = nullptr;
	bActive = false;
}

void ACameraCutsceneController::ToggleSequencerMode(const FSequencerInfo& sequence, bool state)
{
	UBGP_GameInstance* instance = GetGameInstance<UBGP_GameInstance>();
	if (!instance)
		return;

	APlayerController* playerOne = instance->GetFirstController();
	APlayerController* playerTwo = instance->GetSecondController();
	if (!playerOne || !playerTwo)
		return;

	ESlateVisibility widget_state = state ? ESlateVisibility::Hidden : ESlateVisibility::Visible;

	TArray<UUserWidget*> ActiveWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(playerOne, ActiveWidgets, UUserWidget::StaticClass(), true);
	for (UUserWidget* Widget : ActiveWidgets)
	{
		Widget->SetVisibility(widget_state);
	}

}

/// <summary>
/// Runs any changes needed to occur at the moment the camera reaches its pan destination, not to be called.
/// </summary>
void ACameraCutsceneController::StartPanLinger()
{
	UWorld* world = GetWorld();
	if (!world || !current_pan)
		return;
	UBGP_GameInstance* instance = GetGameInstance<UBGP_GameInstance>();
	if (!instance)
		return;
	APlayerController* playerOne = instance->GetFirstController();
	APlayerController* playerTwo = instance->GetSecondController();
	if (!playerOne || !playerTwo)
		return;


	OnPanLinger();

	bPanLingering = true;

	if (current_pan->widget)
	{
		current_pan->widget->SetVisibility(ESlateVisibility::Visible);
		current_pan->widget->SetOwningPlayer(playerOne);
		current_pan->widget->AddToPlayerScreen();
	}


	if (current_pan->camera_shake)
	{
		playerOne->ClientStartCameraShake(current_pan->camera_shake);
		playerTwo->ClientStartCameraShake(current_pan->camera_shake);
	}

	for (APowerActivatedActor* power_actor : current_pan->cutscene_activated_actors)
		if (power_actor)
			power_actor->Powerup();
}

/// <summary>
/// Sets the game for a pan or ending a pan depending on state value, not to be called.
/// </summary>
/// <param name="state">Determines if game is entering/leaving the pan state. True = Entering, False = Leaving </param>
void ACameraCutsceneController::TogglePanMode(const FCameraPanInfo& pan, bool state)
{
	UWorld* world = GetWorld();
	if (!world)
		return;
	UBGP_GameInstance* instance = GetGameInstance<UBGP_GameInstance>();
	if (!instance)
		return;

	APlayerController* playerOne = instance->GetFirstController();
	APlayerController* playerTwo = instance->GetSecondController();
	if (!playerOne || !playerTwo)
		return;

	playerOne->SetCinematicMode(!state, true, true);
	playerTwo->SetCinematicMode(!state, true, true);

	UGameplayStatics::SetForceDisableSplitscreen(world, !state);
	
	ESlateVisibility widget_state = !state ? ESlateVisibility::Hidden : ESlateVisibility::Visible;

	TArray<UUserWidget*> ActiveWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(playerOne, ActiveWidgets, UUserWidget::StaticClass(), true);
	for (UUserWidget* Widget : ActiveWidgets)
	{
		if (!Widget)
			continue;
		if (pan.widget)
			if (Widget->GetFName() != pan.widget->GetFName())
				Widget->SetVisibility(widget_state);
		else
			Widget->SetVisibility(widget_state);
	}	

	bActive = !state;
}