#include "GJHPortfolioEditor.h"

#include "CustomProperties/GJHTableKeyCustomization.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FGJHPortfolioEditorModule, GJHPortfolioEditor);

void FGJHPortfolioEditorModule::StartupModule()
{
	FDefaultModuleImpl::StartupModule();

	RegisterCustomProperties();
}

void FGJHPortfolioEditorModule::ShutdownModule()
{
	FDefaultModuleImpl::ShutdownModule();
}

void FGJHPortfolioEditorModule::RegisterCustomProperties()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("GJHTableKey", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGJHTableKeyCustomization::MakeInstance));
}
