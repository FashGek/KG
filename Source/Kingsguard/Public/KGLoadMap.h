/************************************************************************/
/* @author: xlt
 * @create: 2015/1/27 10:41
/************************************************************************/

#define KG_LOADMAP_DEFINITION() \
	virtual bool LoadMap(FWorldContext& WorldContext, FURL URL, class UPendingNetGame* Pending, FString& Error) \
	{\
		bool bResult = Super::LoadMap(WorldContext, URL, Pending, Error); \
		if (bResult && WorldContext.World() != NULL) \
		{\
			WorldContext.World()->GetWorldSettings()->NotifyBeginPlay(); \
		}\
		return bResult;\
	}