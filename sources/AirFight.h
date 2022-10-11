#pragma once
#include "quad2d.h"

//���ԂƂ��ɓ���t�F�[�h�C��

//�^�C�g��
 void Title();
 void TitleUpdate();
 void TitleDraw();
 void TitleExit();
 //�Q�[��
void PlayGameInit();
void PlayGameUpdate();
void PlayGameDraw();
void PlayGameExit();
//���U���g
 void ResultInit();
 void ResultUpdate();
 void ResultDraw();
 void ResultExit();
 // ��ʒ萔
 enum class SCENENUM {
	 TITLE_INIT, TITLE,  // �^�C�g��
	 GAME_INIT, GAME,    // �Q�[��
	 RESULT_INIT, RESULT, // ���U���g

	 FADE
 };
class Game
{

public: // �N���X�̌��J�v�f

	// �����o�[�֐�
	//static void Initialize();	// �Q�[���̏���������
	//static void Update();		// �X�V����
	//static void Draw();			// �`�揈��
	//static void Release();		// �������



	//// GameObject�����֐�
	//static GameObject* CreateRoad(); // ���H
	//static GameObject* CreateShot(); // �e
	//static GameObject* CreateExplosion(); // ����
	//static GameObject* CreateTitleLogo(); // �^�C�g�����S
	//static GameObject* CreateFadePanel(); // 

	// ���̃Q�[���ɓo�ꂷ��Q�[���I�u�W�F�N�g
	static std::vector<GameObject*> mGameObjects;

private: // �N���X�̔���J�v�f
public:
	// �e�Ǘ��p�z��
	static std::vector<GameObject*> mShots;

	// �G�t�F�N�g�Ǘ��p�z��
	static std::vector<GameObject*> mEffects;
	static std::vector<Quad2D*> g_quad2Ds;

    

	static int mNextScene; // ���̃V�[��
	static SCENENUM mCurrentScene;  // ���݂̃V�[���i��ʁj

	static void SetNextScene(int next) {
		mNextScene = next;
		mCurrentScene = SCENENUM::FADE;
	}

	


};