#pragma once

#include <DirectXMath.h>

class UvAnimation
{

public:
	UvAnimation()
	{
		// ����̓T�C�Y�����ߑł��ɂ��܂����A
		// ���낢��ȃp�^�[���ɑΉ��ł���悤�ɉ������Ă݂Ă��������B
		mLeftTop.x = 0.0f;
		mLeftTop.y = 0.0f;
		mSize.x = 1.0f / 4.0f;
		mSize.y = 1.0f / 4.0f;
		mIsPlaying = true;
	}

	// �A�j���[�V������i�߂�X�V����
	void Update();

	// ����UV���W���擾����
	DirectX::XMFLOAT2 GetLeftTop()
	{
		return mLeftTop;
	}

	// �A�j���[�V�����Đ������H
	bool IsPlaying()
	{
		return mIsPlaying;
	}

private:
	DirectX::XMFLOAT2 mLeftTop;
	DirectX::XMFLOAT2 mSize;
	int mCounter;
	bool mIsPlaying;

};
