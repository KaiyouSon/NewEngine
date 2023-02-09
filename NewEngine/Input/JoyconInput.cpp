#include "JoyconInput.h"
#include <hidapi/hidapi.h>

#define JOYCON_L_PRODUCT_ID 8198
#define JOYCON_R_PRODUCT_ID 8199

void JoyconInput::Init()
{
	// �ڑ�����Ă���HID�f�o�C�X�̘A�����X�g���擾�B
	hid_device_info* device = hid_enumerate(0, 0);
}

void JoyconInput::Update()
{
	// �v���_�N�gID�����w�肵�āAHID device��open����B��������ƁA����hid�f�o�C�X�̏�񂪍ڂ���hid_device���A���Ă���B
	hid_device* dev = hid_open(device->vendor_id, device->product_id, device->serial_number);
	// ���J���Ă���f�o�C�X�̃v���_�N�g���̎擾�B
	printf("\nproduct_id: %ls", device->product_string);
	// ���̃f�o�C�X�ցB�@�@
	device = device->next;
}

void JoyconInput::Destroy()
{
	hid_free_enumeration(device);
}
