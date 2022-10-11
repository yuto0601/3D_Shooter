#include <vector>
#include<array>
#include<string>
#include	"player.h"
#include	"CCamera.h"
#include "skydome.h"
#include"BoundingSphere.h"
#include "Enemy.h"
#include "bullet.h"
#include"ModelMgr.h"
#include "quad2d.h"
#include "CBillBoard.h"
#include "AirFight.h"
#include "Fader.h"
#include"myimgui.h"

std::vector<Quad2D*>Game::g_quad2Ds;
std::vector<GameObject*> Game::mShots;
std::vector<GameObject*> Game::mEffects;

Quad2D TitleScreen;//�^�C�g�����
Quad2D EndScreen;//�^�C�g�����
Quad2D g_quad2d;//2D�Z�`
extern STATE mState;
Quad2D E_Target;
Player g_player;
Skydome g_skydome;
bool viewBS;
extern SCENENUM sceneNUM;

int enem_Max=50;

Enemy rootEnem;
std::vector<Enemy> g_enemy(enem_Max);//�G

std::vector<GameObject> fighters;
static XMFLOAT3 cusor;
CBillBoard g_billboard;//�r���{�[�h
static float xseed;
//static float UIxPos;
//static float UIyPos;
//static float UIzPos;
static int countdeath;
XMFLOAT4X4 cameramtx;//UI�Ɏg�p����J�����s��
extern bool nextflg;

std::vector<std::unique_ptr<Bullet>> g_bullets;//�J���̃|�C���^�[��錾�i�e)
BoundingSphere g_bs;//�����pBS
//std::vector<std::unique_ptr<BoundingSphere >>g_gun;//�e��BS
DirectX::XMFLOAT2 uv[4] = { XMFLOAT2(0,0),XMFLOAT2(1,0) ,XMFLOAT2(0,1) ,XMFLOAT2(1,1) };//uv���W
//�󒆐�Ŏg�p���郂�f��ID

static float Upper=0;

enum class MODELID { 
	PLAYER,
	SKYDOME,
	ENEMY,
	MISSILE,
	LASER,
};

struct ModelDataList 
{
	MODELID id;//pObj
	std::string modelname;
	std::string vsfilename;
	std::string  psfilename;
	std::string texfolder;
};

std::vector<ModelDataList> g_modellist = {
	{MODELID::PLAYER,"assets/f1/f1.x","shader/vs.hlsl","shader/ps.hlsl","assets/f1/"},
	{MODELID::SKYDOME,"assets/skydome/skydome.x","shader/vs.hlsl","shader/pstexcol.hlsl","assets/skydome/"},
	{MODELID::ENEMY,"assets/f1/f1.x","shader/vs.hlsl","shader/ps.hlsl","assets/f1/"},
	{MODELID::MISSILE,"assets/starwars/shot.x","shader/vs.hlsl","shader/ps.hlsl","assets/starwars/"},
	{MODELID::LASER,"assets/starwars/laser.x","shader/vs.hlsl","shader/ps.hlsl","assets/starwars/"},
};

//�e�`��
void DrawBullets() {
//�S�e�`��
	for (auto& b : g_bullets) {
	b->Draw();
	}
}
//�e�X�V
void UpdateBullets() {
	//�S�e�X�V
	for (auto& b : g_bullets) {
		b->Update();
		for (auto& e : g_enemy) {
			float distance;
			DX11p2pLength(e.GetPos(), b->GetPos(), distance);
			if (distance <= e.GetBS().r) {
				e.kill_flg = true;
			}
		}
	}

	//����ł���e�폜
	auto it = g_bullets.begin();
	while (it != g_bullets.end()) {
		if ((*it)->isLive() == false) {
			it = g_bullets.erase(it);
		}
		else ++it;
	}
}
//�e����
void FireBullet(XMFLOAT4X4 mtx) {
	std::unique_ptr<Bullet> p;

	p = std::make_unique<Bullet>();//�o���b�g

	//�e�̏�����
	p->Init();//��������
	               //���f���Z�b�g
	p->SetModel(
		ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::MISSILE)].modelname));
	//�����ʒu�Z�b�g
	p->SetInitialPos(XMFLOAT3(mtx._41, mtx._42, mtx._43));
	//���˕����Z�b�g
	p->SetDirection(mtx);
	//�Ō�ɒe��ǉ�
	//vector�R���e�i�Ƀ��j�[�N�|�C���^���Z�b�g
	g_bullets.emplace_back(std::move(p));


};


