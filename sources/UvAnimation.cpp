#include "UvAnimation.h"

void UvAnimation::Update()
{
	if (++mCounter % 4 == 0) { // �A�j���[�V�������P�R�}�E��
		mLeftTop.x += mSize.x;

		if (mLeftTop.x >= 1.0f) // �E�[�ɒB����
		{
			mLeftTop.x = 0.0f;
			mLeftTop.y += mSize.y; // �P�R�}����

			if (mLeftTop.y >= 1.0f) // ���[�ɒB����
			{
				//mLeftTop.y = 0.0f;
				mIsPlaying = false;
			}
		}
	}
}
