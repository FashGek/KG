
/*
@author: xlt
@create: 2015/1/27 10:00
*/

#pragma once

#include "KGWorldSettings.generated.h"

UCLASS()
class KINGSGUARD_API AKGWorldSettings : public AWorldSettings
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = World)
	static AKGWorldSettings* GetWorldSettings(UObject* WorldContextObject);

public:


	virtual void PostLoad() override;
	virtual void PostInitProperties() override;
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};
