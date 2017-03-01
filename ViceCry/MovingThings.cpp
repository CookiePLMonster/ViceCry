#include "StdAfx.h"
#include "MovingThings.h"

#define DEG_TO_RAD		(M_PI/180.0)

std::vector<CMovingThing>			CMovingThings::m_aMovingThings;
tMovingThingsInfo*					CMovingThings::m_pMovingThingsInfo;

int&			snTimeInMilliseconds = *(int*)0x974B2C;
unsigned int&	dwFrameCounter = *(unsigned int*)0xA0D898;
CVector&		vecCamPos = *(CVector*)0x7E46B8;

// Wrapper
WRAPPER void* GetModelInfo(const char* pName, int* nModelID){ WRAPARG(pName); WRAPARG(nModelID); EAXJMP(0x55F7D0); }

void CMovingThings::Init()
{
	m_aMovingThings.clear();
	
	if ( FILE* hFile = CFileMgr::OpenFile("data\\moving_things.dat", "r") )
	{
		m_pMovingThingsInfo = new tMovingThingsInfo;

		while ( const char* pLine = CFileLoader::LoadLine(hFile) )
		{
			if ( pLine[0] && pLine[0] != '#' )
			{
				char				cModelName[64];
				CMovingThingInfo	sNewThing;

				if ( sscanf(pLine, "%s %d %f %f %f %f %f %f %f", cModelName, &sNewThing.nRotPeriodX, &sNewThing.fDefAngle.x, &sNewThing.fDefAngle.y, &sNewThing.fDefAngle.z, &sNewThing.vecRadius.x, &sNewThing.vecRadius.y, &sNewThing.vecRadius.z, &sNewThing.fMaxDist) == 9 )
				{
					int			nModelID;

					sNewThing.eThingType = THING_BLIMP;
					GetModelInfo(cModelName, &nModelID);
					(*m_pMovingThingsInfo)[nModelID] = sNewThing;
				}
				else if ( sscanf(pLine, "%s %d %d %d %f %f %f %f", cModelName, &sNewThing.nRotPeriodX, &sNewThing.nRotPeriodY, &sNewThing.nRotPeriodZ, &sNewThing.fDefAngle.x, &sNewThing.fDefAngle.y, &sNewThing.fDefAngle.z, &sNewThing.fMaxDist) == 8 )
				{
					int			nModelID;

					sNewThing.eThingType = THING_FAN;
					GetModelInfo(cModelName, &nModelID);
					(*m_pMovingThingsInfo)[nModelID] = sNewThing;
				}
			}
		}

		CFileMgr::CloseFile(hFile);
	}
}

void CMovingThings::PossiblyAddThisEntity(CEntity* pEntity)
{
	auto it = m_pMovingThingsInfo->find(pEntity->m_wModelIndex);
	if ( it != m_pMovingThingsInfo->end() )
	{
		CMovingThing sNewThing;

		sNewThing.m_pEntity = pEntity;
		sNewThing.m_bFarAway = false;
		//sNewThing.m_vPos = *pEntity->m_mCoords.GetPos();

		sNewThing.m_nRotPeriodX = it->second.nRotPeriodX;
		sNewThing.m_nRotPeriodY = it->second.nRotPeriodY;
		sNewThing.m_nRotPeriodZ = it->second.nRotPeriodZ;

		if ( it->second.nRotPeriodX )
			sNewThing.m_fRotSpeed.x = static_cast<float>(2*M_PI) / sNewThing.m_nRotPeriodX;
		else
			sNewThing.m_fRotSpeed.x = it->second.fDefAngle.x * static_cast<float>(DEG_TO_RAD);
		if ( it->second.nRotPeriodY )
			sNewThing.m_fRotSpeed.y = static_cast<float>(2*M_PI) / sNewThing.m_nRotPeriodY;
		else
			sNewThing.m_fRotSpeed.y = it->second.fDefAngle.y * static_cast<float>(DEG_TO_RAD);
		if ( it->second.nRotPeriodZ )
			sNewThing.m_fRotSpeed.z = static_cast<float>(2*M_PI) / sNewThing.m_nRotPeriodZ;
		else
			sNewThing.m_fRotSpeed.z = it->second.fDefAngle.z * static_cast<float>(DEG_TO_RAD);

		sNewThing.m_fMaxUpdateDist = it->second.fMaxDist * it->second.fMaxDist;

		// Blimp stuff
		sNewThing.m_vecCentralRotPoint = it->second.fDefAngle;
		sNewThing.m_vecRadius = it->second.vecRadius;

		sNewThing.m_eThingType = it->second.eThingType;

		m_aMovingThings.push_back(sNewThing);
	}
}

void CMovingThings::PostInit()
{
	delete m_pMovingThingsInfo;
	m_aMovingThings.shrink_to_fit();
}

void CMovingThings::Update()
{
	// Update ALL [s]close[/s] things
	int		j = 0;
	for ( auto it = m_aMovingThings.begin(); it != m_aMovingThings.end(); it++ )
	{
		if ( it->ShouldBeUpdated(j++) )
			it->Update();
	}
}

void CMovingThing::Update()
{
	switch ( m_eThingType )
	{
	case THING_FAN:
		{
			CVector		sRotations(	m_nRotPeriodX ? m_fRotSpeed.x * ( snTimeInMilliseconds % m_nRotPeriodX ) : m_fRotSpeed.x,
									m_nRotPeriodY ? m_fRotSpeed.y * ( snTimeInMilliseconds % m_nRotPeriodY ) : m_fRotSpeed.y,
									m_nRotPeriodZ ? m_fRotSpeed.z * ( snTimeInMilliseconds % m_nRotPeriodZ ) : m_fRotSpeed.z);

			m_pEntity->m_mCoords.SetRotateOnly(sRotations.x, sRotations.y, sRotations.z);
			break;
		}
	case THING_BLIMP:
		{
			float		fRotAngle = m_fRotSpeed.x * ( snTimeInMilliseconds % m_nRotPeriodX );
			CVector*	pBlimpPos = m_pEntity->m_mCoords.GetPos();

			m_pEntity->m_mCoords.SetRotateZOnly(fRotAngle);
			pBlimpPos->x = -cos(-fRotAngle) * m_vecRadius.x + m_vecCentralRotPoint.x;
			pBlimpPos->y = sin(-fRotAngle) * m_vecRadius.y + m_vecCentralRotPoint.y;
			pBlimpPos->z = sin(-fRotAngle) * m_vecRadius.z + m_vecCentralRotPoint.z;
			break;
		}
	}

	m_pEntity->m_mCoords.UpdateRW();
	m_pEntity->UpdateRwFrame();

	CVector		vecDistFromCam(m_pEntity->m_mCoords.GetPos()->x - vecCamPos.x, m_pEntity->m_mCoords.GetPos()->y - vecCamPos.y);

	if ( vecDistFromCam.x * vecDistFromCam.x + vecDistFromCam.y * vecDistFromCam.y > m_fMaxUpdateDist )
	{
		if ( !m_bFarAway )
			m_bFarAway = true;
	}
	else
	{
		if ( m_bFarAway )
			m_bFarAway = false;
	}
}

bool CMovingThing::ShouldBeUpdated(unsigned int dwCount)
{
	if ( !m_bFarAway )
		return true;

	return (dwCount & 7) == (dwFrameCounter & 7);
}