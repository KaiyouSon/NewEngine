#pragma once
#include "Singleton.h"
#include <hidapi/hidapi.h>

template<typename T> class Singleton;

class JoyconInput : public Singleton<JoyconInput>
{
private:
	friend Singleton<JoyconInput>;

private:
	hid_device_info* device;

public:
	void Init();
	void Update();
	void Destroy();
};