//�J����//////////////////////////////////////////////////////////////////////////////////////////
//FPS�J����
void FPSCamera() {
	//�s��擾
	XMFLOAT4X4 mtx;
	GameObject* pObj = new Player();
	mtx = pObj->GetMtx();
	//Z���擾
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//�����_�v�Z
	XMFLOAT3 lookat;
	lookat.x = mtx._41 + zaxis.x;
	lookat.y = mtx._42 + zaxis.y;
	lookat.z = mtx._43 + zaxis.z;
	//�J�����ʒu�v�Z
	XMFLOAT3 eye;
	eye.x = mtx._41;
	eye.y = mtx._42;
	eye.z = mtx._43;
	//�J�����ʒu�v�Z
	XMFLOAT3 up;
	up.x = mtx._21;
	up.y = mtx._22;
	up.z = mtx._23;

	//�J�����N���X�ɃZ�b�g
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);
	CCamera::GetInstance()->CreateCameraMatrix();
}
//�Œ蒍���J����
void FixedCamera(Player name) {
	XMFLOAT3 lookat;
	lookat = name.GetPos();
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->CreateCameraMatrix();
}
//������ƒx��Ă���J����
void DelayCamera(Player name) {
	int DerayPow=15;
	//�v���C���̍s��擾
	XMFLOAT4X4 mtx;
	mtx = name.GetMtx();
	//�r���[�ϊ��s���ύX
	static int svidx = 0;
	static bool firstflag = false;

	static std::array<XMFLOAT3, 100>zaxis;
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;

	static std::array<XMFLOAT3, 100>yaxis;
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;

	static std::array<XMFLOAT3, 100>pos;
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;

	svidx++;
	if (svidx > zaxis.size() - 1) {
		svidx = 0;
		firstflag = true;
	}
	//"DerayPow"�t���[���O�̏����g�p���ăJ�������쐬
	int useidx;
	useidx = svidx - DerayPow;
	if (useidx < 0) {
		if (firstflag == false) {
			useidx = 0;
		}
		else {
			useidx = svidx + zaxis.size() - DerayPow;
		}
	}
	XMFLOAT3 eye;
	XMFLOAT3 lookat;
	XMFLOAT3 up;

	lookat = pos[useidx];

	eye.x = pos[useidx].x - 30 * zaxis[useidx].x;
	eye.y = pos[useidx].y - 30 * zaxis[useidx].y;
	eye.z = pos[useidx].z - 30 * zaxis[useidx].z;

	up = yaxis[useidx];

	//�r���[�ϊ��s��̐���
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetUp(up);
	CCamera::GetInstance()->CreateCameraMatrix();


}

void TPSCamera(Player name) {
	//�v���C���̍s��擾
	XMFLOAT4X4 mtx;
	mtx = name.GetMtx();
	//�r���[�ϊ��s���ύX
	static int svidx = 0;
	static bool firstflag = false;

	static XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	static XMFLOAT3 yaxis;
	yaxis.x = mtx._21;
	yaxis.y = mtx._22;
	yaxis.z = mtx._23;

	static XMFLOAT3 pos;
	pos.x = mtx._41;
	pos.y = mtx._42;
	pos.z = mtx._43;

	XMFLOAT3 eye;
	XMFLOAT3 lookat;
	XMFLOAT3 up;

	lookat = XMFLOAT3{ pos.x , pos.y ,pos.z };

	eye.x = pos.x - 30 * zaxis.x + yaxis.x * 10;
	eye.y = pos.y - 30 * zaxis.y + yaxis.y * 10;
	eye.z = pos.z - 30 * zaxis.z + yaxis.z * 10;

	up = yaxis;

	//�r���[�ϊ��s��̐���
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetUp(up);
	CCamera::GetInstance()->CreateCameraMatrix();
}

