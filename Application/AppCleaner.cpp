#include "AppCleaner.h"
#include <SceneManager.h>
#include <EngineGeneralTime.h>
#include <AssetManager.h>
#include <EngineScreen.h>
#include <InputManager.h>
#include <EngineGeneralSound.h>

void AppCleaner()
{
	SceneManager::DestroyMemory();

	AssetManager::DestroyMemory();

	EngineGeneralSound::DestroyMemory();
	InputManager::DestroyMemory();
	EngineScreen::DestroyMemory();
	EngineGeneralTime::DestroyMemory();

}