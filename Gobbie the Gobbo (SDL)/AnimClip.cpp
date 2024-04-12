#include "AnimClip.h"


AnimClip::AnimClip()
{
}

void AnimClip::Initialize(SDL_Renderer* renderer, string imgFile, AnimClipInfo anim)
{
	m_spritesheet.SetSprite(renderer, imgFile);
	m_row = anim.row;
	m_spriteCount = anim.spriteCount;
	m_animTimeBetween = anim.animTimeBetween;
	m_animTimer = 0.0f;
	m_event_triggered = false;
	m_event_triggered_already = false;
	m_event_2_triggered = false;
	m_event_2_triggered_already = false;
	m_event_frame[0] = anim.eventTrigger[0];
	m_event_frame[1] = anim.eventTrigger[1];
	m_startSpriteIndex = anim.start;
	m_currentSpriteIndex = m_startSpriteIndex;
	m_looping = anim.looping;

	m_initialized = true;

	SetAnimation(anim);
}

void AnimClip::SetAnimation(AnimClipInfo anim)
{
	m_row = anim.row;
	m_spriteCount = anim.spriteCount;
	m_animTimeBetween = anim.animTimeBetween;
	m_animTimer = 0.0f;
	m_event_triggered = false;
	m_event_triggered_already = false;
	m_event_2_triggered = false;
	m_event_2_triggered_already = false;
	m_event_frame[0] = anim.eventTrigger[0];
	m_event_frame[1] = anim.eventTrigger[1];
	m_startSpriteIndex = anim.start;
	m_currentSpriteIndex = m_startSpriteIndex;
	m_looping = anim.looping;

	if (m_spritesheet.GetSpriteTexture() == nullptr) m_initialized = false;

	SetAnimationSprite(m_row, m_currentSpriteIndex);
}

void AnimClip::DebugCurrentSpriteInfo()
{
	cout << "Current Sprite Info: " << endl;
	cout << " Row: " << m_row << endl;
	cout << " Amount of Animation Sprites On Row: " << m_currentSpriteIndex << endl;
	cout << "Sprite Source Info: " << endl;
	cout << " Source Rec X: " << GetSrcRect().x << endl;
	cout << " Source Rec Y: " << GetSrcRect().y << endl;
	cout << " Source Rec W: " << GetSrcRect().w << endl;
	cout << " Source Rec H: " << GetSrcRect().h << endl;
	cout << "Sprite Destination Info: " << endl;
	cout << " Destination Rec X: " << GetDesRect().x << endl;
	cout << " Destination Rec Y: " << GetDesRect().y << endl;
	cout << " Destination Rec W: " << GetDesRect().w << endl;
	cout << " Destination Rec H: " << GetDesRect().h << endl;
}

SDL_Texture* AnimClip::GetSprite()
{
	return m_spritesheet.GetSpriteTexture();
}

void AnimClip::SetAnimationSprite(int row, int column)
{
	if (!m_spritesheet.GetSpriteTexture())
	{
		cout << "Couldn't set sprite as no spritesheet." << endl;
		return;
	}

	m_spritesheet.SetSrcRect(Vector2(m_currentSpriteIndex * m_spritesheet.GetBaseImageSize().x, m_row * m_spritesheet.GetBaseImageSize().y));
}

void AnimClip::SetDesRect(float x, float y, float xScale, float yScale)
{
	m_spritesheet.SetDesRect(x, y, xScale, yScale);
}

void AnimClip::SetDesRect(Transform transform)
{
	SetDesRect(transform.position.x, transform.position.y, transform.scale.x, transform.scale.y);
}

void AnimClip::SetSrcRect(Transform transform)
{
	m_spritesheet.SetSrcRect(transform);
}

bool AnimClip::Update()
{
	if (m_event_triggered)
	{
		m_event_triggered_already = true;
	}
	if (m_event_2_triggered)
	{
		m_event_2_triggered_already = true;
	}

	m_animTimer += Time::GetDeltaTime();

	if (m_animTimer >= m_animTimeBetween)
	{
		if (m_event_frame[0] == m_currentSpriteIndex && !m_event_triggered_already)
		{
			m_event_triggered = true;
		}
		else if (m_event_frame[1] == m_currentSpriteIndex && !m_event_2_triggered_already)
		{
			m_event_2_triggered = true;
		}
		else 
		{
			m_event_triggered = false;
			m_event_2_triggered = false;
		}

		m_currentSpriteIndex++;

		if (m_currentSpriteIndex >= m_startSpriteIndex + m_spriteCount)
		{
			if (m_looping) 
			{
				m_currentSpriteIndex = m_startSpriteIndex;
				m_event_triggered_already = false;
				m_event_2_triggered_already = false;
			}
			else 
			{
				return true;
			}
		}

		SetAnimationSprite(m_row, m_currentSpriteIndex);

		m_animTimer = 0.0f;
		//DebugCurrentSpriteInfo();

	}

	return false;
}

void AnimClip::RenderUpdate(SDL_Renderer* renderer, Transform transform)
{
	if (m_custom_scale.x > 0 && m_custom_scale.y > 0)
	{
		transform.scale *= m_custom_scale;
	}

	SetDesRect(transform);

	SDL_Rect src = GetSrcRect();
	SDL_Rect des = GetDesRect();

	SDL_RenderCopy(renderer, GetSprite(), &src, &des);

}

void AnimClip::SetCustomScale(Vector2 newScale)
{

	m_custom_scale = newScale;
	
	m_custom_scale.x = SDL_clamp(m_custom_scale.x, 0.001f, 1.0f);
	m_custom_scale.y = SDL_clamp(m_custom_scale.y, 0.001f, 1.0f);
	
}
