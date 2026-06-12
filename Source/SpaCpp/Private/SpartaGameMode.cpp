// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPawn.h"
#include "SpartaPlayerController.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = ASpartaPawn::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
}
