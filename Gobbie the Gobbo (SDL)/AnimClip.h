#pragma once

#include "Sprite.h"

class AnimClip
{

public:

	AnimClip();

	void Initialize(SDL_Renderer* renderer, string imgFile, AnimClipInfo anim);
		
	void SetAnimation(AnimClipInfo anim);

	void DebugCurrentSpriteInfo();

	SDL_Texture* GetSprite();

	void SetAnimationSprite(int  row, int column);

	SDL_Rect GetSrcRect() { return m_spritesheet.GetSrcRect(); }
	SDL_Rect GetDesRect() { return m_spritesheet.GetDesRect(); }

	void SetDesRect(float x, float y, float xScale = 1, float yScale = 1);
	void SetDesRect(Transform transform);

	void SetSrcRect(Transform transform);

	bool Update();

	void RenderUpdate(SDL_Renderer* renderer, Transform transform);

	bool IsInitialized() { return m_initialized; }

	bool Triggered() { return m_event_triggered; }
	bool TriggeredEventPassed() { return m_event_triggered_already; }

	bool Triggered2() { return m_event_2_triggered; }
	bool TriggeredEventPassed2() { return m_event_2_triggered_already; }

	int GetEventFrameOne() { return m_event_frame[0]; }
	int GetEventFrameTwo() { return m_event_frame[1]; }

	int GetRow() { return m_row; }

	void SetCustomScale(Vector2 newScale);

private:

	Vector2 m_custom_scale{ -1,-1 };

	bool m_initialized{ false };

	Sprite m_spritesheet;

	float m_animTimeBetween{ 1.0 };
	float m_animTimer{ 0.0f };

	int m_event_frame[2]{ -1,-1 };

	bool m_event_triggered{ false };
	bool m_event_triggered_already{ false };

	bool m_event_2_triggered{ false };
	bool m_event_2_triggered_already{ false };

	int m_row{ 0 };
	int m_spriteCount{ 0 };
	int m_startSpriteIndex{ 0 };
	int m_currentSpriteIndex{ 0 };
	bool m_looping{ true };
};