//�����蔻��
bool Intersect( BoundingSphere&a,  BoundingSphere& b) {
	float x2 = (a.GetWPos().x - b.GetWPos().x)* (a.GetWPos().x - b.GetWPos().x)+
		             (a.GetWPos().y - b.GetWPos().y)+ (a.GetWPos().y - b.GetWPos().y);
	float x=a.GetR()+b.GetR();
	return x2 <= x;
}
void UIDraw() {
	//�QD�Z�`�`��
	TurnOffZbuffer();//Z�o�b�t�@�I�t
	static int loop = 0;
	loop++;
	static int idx = 0;
	if (loop % 30 == 29) {
		idx++;
		idx = idx % 7;
	}
	XMFLOAT2 uv[7][4] = {
	{
		{0.0f / 8.0f,0.0f},
		{1.0f / 8.0f,0.0},
		{1.0f / 8.0f,1.0f / 4.0f},
		{1.0f / 8.0f,1.0f / 4.0f}
	},
		{
		{1.0f / 8.0f,0.0f},
		{2.0f / 8.0f,0.0},
		{1.0f / 8.0f,1.0f / 4.0f},
		{2.0f / 8.0f,1.0f / 4.0f}
	},
		{
		{2.0f / 8.0f,0.0f},
		{3.0f / 8.0f,0.0},
		{2.0f / 8.0f,1.0f / 4.0f},
		{1.0f / 8.0f,1.0f / 4.0f}
	},
		{
		{3.0f / 8.0f,0.0f},
		{4.0f / 8.0f,0.0},
		{3.0f / 8.0f,1.0f / 4.0f},
		{4.0f / 8.0f,1.0f / 4.0f}
	},
		{
		{4.0f / 8.0f,0.0f},
		{5.0f / 8.0f,0.0},
		{4.0f / 8.0f,1.0f / 4.0f},
		{5.0f / 8.0f,1.0f / 4.0f}
	},
		{
		{6.0f / 8.0f,0.0f},
		{7.0f / 8.0f,0.0},
		{6.0f / 8.0f,1.0f / 4.0f},
		{7.0f / 8.0f,1.0f / 4.0f}
	},
	};
	g_quad2d.updateVertex(200, 200, XMFLOAT4(1, 1, 1, 1), uv[idx]);
}

void TitleScreenDraw()
{
	//�QD�Z�`�`��
	TurnOffZbuffer();//Z�o�b�t�@�I�t
	
TitleScreen.Init(1280, 1280, XMFLOAT4(1, 1, 1, a), uv);
TitleScreen.SetScale(1, 1, 1);
TitleScreen.SetPosition(640, 360, 1);


}

void EndScreenDraw()
{
	//�QD�Z�`�`��
	TurnOffZbuffer();//Z�o�b�t�@�I�t
	static int loop = 0;
	loop++;
	static int idx = 0;
	if (loop % 30 == 29) {
		idx++;
		idx = idx % 7;
	}
	XMFLOAT2 uv[7][4] = {
	{
		{0.0f / 8.0f,0.0f},
		{1.0f / 8.0f,0.0},
		{1.0f / 8.0f,1.0f / 4.0f},
		{1.0f / 8.0f,1.0f / 4.0f}
	},
		{
		{1.0f / 8.0f,0.0f},
		{2.0f / 8.0f,0.0},
		{1.0f / 8.0f,1.0f / 4.0f},
		{2.0f / 8.0f,1.0f / 4.0f}
	},
		{
		{2.0f / 8.0f,0.0f},
		{3.0f / 8.0f,0.0},
		{2.0f / 8.0f,1.0f / 4.0f},
		{1.0f / 8.0f,1.0f / 4.0f}
	},
		{
		{3.0f / 8.0f,0.0f},
		{4.0f / 8.0f,0.0},
		{3.0f / 8.0f,1.0f / 4.0f},
		{4.0f / 8.0f,1.0f / 4.0f}
	},
		{
		{4.0f / 8.0f,0.0f},
		{5.0f / 8.0f,0.0},
		{4.0f / 8.0f,1.0f / 4.0f},
		{5.0f / 8.0f,1.0f / 4.0f}
	},
		{
		{6.0f / 8.0f,0.0f},
		{7.0f / 8.0f,0.0},
		{6.0f / 8.0f,1.0f / 4.0f},
		{7.0f / 8.0f,1.0f / 4.0f}
	},
	};
	EndScreen.updateVertex(200, 200, XMFLOAT4(1, 1, 1, 1), uv[idx]);
}
//�Q�[���̏�����

/////////////////////////////////////////////���C�����[�v/////////////////////////////////////////////

/////////////////////////////////////�^�C�g�����///////////////////////////////////////////////////
void Title()
{
	
	//�^�C�g����ʂ̏�����
	TitleScreen.Init(1209, 737, { 1,1,1,1 }, uv);
	TitleScreen.SetScale(1, 1, 1);
	TitleScreen.SetPosition(640, 300, 1);//640, 360, 1
	TitleScreen.LoadTexture("assets/starwars/texture/TitleScreen.jpg");
}

