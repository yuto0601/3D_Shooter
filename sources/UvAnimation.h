#pragma once

#include <DirectXMath.h>

class UvAnimation
{

public:
	UvAnimation()
	{
		// 今回はサイズを決め打ちにしますが、
		// いろいろなパターンに対応できるように改造してみてください。
		mLeftTop.x = 0.0f;
		mLeftTop.y = 0.0f;
		mSize.x = 1.0f / 4.0f;
		mSize.y = 1.0f / 4.0f;
		mIsPlaying = true;
	}

	// アニメーションを進める更新処理
	void Update();

	// 左上UV座標を取得する
	DirectX::XMFLOAT2 GetLeftTop()
	{
		return mLeftTop;
	}

	// アニメーション再生中か？
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
