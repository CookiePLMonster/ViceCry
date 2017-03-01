#include "StdAfx.h"
#include "Renderer.h"

#define _MEMORY_NO_CRT
#include "MemoryMgr.h"

unsigned int&			CRenderer::ms_nNoOfVisibleEntities = *(unsigned int*)0xA0D1E4;

std::vector<CEntity*>	CRenderer::aVisibleEntityPtrs;

void CRenderer::AddVisibleEntity(CEntity* pEntity)
{
	if ( ms_nNoOfVisibleEntities + 1 > aVisibleEntityPtrs.size() )
	{
		// Request a size change and update pointers
		static void*		pSavedPtr = nullptr;

		aVisibleEntityPtrs.resize(ms_nNoOfVisibleEntities + 1);
		if ( pSavedPtr != aVisibleEntityPtrs.data() )
		{
			if ( !pSavedPtr )
				aVisibleEntityPtrs.resize(INITIAL_RENDERER_SIZE);

			pSavedPtr = aVisibleEntityPtrs.data();
			Memory::VP::Patch<const void*>(0x4C9F83, pSavedPtr);
			Memory::VP::Patch<const void*>(0x4CA1BB, pSavedPtr);
			Memory::VP::Patch<const void*>(0x4CA203, pSavedPtr);
		}
	}

	aVisibleEntityPtrs[ms_nNoOfVisibleEntities++] = pEntity;
}