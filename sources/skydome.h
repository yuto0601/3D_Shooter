#pragma once
#include "gameobject.h"
#include "CModel.h"
class Skydome:public GameObject
{
private:
	CModel* m_model;//3Dƒ‚ƒfƒ‹
public:
	bool Init();
	void Draw();
	void Update();
	void Finalize();
	const CModel* GetModel() {
		return m_model;
	}
	void SetModel(CModel* pmodel) {
		m_model = pmodel;
	}
};

