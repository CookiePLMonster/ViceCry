#ifndef __RENDERER
#define __RENDERER

#define INITIAL_RENDERER_SIZE		1000

class CRenderer
{
private:
	static unsigned int&			ms_nNoOfVisibleEntities;

	static std::vector<CEntity*>	aVisibleEntityPtrs;

public:
	static void						AddVisibleEntity(CEntity* pEntity);
};

#endif