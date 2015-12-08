#pragma once

#include "pao/Windows/Component.h"


class AvsComponent : public Component {
	class Impl;
	Impl* pImpl;
private:
	virtual void Init(HWND hwnd) override;
	virtual void Update() override;
	virtual void Final() override;
public:
	AvsComponent();
	~AvsComponent();
};
