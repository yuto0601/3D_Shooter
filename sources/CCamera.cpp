#include"CCamera.h"
CCamera* CCamera::GetInstance() {
	static CCamera instance;
	return &instance;
}
void CCamera::Init(float nearclip, float farclip, float fov,
	float width, float height,
	XMFLOAT3 eye, XMFLOAT3 lookat, XMFLOAT3 up) {

	SetProjection(nearclip, farclip, fov, width, height);
	SetCamera(eye, lookat, up);
}
void CCamera::SetNear(float nearclip) {
	m_near = nearclip;
}

void CCamera::SetFar(float farclip) {
	m_far = farclip;
}

void CCamera::SetFov(float fov) {
	m_Fov = fov;
}

void CCamera::SetAspect(float width, float height) {
	m_Aspect = width / height;
}

void CCamera::SetProjection(float nearclip, float farclip, float fov, float width, float height) {
	SetNear(nearclip);
	SetFar(farclip);
	SetFov(fov);
	SetAspect(width, height);
	CreateProjectionMatrix();
}

void CCamera::SetCamera(const XMFLOAT3& eye, const XMFLOAT3& lookat, const XMFLOAT3& up) {

	SetEye(eye);
	SetLookat(lookat);
	SetUp(up);
	CreateCameraMatrix();
}

void CCamera::SetEye(const XMFLOAT3& eye) {
	m_Eye = eye;
}

void CCamera::SetLookat(const XMFLOAT3& lookat) {
	m_Lookat = lookat;
}

void CCamera::SetUp(const XMFLOAT3& up) {
	m_Up = up;
}

void CCamera::SetEyeOnly(const XMFLOAT3& eye) {
	m_Eye = eye;
	CreateCameraMatrix();
}
void CCamera::SetLookatOnly(const XMFLOAT3& lookat) {
	m_Lookat = lookat;
	CreateCameraMatrix();
}

void CCamera::SetUpOnly(const XMFLOAT3& up) {
	m_Up = up;
	CreateCameraMatrix();
}

void CCamera::CreateCameraMatrix() {
	ALIGN16 XMVECTOR Eye = XMVectorSet(m_Eye.x, m_Eye.y, m_Eye.z, 0.0f);
	ALIGN16 XMVECTOR At = XMVectorSet(m_Lookat.x, m_Lookat.y, m_Lookat.z, 0.0f);
	ALIGN16 XMVECTOR Up = XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.0f);

	ALIGN16 XMMATRIX camera;
	camera = XMMatrixLookAtLH(Eye, At, Up);

	XMStoreFloat4x4(&m_Camera, camera);
}

void CCamera::CreateProjectionMatrix() {

	ALIGN16 XMMATRIX projection;

	projection = XMMatrixPerspectiveFovLH(m_Fov, m_Aspect, m_near, m_far);

	XMStoreFloat4x4(&m_Projection, projection);

}

const XMFLOAT4X4& CCamera::GetCameraMatrix() {
	return m_Camera;
}

const XMFLOAT4X4& CCamera::GetProjectionMatrix() {
	return m_Projection;
}

float CCamera::GetFov() const {
	return m_Fov;
}

const XMFLOAT3& CCamera::GetEye() const {
	return m_Eye;
}

const XMFLOAT3& CCamera::GetLookat() const {
	return m_Lookat;
}

const XMFLOAT3& CCamera::GetUp() const {
	return m_Up;
}

float flame;
bool keyInput = false;
bool isRot;

void CCamera::Update(XMFLOAT4X4 p_mtx,XMFLOAT3 p_pos,XMFLOAT3 p_forward,XMFLOAT3 p_up) {
	/*カメラの処理*/ 
		//カメラの種類の変更
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_V)) {
			if (!inputV) {
				g_CamVal++;
				if (g_CamVal > MAXCAM) {
					g_CamVal = 0;
				}
			}
			inputV = true;
		}
		else { inputV = false; }
		XMFLOAT3 eye = p_pos;
		XMFLOAT3 up = XMFLOAT3{ 0,1,0 };
		XMFLOAT3 lookat;
		lookat.x = p_pos.x + p_forward.x;
		lookat.y = p_pos.y + p_forward.y;
		lookat.z = p_pos.z + p_forward.z;


		//遅れてくるカメラの下処理

		static int svidx = 0;
		static bool firstflag = false;

		XMFLOAT4X4 mtx;
		mtx = p_mtx;


		static std::array<XMFLOAT3, 100> zaxis;
		zaxis[svidx].x = mtx._31;
		zaxis[svidx].y = mtx._32;
		zaxis[svidx].z = mtx._33;

		static std::array<XMFLOAT3, 100> yaxis;
		yaxis[svidx].x = mtx._21;
		yaxis[svidx].y = mtx._22;
		yaxis[svidx].z = mtx._23;

		static std::array<XMFLOAT3, 100> pos;
		pos[svidx].x = mtx._41;
		pos[svidx].y = mtx._42;
		pos[svidx].z = mtx._43;

		svidx++;
		if (svidx > zaxis.size() - 1) {
			svidx = 0;
			firstflag = true;
		}



		switch (g_CamVal)
		{
		case 0:
			break;
		case 1:
			eye = XMFLOAT3{ 0, 0, -100 };
			up = XMFLOAT3{ 0,1,0 };
			lookat = p_pos;
			break;
		case 2:
			eye = p_pos;
			lookat = XMFLOAT3{ p_pos.x + p_forward.x * 10.0f,p_pos.y + p_forward.y * 10.0f,p_pos.z + p_forward.z * 10.0f };
			up = p_up;
			break;
		case 3:
			eye = XMFLOAT3{ p_pos.x - p_forward.x * 30 + p_up.x * 10,
							p_pos.y - p_forward.y * 30 + p_up.y * 10,
							p_pos.z - p_forward.z * 30 + p_up.z * 10 };
			lookat = XMFLOAT3{ p_pos.x + p_up.x * 10,
							   p_pos.y + p_up.y * 10,
							   p_pos.z + p_up.z * 10 };
			up = p_up;


			break;
		case 4:
			int useidx;
			useidx = svidx - 20;
			if (useidx < 0) {
				if (!firstflag) {
					useidx = 0;
				}
				else {
					useidx = svidx + zaxis.size() - 20;
				}
			}

			lookat.x = pos[useidx].x + p_up.x * 10;
			lookat.y = pos[useidx].y + p_up.y * 10;
			lookat.z = pos[useidx].z + p_up.z * 10;

			eye.x = pos[useidx].x - 30 * zaxis[useidx].x + p_up.x * 10;
			eye.y = pos[useidx].y - 30 * zaxis[useidx].y + p_up.y * 10;
			eye.z = pos[useidx].z - 30 * zaxis[useidx].z + p_up.z * 10;
			up = yaxis[useidx];

			break;
		default:
			break;
		}

		if (g_CamVal != 0)
			CCamera::GetInstance()->SetCamera(eye, lookat, up);

	return;
}