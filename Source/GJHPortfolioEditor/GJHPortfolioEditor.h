#pragma once

#include "CoreMinimal.h"

class FGJHPortfolioEditorModule : public FDefaultModuleImpl
{
public:
	virtual void StartupModule() override;	
	virtual void ShutdownModule() override;
	
private:
	void RegisterCustomProperties();
};
