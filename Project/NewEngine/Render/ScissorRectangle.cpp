#include "ScissorRectangle.h"
#include "RenderBase.h"
#include "RenderWindow.h"

void ScissorRectangle::Update()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	//------------------------ �V�U�[��`�̐ݒ�R�}���h ------------------------//
	// �V�U�[��`
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + (LONG)renderWindow->GetWindowSize().x; // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + (LONG)renderWindow->GetWindowSize().y; // �؂蔲�����W��
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	renderBase->GetCommandList()->RSSetScissorRects(1, &scissorRect);
}
