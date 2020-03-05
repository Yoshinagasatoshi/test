#pragma once

#include "SkinModelEffect.h"
#include "Skeleton.h"

/*!
*@brief	FBX�̏�����B
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};
/*!
*@brief	�X�L�����f���N���X�B
*/
class SkinModel
{
public:
	//���b�V�������������Ƃ��̃R�[���o�b�N�֐��B
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*
	*@brief �R���X�g���N�^�B
	*/
	SkinModel();
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~SkinModel();
	/// <summary>
	/// �A�b�v�f�[�g
	/// </summary>
	void Update();
	/*!
	*@brief	�������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*@param[in] enFbxUpAxis		fbx�̏㎲�B�f�t�H���g��enFbxUpAxisZ�B
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
	/*!
	*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
	*@param[in]	position	���f���̍��W�B
	*@param[in]	rotation	���f���̉�]�B
	*@param[in]	scale		���f���̊g�嗦�B
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	�{�[���������B
	*@param[in]		boneName	�{�[���̖��O�B
	*@return	���������{�[���B������Ȃ������ꍇ��nullptr��Ԃ��܂��B
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	���f����`��B
	*@param[in]	viewMatrix		�J�����s��B
	*  ���[���h���W�n��3D���f�����J�������W�n�ɕϊ�����s��ł��B
	*@param[in]	projMatrix		�v���W�F�N�V�����s��B
	*  �J�������W�n��3D���f�����X�N���[�����W�n�ɕϊ�����s��ł��B
	*/
	void Draw( CMatrix viewMatrix, CMatrix projMatrix );

	/// <summary>
	// �}�e���A���ɑ΂��ăN�G�����s��
	/// </summary>
	void QueryMaterials(std::function<void(SkinModelEffect*)> func)
	{
		m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
			func(reinterpret_cast<SkinModelEffect*>(material));
			});
	}
	/*!
	*@brief	�X�P���g���̎擾�B
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	���b�V������������B
	*@param[in] onFindMesh		���b�V�������������Ƃ��̃R�[���o�b�N�֐�
	*/
	void FindMesh(OnFindMesh onFindMesh) const
	{
		for (auto& modelMeshs : m_modelDx->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				onFindMesh(mesh);
			}
		}
	}
	CMatrix GetWorldMatrix() const
	{
		return m_worldMatrix;
	}
	/*!
	*@brief	SRV�̃��W�X�^�ԍ��B
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<�f�B�t���[�Y�e�N�X�`���B
		enSkinModelSRVReg_BoneMatrix,				//!<�{�[���s��B
	};
private:
	/*!
	*@brief	�T���v���X�e�[�g�̏������B
	*/
	void InitSamplerState();
	/*!
	*@brief	�萔�o�b�t�@�̍쐬�B
	*/
	void InitConstantBuffer();
	/// <summary>
	/// �A���x�h�e�N�X�`���̏������B
	/// </summary>
	void InitAlbedoTexture();

	/// <summary>
	/// �f�B���N�V�������C�g�̏������B
	/// </summary>
	void InitDirectionLight();
	/*!
	*@brief	�X�P���g���̏������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*/
	void InitSkeleton(const wchar_t* filePath);
	
private:
	//�萔�o�b�t�@�B
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
	};
	//�f�B���N�V�������C�g�̒萔�o�b�t�@
	struct SDirectionLight {
		CVector4 direction[4];
		CVector4 color[4];
	};
	//���C�g�̒萔�o�b�t�@
	struct SLight {
		SDirectionLight     directionLight; //�f�B���N�V�������C�g
		CVector3            eyePos;         //���_�̍��W
		float               specPow;        //���ʔ̎Ђ̍i��
		CVector3            AmbLight;       //�A���r�G���g���C�g
	};
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	   //!<FBX�̏�����B
	ID3D11Buffer*		m_cb = nullptr;					   //!<�萔�o�b�t�@�B
	ID3D11Buffer*       m_lightCb = nullptr;               //!<���C�g�p�萔�o�b�t�@�B
	//SDirectionLight     m_dirLight;                        //!<�f�B���N�V�������C�g
	SLight              m_light;                           //���C�g�̍\����
	Skeleton			m_skeleton;						   //!<�X�P���g���B
	CMatrix				m_worldMatrix;					   //!<���[���h�s��B
	DirectX::Model*		m_modelDx;					   	   //!<DirectXTK���񋟂��郂�f���N���X�B
	ID3D11SamplerState* m_samplerState = nullptr;		   //!<�T���v���X�e�[�g�B
	ID3D11ShaderResourceView* m_albedoTextureSRV = nullptr;//!<�A���x�h�e�N�X�`���B
	const int Lightnumber = 4;
};