// IMGUI�E�C���h�E
void Controller() {
	ImGui::SetNextWindowPos(ImVec2(1006, 122), 1);
	ImGui::SetNextWindowSize(ImVec2(261, 86), 1);
	ImGui::Begin(u8"�������");
	std::string str;

	str = u8"���@�FW,A,S,D�L�[�F�ړ�������ς���";
	ImGui::Text(str.c_str());
	str = u8"���@�FQ,E�L�[�F��]G";
	ImGui::Text(str.c_str());
	str = u8"���@�FL�F�e�̔���";
	ImGui::Text(str.c_str());
	ImGui::End();

}

void TitleUpdate()
{

	switch (FaderGetState())
	{

	case FADEIN:
		a -= 0.01f;
		if (a <= 0.0f)
		{
			a = 0;
			mState = NONE;
		}
		break;

	case FADEOUT:
		a += 0.01f;
		if (a >= 1.0f)
		{
			a = 1;
			mState = NONE;
		}
		break;
	case NONE:

		break;
	}
	TitleScreenDraw();
	if ((CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))) {
		 FaderFadeIn(); 

	}

	if (NONE == FaderGetState() && a == 0) {
		nextflg = true;
	}
}

void TitleDraw()
{
	TitleScreen.Draw();
	imguiDraw(Controller);
}

void TitleExit()
{
	TitleScreen.UnInit();
}

/////////////////////////////////////�^�C�g�����///////////////////////////////////////////////////

/////////////////////////////////////////�Q�[��////////////////////////////////////////////////////////
void PlayGameInit() {

	//�t�@�C���ǂݍ���
	for (int i = 0; i < g_modellist.size(); ++i) {
	 ModelMgr::GetInstance().LoadModel(
		g_modellist[i].modelname,
		g_modellist[i].vsfilename,
		g_modellist[i].psfilename,
		g_modellist[i].texfolder);

	}
	
	//���C����ʂɏo�Ă���UI�̏�����
	// 
	// 
	//�Ə���
	g_quad2d.Init(480,480,{1,1,1,1},uv);
	g_quad2d.SetScale(0.1,0.1,1);
	g_quad2d.SetPosition(640,360,1);
	g_quad2d.LoadTexture("assets/starwars/texture/E_Target.png");


	//�n�`
	// 
	// 
	//�X�J�C�h�[��������
	g_skydome.Init();
g_skydome.SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::SKYDOME)].modelname));
	




//characther
// 
// 
// �v���C��������
    g_player.Init();
	g_player.SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::PLAYER)].modelname));
	//�G�L������������
	//g_enemy.reserve(enem_Max);
	while (g_enemy.size() < enem_Max)
	{
		Enemy e = rootEnem;
		g_enemy.push_back(e);
	}
	for (auto& e : g_enemy) {
		e.SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::ENEMY)].modelname));
		e.Init();
	}	
	//Player�̋��E���쐬
	{
		//���f���̑S���_�𔲂��o��
		const CModel*pmodel = g_player.GetModel();
		const ModelData& md = pmodel->GetModelData();
		const std::vector<Mesh>& meshes = md.GetMeshes();

		//���E�����ǂ��ɂ��邩�̍��W���i�[���钸�_�Q
		std::vector<XMFLOAT3> vertices;

		for (auto& m : meshes) {
			for (auto& v : m.m_vertices) {
				vertices.emplace_back(v.m_Pos);
			}
		}
		g_bullets.clear();
		//���E��������
		g_bs.Init(
			vertices,//���_�Q
			XMFLOAT4(1, 1, 0.4, 0.3f));//���_�J���[
	}

