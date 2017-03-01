#ifndef __GENERAL
#define __GENERAL

class CEntity;

class CVector2D
{
public:
	float		x, y;

	CVector2D()
	{
	};

	CVector2D(float fX, float fY)
		: x(fX), y(fY)
	{
	};
};

class CVector
{
public:
	float		x, y, z;

	CVector()
	{
	};

	CVector(float fX, float fY, float fZ = 0.0f)
		: x(fX), y(fY), z(fZ)
	{
	};

	CVector(const RwV3d& rwVec)
		: x(rwVec.x), y(rwVec.y), z(rwVec.z)
	{
	};

	CVector&		operator+=(const CVector& vec)
			{ x += vec.x; y += vec.y; z += vec.z;
			return *this; };

	friend inline float DotProduct(const CVector& vec1, const CVector& vec2)
		{ return vec1.x * vec2.x + vec1.x * vec2.y + vec1.z * vec2.z; };

	friend inline CVector operator*(const CVector& in, float fMul)
		{ return CVector(in.x * fMul, in.y * fMul, in.z * fMul); };
};

class CMatrix
{
public:
    RwMatrix		matrix;
    RwMatrix*		matrixPtr;
    BOOL			haveRwMatrix;

	CMatrix()
		: matrixPtr(NULL), haveRwMatrix(FALSE)
	{
	};

	CMatrix(RwMatrix* pMatrix, bool bAttach)
		: matrixPtr(pMatrix), haveRwMatrix(bAttach)
	{
		memcpy(&matrix, pMatrix, sizeof(RwMatrix));
	};

	CMatrix(CVector vecRight, CVector vecUp, CVector vecAt, CVector vecPos)
	{
		matrix.right.x = vecRight.x;
		matrix.right.y = vecRight.y;
		matrix.right.z = vecRight.z;

		matrix.up.x = vecUp.x;
		matrix.up.y = vecUp.y;
		matrix.up.z = vecUp.z;

		matrix.at.x = vecAt.x;
		matrix.at.y = vecAt.y;
		matrix.at.z = vecAt.z;

		matrix.pos.x = vecPos.x;
		matrix.pos.y = vecPos.y;
		matrix.pos.z = vecPos.z;
	};

	CMatrix&		operator= (const CMatrix& cSource);

	inline CVector*	GetUp()
		{ return reinterpret_cast<CVector*>(&matrix.up); };

	inline CVector*	GetAt()
		{ return reinterpret_cast<CVector*>(&matrix.at); };

	inline CVector* GetPos()
		{ return reinterpret_cast<CVector*>(&matrix.pos); };

	friend inline CMatrix operator*(const CMatrix& Rot1, const CMatrix& Rot2)
	{ return CMatrix(	CVector(Rot1.matrix.right.x * Rot2.matrix.right.x + Rot1.matrix.right.y * Rot2.matrix.up.x + Rot1.matrix.right.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.right.x * Rot2.matrix.right.y + Rot1.matrix.right.y * Rot2.matrix.up.y + Rot1.matrix.right.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.right.x * Rot2.matrix.right.z + Rot1.matrix.right.y * Rot2.matrix.up.z + Rot1.matrix.right.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.up.x * Rot2.matrix.right.x + Rot1.matrix.up.y * Rot2.matrix.up.x + Rot1.matrix.up.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.up.x * Rot2.matrix.right.y + Rot1.matrix.up.y * Rot2.matrix.up.y + Rot1.matrix.up.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.up.x * Rot2.matrix.right.z + Rot1.matrix.up.y * Rot2.matrix.up.z + Rot1.matrix.up.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.at.x * Rot2.matrix.right.x + Rot1.matrix.at.y * Rot2.matrix.up.x + Rot1.matrix.at.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.at.x * Rot2.matrix.right.y + Rot1.matrix.at.y * Rot2.matrix.up.y + Rot1.matrix.at.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.at.x * Rot2.matrix.right.z + Rot1.matrix.at.y * Rot2.matrix.up.z + Rot1.matrix.at.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.pos.x * Rot2.matrix.right.x + Rot1.matrix.pos.y * Rot2.matrix.up.x + Rot1.matrix.pos.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.pos.x * Rot2.matrix.right.y + Rot1.matrix.pos.y * Rot2.matrix.up.y + Rot1.matrix.pos.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.pos.x * Rot2.matrix.right.z + Rot1.matrix.pos.y * Rot2.matrix.up.z + Rot1.matrix.pos.z * Rot2.matrix.at.z + Rot2.matrix.pos.z)); };

	friend inline CVector operator*(const CMatrix& matrix, const CVector& vec)
				{ return CVector(matrix.matrix.up.x * vec.y + matrix.matrix.right.x * vec.x + matrix.matrix.at.x * vec.z + matrix.matrix.pos.x,
								matrix.matrix.up.y * vec.y + matrix.matrix.right.y * vec.x + matrix.matrix.at.y * vec.z + matrix.matrix.pos.y,
								matrix.matrix.up.z * vec.y + matrix.matrix.right.z * vec.x + matrix.matrix.at.z * vec.z + matrix.matrix.pos.z); };

	inline void		SetRotateZ(float fAngle)
	{ SetRotateZOnly(fAngle); matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f; };

	void			RotateY(float fAngle);

	void			SetRotateOnly(float fAngleX, float fAngleY, float fAngleZ);

	void			UpdateRW();
	void			SetRotateXOnly(float fAngle);
	void			SetRotateYOnly(float fAngle);
	void			SetRotateZOnly(float fAngle);
};

class CWeapon
{
public:
	DWORD			m_nIndex;
	DWORD			unk1;
	DWORD			m_AmmoInClip;
	DWORD			m_Ammo;
	DWORD			unk2;
	BYTE			unk3;

	CWeapon(DWORD nIndex, DWORD nAmmo);

	void			FireProjectile(CEntity* pOwner, CVector* pVec, float fFloat);
};

class CPlaceable
{
private:
	void*				__vmt;

public:
	CMatrix				m_mCoords;
};

class CEntity : public CPlaceable
{
public:
    RpClump *m_pRwObject;
    char m_bType;
    char field_51;
    char field_52;
    char field_53;
    char field_54;
    char field_55;
    char field_56;
    char field_57;
    WORD m_wScanCode;
    WORD m_wRandomSeed;
    WORD m_wModelIndex;
    WORD m_wLevel;
    WORD field_60;
    WORD field_62;

	void		UpdateRwFrame();
};

class CPhysical : public CEntity
{
public:
    int field_64;
    int field_68;
    int field_6C;
    DWORD field_70;
    int m_dwLastTimeCollided;
    CVector m_vMoveSpeed;
    CVector m_vTurnSpeed;
    CVector m_vAcceleration;
    CVector m_vTurnAcceleration;
    CVector m_vForce;
    CVector m_vTorque;
    int m_fMass;
    float m_fTurnMass;
    DWORD m_fVelocityFrequency;
    int m_fAirResistance;
    int m_fElasticity;
    int m_fBuoyancy;
    CVector m_vTurnForce;
    void *m_pEntryInfoList;
    int m_pMovingList;
    char field_EC;
    char field_ED;
    char m_bNumCollidedEntities;
    char field_EF;
    CEntity *m_pCollidedEntity[6];
    DWORD field_108;
    DWORD m_fDamageIntensity;
    DWORD field_110;
    DWORD field_114;
    DWORD field_118;
    DWORD field_11C;
    WORD field_120;
    BYTE m_bFlags;
    BYTE field_123[1];
    BYTE m_bLevel;
};

#endif