// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerActivatedActor.h"
#include "Camera/CameraShakeBase.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "CameraCutsceneController.generated.h"

UENUM(BlueprintType)
enum ECameraEventType
{
	Sequence,
	Pan,
};

USTRUCT(BlueprintType)
struct BETTERGAMEPROJECT_API FSequencerInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cutscene Type",
		meta = (ToolTip = "For a sequencer type cutscene, set this value and leave pan cam empty"))
	ULevelSequence* sequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequencer | Skipping",
		meta = (ToolTip = "If the players can skip the cutscene or not."))
	bool bSkippable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequencer | Cinematic",
		meta = (ToolTip = "Set to false if the sequence does NOT use a camera."))
	bool bCinematic = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequencer | Levels",
		meta = (ToolTip = "A quick setting saying if after the cutscene the game should load a new scene."))
	FString load_level_name = ""; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequencer | Levels",
        meta = (ToolTip = "A sequence length."))
    float sequence_length = 1.0f;
};

USTRUCT(BlueprintType)
struct BETTERGAMEPROJECT_API FCameraPanInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cutscene Type",
		meta = (ToolTip = "For a pan type cutscene, set this value and leave sequencer empty"))
	ACameraActor* pan_cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pan | Power Activated Actors",
		meta = (ToolTip = "A list of 'PowerActivatedActor's that get activated at the start of the cutscene"))
	TArray<APowerActivatedActor*> cutscene_activated_actors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pan | Shake",
		meta = (ToolTip = "Camera shake that runs during the cutscene"))
	TSubclassOf<UCameraShakeBase> camera_shake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pan | UI",
		meta = (ToolTip = "A widget to be displayed during the cutscene"))
	UUserWidget* widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pan | Timing",
		meta = (ToolTip = "The time it takes the camera to swap to/back from the pan cam"))
	float cam_blend_time = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pan | Timing",
		meta = (ToolTip = "The amount of time the camera stays at the pan cam"))
	float linger_time = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pan | Skipping",
		meta = (ToolTip = "If the players can skip the cutscene or not."))
	bool bSkippable = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pan | Levels",
		meta = (ToolTip = "A quick setting saying if after the cutscene the game should load a new scene."))
	FString load_level_name = "";
};

UCLASS(Blueprintable)
class BETTERGAMEPROJECT_API ACameraCutsceneController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraCutsceneController();

protected:
#pragma region UE4 Class Method Overrides
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

public:
#pragma region Blueprint Ready Cutscene Functions
	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Plays either a sequence or pan defined in the cutscene controller using the index position of it in it's respective list."))
	void PlayCameraEvent(TEnumAsByte<ECameraEventType> type, int index);
	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Returns if the cutscene controller is currently active."))
	bool IsActive() { return bActive; }
#pragma endregion

#pragma region Sequencer Event Declarations
	UFUNCTION(BlueprintImplementableEvent, meta = (Tooltip = "Called when a sequencer cutscene starts"))
	void OnSequencerStart();
	UFUNCTION(BlueprintImplementableEvent, meta = (Tooltip = "Called when a sequencer cutscene ends"))
	void OnSequencerEnd();
	UFUNCTION(BlueprintImplementableEvent, meta = (Tooltip = "Called when a sequencer cutscene is skipped"))
	void OnSequencerSkip();
#pragma endregion

#pragma region Pan Event Declarations
	UFUNCTION(BlueprintImplementableEvent, meta = (Tooltip = "Called when a camera pan cutscene starts"))
	void OnPanStart();
	UFUNCTION(BlueprintImplementableEvent, meta = (Tooltip = "Called when a camera pan cutscene ends"))
	void OnPanEnd();
	UFUNCTION(BlueprintImplementableEvent, meta = (Tooltip = "Called when a camera pan cutscene reaches its destination camera"))
	void OnPanLinger();
	UFUNCTION(BlueprintImplementableEvent, meta = (Tooltip = "Called when a camera pan cutscene is skipped"))
	void OnPanSkip();
#pragma endregion

#pragma region Internal Cutscene Functions
	UFUNCTION()
	void PlaySequence(int index, bool bAffectsMovement = true);
	UFUNCTION()
	void EndSequence();
	UFUNCTION()
	void PlayCameraPan(int index);
	UFUNCTION()
	void EndCameraPan(bool bImmediate = false);

	ULevelSequencePlayer* GetSequencePlayer() { return sequence_player; }
	FSequencerInfo* GetCurrentSequence() { return current_sequence; }
	FCameraPanInfo* GetCurrentPan() { return current_pan; }
	bool PanIsLingering() { return bPanLingering; }

private:
	UFUNCTION()
	void ToggleSequencerMode(const FSequencerInfo& sequence, bool state);

	UFUNCTION()
	void StartPanLinger();
	UFUNCTION()
	void TogglePanMode(const FCameraPanInfo& pan, bool state);
#pragma endregion

protected:
#pragma region Exposed Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cutscene Settings", meta = (AllowPrivateAccess = "true",
		Tooltip = "Information of any sequencer cutscenes for the loaded level."))
	TArray<FSequencerInfo> sequencer_info;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cutscene Settings", meta=(AllowPrivateAccess = "true",
		Tooltip = "Information of any camera pans for the loaded level."))
	TArray<FCameraPanInfo> cam_pan_info;
	UPROPERTY(VisibleAnywhere, Category = "Debug", meta = (AllowPrivateAccess = "true",
		Tooltip = "If there is a currently active cutscene or not. Use 'IsActive'."))
	bool bActive = false;
	UPROPERTY(VisibleAnywhere, Category = "Debug", meta = (AllowPrivateAccess = "true",
		Tooltip = "If the camera has panned and is currently lingering in it's destination."))
	bool bPanLingering = false;
#pragma endregion

#pragma region Internal Properties
	UPROPERTY()
	ULevelSequencePlayer* sequence_player;
	UPROPERTY()
	ALevelSequenceActor* sequence_actor;
	UPROPERTY()
	AActor* camera_store_one;
	UPROPERTY()
	AActor* camera_store_two;

	FSequencerInfo* current_sequence;
	FCameraPanInfo* current_pan;
#pragma endregion
};
