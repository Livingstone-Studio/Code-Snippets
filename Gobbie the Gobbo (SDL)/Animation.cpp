#include "Animation.h"

Animation::Animation()
	: animClips()
{
}

Animation::~Animation()
{
}

void Animation::InitAnim(SDL_Renderer* renderer, string imgFile, AnimationInfo anim)
{
	m_sprite_name__ = imgFile;
	m_offset = anim.offset;
	animClips[0].Initialize(renderer, imgFile, anim.actionAnimClip);
	animClips[1].Initialize(renderer, imgFile, anim.actionAnimShadow);
}

void Animation::ChangeAnimation(SDL_Renderer* renderer, AnimationInfo animInfo)
{
	if (m_anim_name != animInfo.name) 
	{
		if (animClips[0].IsInitialized() && animClips[1].IsInitialized())
		{
			m_anim_name = animInfo.name;
			m_offset = animInfo.offset;

			animClips[0].SetAnimation(animInfo.actionAnimClip);

			animClips[1].SetAnimation(animInfo.actionAnimShadow);
		}
		else 
		{
			InitAnim(renderer, m_sprite_name__, animInfo);
		}
	}
}

void Animation::Start()
{
}

void Animation::Update()
{
	for (int i = 0; i < 2; i++)
	{
		//cout << i << ":" << endl;

		bool clipComp = animClips[i].Update();

		if (i == 0) completed = clipComp;
	}
}

void Animation::RenderUpdate(SDL_Renderer* renderer, Transform transform)
{
	for (int i = 0; i < 2; i++)
	{
		animClips[1 - i].RenderUpdate(renderer, transform);
	}
}

Vector2 Animation::GetOffset()
{
	return m_offset;
}

void Animation::UpdateSRC(Transform t, Transform y)
{
	animClips[0].SetSrcRect(t);
	animClips[1].SetSrcRect(y);
}

void Animation::SetCustomScaleAnimTwo(Vector2 newScale)
{
	animClips[1].SetCustomScale(newScale);
}

