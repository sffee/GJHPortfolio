#include "GJHPortfolio.h"
#include "Modules/ModuleManager.h"
#include "GameplayTag/GJHGameplayTag.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FGJHPortfolioModule, GJHPortfolio, "GJHPortfolio" );

void FGJHPortfolioModule::StartupModule()
{
}

void FGJHPortfolioModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