#include <time.h>
	srand((unsigned int)time(NULL));

	
	
}
void PlayGameUpdate() {
			//////////////////////////////////////////////////////*�Q�[���T�C�N��*/////////////////////////////////////////////////////////
			// �v���C���X�V
	
			g_player.Update();
			//���E���ړ��i�����蔻��̓v���C���[�̃|�W�V�������Q�ƒ��j
			g_bs.UpdatePosition(g_player.GetMtx());
			//�e����
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_L)) {
				FireBullet(g_player.GetMtx());
			}

			//�����蔻��\���ύX
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_B)) {
				viewBS = !viewBS;
			}

			//�e�X�V
			UpdateBullets();
			//�J�����Q
		//	DelayCamera(g_player);
			TPSCamera(g_player);
			//Enemy�X�V
			for (auto& e : g_enemy) {
				int act = rand() % 7;
				int time = rand() % 60 + 1;
				e.Update(act, time);
			}
				
			switch (FaderGetState())
			{

			case FADEIN:
				a -= 0.01f;
				FaderUpdate();
				if (a <= 0.0f)
				{
					a = 0;
					mState = NONE;
				}
				break;

			case FADEOUT:
				a += 0.01f;
				FaderUpdate();
				if (a >= 1.0f)
				{
					a = 1;
					mState = NONE;
				}
				break;
			case NONE:

				break;
			}

			//Enemy�폜
			auto it = g_enemy.begin();
			while (it != g_enemy.end()) {
				if ((it)->isDeath() == true) {
					it = g_enemy.erase(it);

				}
				else ++it;
			}
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DELETE))
			{
				g_enemy.erase(g_enemy.begin());
			}
			static bool isEnd=false;
			//5�@���j�ŃN���A
			if (enem_Max-g_enemy.size() >= 5&&!isEnd)
			{
				extern Quad2D Fader;
				extern Quad2D* FaderBuffer;
				extern DirectX::XMFLOAT2 uvfader[4];
				Fader.Init(1280, 1280, XMFLOAT4(1, 1, 1, a), uvfader);
				Fader.SetScale(1, 1, 1);
				Fader.SetPosition(640, 360, 1);
				FaderBuffer = &Fader;

				isEnd = true;
				//�I��������
				FaderFadeIn();
			}
			switch (FaderGetState())
			{

			case FADEIN:
				a -= 0.01f;
				if (a <= 0.0f)
				{
					a = 0;
					mState = NONE;
				}
				break;

			case FADEOUT:
				a += 0.01f;
				if (a >= 1.0f)
				{
					a = 1;
					mState = NONE;
				}
				break;
			case NONE:

				break;
			}

			if (NONE == FaderGetState() && a == 0&&isEnd) {
			 nextflg = true;
			 isEnd = false;
			}

			//�Ə���X�V
			UIDraw();
			
			for (auto e:g_enemy)
			{
				if (e.isOut()) {
					e.attraction();
				}
			}

			g_player.attraction();

			                      //////////////////////////�Q�[�����C�����[�v////////////////////////////////////////////////////////////////////
		














			//////////////////////////////�G���h�X�N���[��//////////////////////////////////////////////////
			////END��ʕ`��
			//EndScreenDraw();
			////�X�y�[�X�Ń^�C�g���ɖ߂�
 		//	if ((CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))) {
			//
			//}
			//Player�J�����Q
			DelayCamera(g_player);
			
	}
void PlayGameDraw() {
		//�X�J�C�h�[���\��
		g_skydome.Draw();
		//�G�`��
		for (auto& e : g_enemy) {
			e.Draw();
			if (viewBS) {
				e.BSDraw();
			}
		}
		// �v���C���`��
		g_player.Draw();
		if (viewBS) {
			g_bs.Draw();
		}


		//�e�`��
		DrawBullets();
		//�Ə���X�V
		g_quad2d.Draw();
}


void PlayGameExit() {
	// �v���C���I������
	g_player.Finalize();
	//�G�I������
	for (auto& e : g_enemy) {
		e.Finalize();
	}
	//�X�J�C�h�[���I������
	g_skydome.Finalize();
	
	//���E���I������
	g_bs.Exit();
//UI�I������
	g_billboard.Dispose();
}

////////////////////////////////�Q�[����//////////////////////////////////////////////////





//////////////////////////���U���g////////////////////////////////////////////////////////////////////////////////////////
void ResultInit()
{
	EndScreen.Init(1280, 1280, { 1,1,1,1 }, uv);
	EndScreen.SetScale(1, 1, 1);
	EndScreen.SetPosition(640, 360, 1);
	EndScreen.LoadTexture("assets/starwars/texture/gameoverscreen.png");
}

bool toTitle=false;
void ResultUpdate()
{
	switch (FaderGetState())
	{

	case FADEIN:
		a -= 0.01f;
		if (a <= 0.0f)
		{
			a = 0;
			mState = NONE;
		}
		break;

	case FADEOUT:
		a += 0.01f;
		if (a >= 1.0f)
		{
			a = 1;
			mState = NONE;
		}
		break;
	case NONE:

		break;
	}
	TitleScreenDraw();
	
	if ((CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))) {
		FaderFadeIn();
		toTitle = true;
	}

	if (NONE == FaderGetState() && a == 0 && toTitle) {
		nextflg = true;
	}
}

void ResultDraw()
{
	EndScreen.Draw();
}

void ResultExit() {
	EndScreen.UnInit();
}
/////////////////////////////////////////////���C�����[�v/////////////////////////////////////////////