#ifndef __MOVINGTHINGS
#define __MOVINGTHINGS

enum eMovingThingType
{
	THING_FAN,
	THING_BLIMP
};

class CMovingThing
{
public:
	//CMovingThing*		m_pNext;
	//CMovingThing*		m_pPrev;
	bool				m_bFarAway;
	//CVector				m_vPos;
	CEntity*			m_pEntity;
	int					m_nRotPeriodX, m_nRotPeriodY, m_nRotPeriodZ;
	CVector				m_fRotSpeed;
	float				m_fMaxUpdateDist;
	eMovingThingType	m_eThingType;

	// Blimps
	CVector				m_vecCentralRotPoint;
	CVector				m_vecRadius;

public:
	inline bool			ShouldBeUpdated(unsigned int dwCount);
	void				Update();
};

class CMovingThingInfo
{
public:
	int					nRotPeriodX, nRotPeriodY, nRotPeriodZ;
	CVector				fDefAngle;
	CVector				vecRadius;
	float				fMaxDist;
	eMovingThingType	eThingType;
};

typedef std::map<int,CMovingThingInfo>		tMovingThingsInfo;

class CMovingThings
{
private:
	static std::vector<CMovingThing>			m_aMovingThings;
	static tMovingThingsInfo*					m_pMovingThingsInfo;

public:
	static void									Init();
	static void									PostInit();
	static void									PossiblyAddThisEntity(CEntity* pEntity);
	static void									Update();
};

#endif