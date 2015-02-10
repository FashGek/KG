// Fill out your copyright notice in the Description page of Project Settings.

#ifndef __KINGSGUARD_H__
#define __KINGSGUARD_H__

#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(KG, Log, All);

#include "KGATypes.h"

#include "KGBaseGameMode.h"


#include "KGCharacter.h"


#include "KGHUD.h"
#include "KGHUDWidget.h"


#include "KGBasePlayerController.h"
#include "KGPlayerController.h"

#include "KGInventory.h"
#include "KGWeapon.h"
#include "KGPickup.h"
#include "KGGameSession.h"

#include "KGGameMode.h"
#include "KGWSGameMode.h"
#include "KGFXGameMode.h"


extern FCollisionResponseParams WorldResponseParams;


extern void UnregisterComponentTree(USceneComponent* Comp);

#endif
