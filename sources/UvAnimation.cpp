#include "UvAnimation.h"

void UvAnimation::Update()
{
	if (++mCounter % 4 == 0) { // アニメーションを１コマ右へ
		mLeftTop.x += mSize.x;

		if (mLeftTop.x >= 1.0f) // 右端に達した
		{
			mLeftTop.x = 0.0f;
			mLeftTop.y += mSize.y; // １コマ下へ

			if (mLeftTop.y >= 1.0f) // 下端に達した
			{
				//mLeftTop.y = 0.0f;
				mIsPlaying = false;
			}
		}
	}
}
