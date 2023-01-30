#include "RenderTexture.h"
#include "Camera.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include <memory>
using namespace std;

RenderTexture::RenderTexture() :
	pos(0), scale(1), rot(0), size(0),
	anchorPoint(0.5f), color(Color::white),
	vertexBuffer(new VertexBuffer<VertexPosUv>),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform2D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>)
{
}

RenderTexture::~RenderTexture()
{
	delete vertexBuffer;
	delete constantBufferTransform;
	delete constantBufferColor;
}

void RenderTexture::Initialize()
{
	HRESULT result;

	RenderBase* renderBase = RenderBase::GetInstance();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	CD3DX12_HEAP_PROPERTIES textureHeapProp =
		CD3DX12_HEAP_PROPERTIES(
			D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0);

	CD3DX12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			renderWindow->GetWindowSize().x,
			renderWindow->GetWindowSize().y,
			1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	// �e�N�X�`���̃T�C�Y���Z�b�g
	texture.size = { (float)textureResourceDesc.Width, (float)textureResourceDesc.Height };

	// �e�N�X�`���o�b�t�@�̐���
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texture.buffer));
	assert(SUCCEEDED(result));

	// �e�N�X�`���[��ԃN���A
	{
		const UINT pixelCount = renderWindow->GetWindowSize().x * renderWindow->GetWindowSize().y;
		const UINT rowPitch = sizeof(UINT) * renderWindow->GetWindowSize().x;
		const UINT depthPitch = rowPitch * renderWindow->GetWindowSize().y;
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}
		result = texture.buffer->WriteToSubresource(
			0,
			nullptr,
			img,
			rowPitch,
			depthPitch);
		assert(SUCCEEDED(result));
		delete img;
	}
	RenderBase::GetInstance()->CreateSrv(texture, textureResourceDesc);

	vertices.resize(4);
	vertices[0].uv = { 0.0f,1.0f };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[3].uv = { 1.0f,0.0f };
	vertexBuffer->Create(vertices);

	// �萔�o�b�t�@
	constantBufferTransform->Init();
	constantBufferColor->Init();

	//HRESULT result;

	// ���_�f�[�^
	//vertices.resize(4);
	//vertices[0] = { { -(size.x / 2), +(size.y / 2), 0.0f },{0.0f, 1.0f} };	//����
	//vertices[1] = { { -(size.x / 2), -(size.y / 2), 0.0f },{0.0f, 0.0f} };	//����
	//vertices[2] = { { +(size.x / 2), +(size.y / 2), 0.0f },{1.0f, 1.0f} };	//�E��
	//vertices[3] = { { +(size.x / 2), -(size.y / 2), 0.0f },{1.0f, 0.0f} };	//�E��
	//vertexBuffer->Create(vertices);

	//vertices.resize(4);
	//vertices[0].uv = { 0.0f,1.0f };
	//vertices[1].uv = { 0.0f,0.0f };
	//vertices[2].uv = { 1.0f,1.0f };
	//vertices[3].uv = { 1.0f,0.0f };
	//vertexBuffer->Create(vertices);

	////// �萔�o�b�t�@
	//constantBufferTransform->Init();
	//constantBufferColor->Init();


	//unique_ptr<Texture> tempTex = TextureBuffer::GetRenderTexture(
	//	{
	//		(float)renderWindow->GetWindowSize().x,
	//		(float)renderWindow->GetWindowSize().y,
	//	});
	////componentManager->GetComponent<Texture>()->SetTexture(*tempTex);

	//// RTV�p�f�X�N���v�^�q�[�v�ݒ�
	//D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc = {};
	//rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//rtvDescHeapDesc.NumDescriptors = 1;
	//// RTV�p�f�X�N���v�^�q�[�v���쐬
	//result = renderBase->GetDevice()->
	//	CreateDescriptorHeap(
	//		&rtvDescHeapDesc,
	//		IID_PPV_ARGS(&descHeapRTV));
	//assert(SUCCEEDED(result));

	//// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	//D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	//rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//// �f�X�N���v�^�q�[�v��RTV�쐬
	//ID3D12Resource* a = componentManager->GetComponent<Texture>()->buffer.Get();
	//auto b = a->GetDesc();

	//renderBase->GetDevice()->CreateRenderTargetView(
	//	componentManager->GetComponent<Texture>()->buffer.Get(),
	//	&rtvDesc,
	//	descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	//// �[�x�o�b�t�@���\�[�X�̐ݒ�
	//CD3DX12_RESOURCE_DESC depthRecDesc =
	//	CD3DX12_RESOURCE_DESC::Tex2D(
	//		DXGI_FORMAT_D32_FLOAT,
	//		(UINT64)renderWindow->GetWindowSize().x,
	//		(UINT)renderWindow->GetWindowSize().y,
	//		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	//// �[�x�o�b�t�@�̐���
	//CD3DX12_HEAP_PROPERTIES depthHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//CD3DX12_CLEAR_VALUE depthClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	//result = renderBase->GetDevice()->CreateCommittedResource(
	//	&depthHeapProperties,
	//	D3D12_HEAP_FLAG_NONE,
	//	&depthRecDesc,
	//	D3D12_RESOURCE_STATE_DEPTH_WRITE,
	//	&depthClearValue,
	//	IID_PPV_ARGS(&depthBuff));
	//assert(SUCCEEDED(result));

	//// DSV�p�f�X�N���v�^�q�[�v���쐬
	//D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	//dsvHeapDesc.NumDescriptors = 1;
	//dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	//result = renderBase->GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	//assert(SUCCEEDED(result));

	//// �f�X�N���v�^�q�[�v�̍쐬
	//D3D12_DEPTH_STENCIL_VIEW_DESC dsvViewDesc = {};
	//dsvViewDesc.Format = DXGI_FORMAT_D32_FLOAT;	// �[�x�n�t�H�[�}�b�g
	//dsvViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//renderBase->GetDevice()->CreateDepthStencilView
	//(
	//	depthBuff.Get(),
	//	&dsvViewDesc,
	//	descHeapDSV->GetCPUDescriptorHandleForHeapStart()
	//);
}
void RenderTexture::Update()
{
	transform.pos = pos;
	transform.scale = { scale.x,scale.y,1 };
	transform.rot = { 0,0,rot };
	transform.Update();

	// �萔�o�b�t�@�ɓ]��
	constantBufferTransform->constantBufferMap->mat =
		transform.worldMat *
		Camera::current.GetOrthoGrphicProjectionMat();

	// �F�]��
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;

	// ���_�o�b�t�@�[�ɒ��_��]��
	TransferTexturePos();
}

