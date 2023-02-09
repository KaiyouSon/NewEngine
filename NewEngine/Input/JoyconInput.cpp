#include "JoyconInput.h"
#include <hidapi/hidapi.h>

#define JOYCON_L_PRODUCT_ID 8198
#define JOYCON_R_PRODUCT_ID 8199

void JoyconInput::Init()
{
	// 接続されているHIDデバイスの連結リストを取得。
	hid_device_info* device = hid_enumerate(0, 0);
}

void JoyconInput::Update()
{
	// プロダクトID等を指定して、HID deviceをopenする。そうすると、そのhidデバイスの情報が載ったhid_deviceが帰ってくる。
	hid_device* dev = hid_open(device->vendor_id, device->product_id, device->serial_number);
	// 今開いているデバイスのプロダクト名の取得。
	printf("\nproduct_id: %ls", device->product_string);
	// 次のデバイスへ。　　
	device = device->next;
}

void JoyconInput::Destroy()
{
	hid_free_enumeration(device);
}
