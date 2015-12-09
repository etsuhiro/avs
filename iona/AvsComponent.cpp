#include "AvsComponent.h"
#include "avs.h"

struct AvsComponent::Impl {
	avs::ScriptEngine scriptEngine;
};

void AvsComponent::Init(HWND hwnd)
{

}

void AvsComponent::Update()
{
	avs::RunningStatus stat = pImpl->scriptEngine.Run(16.f);
}

void AvsComponent::Final()
{

}

AvsComponent::AvsComponent()
{
	pImpl = new Impl();
}

AvsComponent::~AvsComponent()
{
	delete pImpl;
}

void AvsComponent::SetScript(const char* script)
{
	pImpl->scriptEngine.SetScript(script);
}
