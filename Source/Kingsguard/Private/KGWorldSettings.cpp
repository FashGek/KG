
/************************************************************************/
/* @author: xlt
 * @create: 2015/1/27 10:32
 
/************************************************************************/

#include "Kingsguard.h"
#include "KGWorldSettings.h"


AKGWorldSettings::AKGWorldSettings(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{

}

AKGWorldSettings* AKGWorldSettings::GetWorldSettings(UObject* WorldContextObject)
{
	UWorld* World = (WorldContextObject != NULL) ? WorldContextObject->GetWorld() : NULL;
	return (World == NULL) ? Cast<AKGWorldSettings>(World->GetWorldSettings()) : NULL;
}

void AKGWorldSettings::PostLoad()
{
	Super::PostLoad();
}

void AKGWorldSettings::PostInitProperties()
{
	Super::PostInitProperties();
}

void AKGWorldSettings::BeginPlay()
{
	Super::BeginPlay();
}

void AKGWorldSettings::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
