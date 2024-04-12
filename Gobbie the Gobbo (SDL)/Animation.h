#pragma once

#include "AnimClip.h"

#include <vector>

class Camera;

class Animation
{
public:

	Animation();
	~Animation();

	void InitAnim(SDL_Renderer* renderer, string imgFile, AnimationInfo anim);

	void ChangeAnimation(SDL_Renderer* renderer, AnimationInfo animInfo);

	void Start();

	void Update();

	void RenderUpdate(SDL_Renderer* renderer, Transform transform);

	bool IsCompleted() { return completed; }

	bool OneIsTriggered() { return animClips[0].Triggered(); }
	bool OneIsAlreadyTriggered() { return animClips[0].TriggeredEventPassed(); }

	bool TwoIsTriggered() { return animClips[0].Triggered2(); }
	bool TwoIsAlreadyTriggered() { return animClips[0].TriggeredEventPassed2(); }

	int GetRow() { return animClips[0].GetRow(); }

	Vector2 GetOffset();

	string GetAnimName() { return m_anim_name; }

	void UpdateSRC(Transform t, Transform y);

	void SetCustomScaleAnimTwo(Vector2 newScale);


private:

	string m_sprite_name__{ "" };

	string m_anim_name{ "" };

	Vector2 m_offset{ 0,0 };

	AnimClip animClips[2];

	bool completed{ false };

};