void RenderTexture::PreDrawScene()
{
	//RenderBase* renderBase = RenderBase::GetInstance().get();
	//RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	//CD3DX12_RESOURCE_BARRIER resourceBarrier;
	//resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
	//	componentManager->GetComponent<Texture>()->buffer.Get(),
	//	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
	//	D3D12_RESOURCE_STATE_RENDER_TARGET);

	////���\�[�X�o���A��ύX�i�V�F�[�_�[���\�[�X -> �`��\�j
	//renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);

	//// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//// �[�x�X�e���V���r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//// �����_�[�^�[�Q�b�g���Z�b�g
	//renderBase->GetCommandList()->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//// �r���[�|�[�g�̐ݒ�
	//CD3DX12_VIEWPORT viewport = CD3DX12_VIEWPORT(
	//	0.0f, 0.0f,
	//	renderWindow->GetWindowSize().x,
	//	renderWindow->GetWindowSize().y);

	//renderBase->GetCommandList()->RSSetViewports(1, &viewport);
	//// �V�U�����O��`�̐ݒ�
	//CD3DX12_RECT scissorRect = CD3DX12_RECT(
	//	0, 0,
	//	renderWindow->GetWindowSize().x,
	//	renderWindow->GetWindowSize().y);

	//renderBase->GetCommandList()->RSSetScissorRects(1, &scissorRect);

	//// �S��ʃN���A
	//renderBase->GetCommandList()->ClearRenderTargetView(rtvH, RenderBase::clearColor, 0, nullptr);
	//// �[�x�o�b�t�@�̃N���A
	//renderBase->GetCommandList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
void RenderTexture::PostDrawScene()
{
	//RenderBase* renderBase = RenderBase::GetInstance().get();

	//// ���\�[�X�o���A��ύX�i�`��\ -> �V�F�[�_�[���\�[�X�j
	//CD3DX12_RESOURCE_BARRIER resourceBarrier;
	//resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
	//	componentManager->GetComponent<Texture>()->buffer.Get(),
	//	D3D12_RESOURCE_STATE_RENDER_TARGET,
	//	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
}
void RenderTexture::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetPipelineState(renderBase->GetSpritePipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	// SRV�q�[�v�̐ݒ�R�}���h
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		renderBase->GetSpriteRootSignature()->GetRootDescriptorTableIndex(), texture.GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((unsigned short)vertices.size(), 1, 0, 0);
}

void RenderTexture::SetCutPosAndSize(const Vec2& cutPos, const Vec2& cutSize)
{
	//float texLeft = cutPos.x / componentManager->GetComponent<Texture>()->GetTextureSize().x;
	//float texRight = (cutPos.x + cutSize.x) / componentManager->GetComponent<Texture>()->GetTextureSize().x;
	//float texUp = cutPos.y / componentManager->GetComponent<Texture>()->GetTextureSize().y;
	//float texDown = (cutPos.y + cutSize.y) / componentManager->GetComponent<Texture>()->GetTextureSize().y;

	//vertices[0].uv = { texLeft , texDown };	// ����
	//vertices[1].uv = { texLeft ,   texUp };	// ����
	//vertices[2].uv = { texRight ,texDown }; // �E��
	//vertices[3].uv = { texRight ,  texUp }; // �E��

	//vertexBuffer->TransferToBuffer(vertices);
}

void RenderTexture::TransferTexturePos()
{
	// �V�����̃T�C�Y
	float width = texture.size.x;
	float height = texture.size.y;

	// ���݂̃T�C�Y
	float width2 = vertices[0].pos.x - vertices[2].pos.x;
	float height2 = vertices[1].pos.x - vertices[3].pos.x;

	if (width != fabsf(width2) || width != fabsf(height2))
	{
		vertices[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //����
		vertices[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //����
		vertices[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //�E��
		vertices[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //�E��

		vertexBuffer->TransferToBuffer(vertices);
	}
}