// Fill out your copyright notice in the Description page of Project Settings.

#include "Kingsguard.h"
#include "AssetRegistryModule.h"

class FKGModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override;
};


IMPLEMENT_PRIMARY_GAME_MODULE(FKGModule, Kingsguard, "Kingsguard");

DEFINE_LOG_CATEGORY(KG);

// init editor hooks
#if WITH_EDITOR

void FKGModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
}

#else

void FKGModule::StartupModule()
{

}

#endif

FCollisionResponseParams WorldResponseParams = []()
{
	FCollisionResponseParams Result(ECR_Ignore);
	Result.CollisionResponse.WorldStatic = ECR_Block;
	Result.CollisionResponse.WorldDynamic = ECR_Block;
	return Result;
}();


void UnregisterComponentTree(USceneComponent* Comp)
{
	if (Comp != NULL)
	{
		TArray<USceneComponent*> Childen;
		Comp->GetChildrenComponents(true, Childen);
		Comp->DetachFromParent();
		Comp->UnregisterComponent();
		for (USceneComponent* Child : Childen)
		{
			Child->UnregisterComponent();
		}
	}
}
