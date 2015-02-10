
#include "Kingsguard.h"
#include "KGWeapon_Blunderbuss.h"


AKGWeapon_Blunderbuss::AKGWeapon_Blunderbuss(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}
