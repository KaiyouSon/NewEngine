#include "InspectorLayer.h"
#include "GuiManager.h"
#include "ComponentManager.h"
#include "Scene.h"
#include "InputManager.h"
using namespace std;

void InspectorLayer::Initialize()
{
	size = { 480,WIN_HEIGHT };
	pos = { WIN_WIDTH - size.x,0 };

	isChangeObjName = false;
}
void InspectorLayer::Update()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

	ImGui::Begin("Inspector", nullptr, window_flags);

	//ShowMenuBar();

	// �^�u
	if (ImGui::BeginTabBar("Data Tab", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Component"))
		{
			ShowComponent();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Collider"))
		{
			ShowCollider();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	//ShowObjData();
	//ImGui::Columns(2, 0, false);
	ImGui::End();
}

void InspectorLayer::ShowMenuBar()
{
	//for (auto tmpObjectList : ObjectManager::GetInstance()->GetObjectList())
	//{
	//	if (ImGui::BeginMenuBar())
	//	{
	//		if (ImGui::BeginMenu("Menu"))
	//		{
	//			if (ImGui::BeginMenu("Add Component"))
	//			{
	//				ImGui::EndMenu();
	//			}
	//			ImGui::EndMenu();
	//		}
	//		ImGui::EndMenuBar();
	//	}
	//}
}
void InspectorLayer::ShowComponent()
{
	// �C�e���[�^�[���擾
	decltype(sceneManager->list)::iterator it =
		next(sceneManager->list.begin(), sceneManager->GetCurrent());

	// �I�u�W�F�N�g�}�l�[�W���[�̎擾
	const auto& objManager = it->get()->GetObjManager();

	// �I�u�W�F�N�g���X�g�̎擾
	const auto& objList = objManager->list;

	// �I�u�W�F�N�g�ꗗ�̕\��
	for (const auto& temp : objList)
	{
		// �I�������I�u�W�F�N�g�̃f�[�^��Inspector�E�B���h�E�ɕ\��
		if (temp->GetisShowDataToInspector())
		{
			// �\���t���O
			ShowisShowFlag(*temp); ImGui::SameLine();

			// ���O
			ShowNameString(*temp);

			// �X�v���C�g�̃��C���[
			ShowDrawLayer(*temp);

			// �^�O
			ShowTagString(*temp);	ImGui::Separator();

			// �g�����X�t�H�[���R���|�l���g
			ShowTransform(*temp);	ImGui::Separator();

			// �e�N�X�`���[�R���|�l���g
			ShowTexture(*temp);		ImGui::Separator();

			// �p�[�e�B�N���G�f�B�^�[
			//ShowParticleEditor(*temp);	ImGui::Separator();

			// �R���|�l���g�ǉ�
			ShowAddComponent(*temp);

		}
	}
}

void InspectorLayer::ShowisShowFlag(GameObject& gameObject)
{
	bool isShow = gameObject.GetisShow();
	ImGui::Checkbox(" ", &isShow);
	gameObject.SetisShow(isShow);
}
void InspectorLayer::ShowNameString(GameObject& gameObject)
{
	char str[25] = {};
	for (int i = 0; i < gameObject.GetName().size(); i++)
	{
		str[i] = gameObject.GetName().c_str()[i];
	}
	if (ImGui::InputText("  ", str, 25) == true)
	{
		isChangeObjName = true;
	}
	gameObject.SetName(str);
}
void InspectorLayer::ShowTagString(GameObject& gameObject)
{
	ImGui::Text("Tag"); ImGui::SameLine();
	ImGui::Text(gameObject.componentManager->GetComponent<Tag>()->GetTag().c_str());
	ImGui::SameLine();

	if (ImGui::Button("   ", ImVec2(14, 14)))
	{
		ImGui::OpenPopup("Tag List");
	}

	if (ImGui::BeginPopup("Tag List"))
	{
		for (int i = 0; i < tagList->GetList().size(); i++)
		{
			if (ImGui::MenuItem(tagList->GetTag(i).c_str()))
			{
				gameObject.componentManager->GetComponent<Tag>()->SetTag(tagList->GetTag(i));
			}
			ImGui::Separator();
		}
		ImGui::EndPopup();
	}
}
void InspectorLayer::ShowDrawLayer(GameObject& gameObject)
{
	if (gameObject.GetObjectType() != SpriteObj) return;

	bool flag = gameObject.GetLayer();
	ImGui::Checkbox("DrawLayer", &flag);
	gameObject.SetLayer(flag);

	ImGui::SameLine();

	string str = flag ? "Forward" : "Back";
	ImGui::Text(str.c_str());
}
void InspectorLayer::ShowTransform(GameObject& gameObject)
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		// ��������u���Ƃ�
		float tmpPos[3] =
		{
			gameObject.componentManager->GetComponent<Transform>()->pos.x,
			gameObject.componentManager->GetComponent<Transform>()->pos.y,
			gameObject.componentManager->GetComponent<Transform>()->pos.z,
		};
		float tmpRot[3] =
		{
			gameObject.componentManager->GetComponent<Transform>()->rot.x,
			gameObject.componentManager->GetComponent<Transform>()->rot.y,
			gameObject.componentManager->GetComponent<Transform>()->rot.z,
		};
		float tmpScale[3] =
		{
			gameObject.componentManager->GetComponent<Transform>()->scale.x,
			gameObject.componentManager->GetComponent<Transform>()->scale.y,
			gameObject.componentManager->GetComponent<Transform>()->scale.z,
		};

		ImGui::DragFloat3("Postion", tmpPos, 0.05);
		ImGui::DragFloat3("Rotation", tmpRot, 0.2);
		ImGui::DragFloat3("Scale", tmpScale, 0.01);

		gameObject.componentManager->GetComponent<Transform>()->pos.x = tmpPos[0];
		gameObject.componentManager->GetComponent<Transform>()->pos.y = tmpPos[1];
		gameObject.componentManager->GetComponent<Transform>()->pos.z = tmpPos[2];

		gameObject.componentManager->GetComponent<Transform>()->rot.x = tmpRot[0];
		gameObject.componentManager->GetComponent<Transform>()->rot.y = tmpRot[1];
		gameObject.componentManager->GetComponent<Transform>()->rot.z = tmpRot[2];

		gameObject.componentManager->GetComponent<Transform>()->scale.x = tmpScale[0];
		gameObject.componentManager->GetComponent<Transform>()->scale.y = tmpScale[1];
		gameObject.componentManager->GetComponent<Transform>()->scale.z = tmpScale[2];

		// �r���{�[�h
		bool tempFlag = gameObject.componentManager->GetComponent<Transform>()->GetisUseBillboard();
		ImGui::Checkbox("Use BillBoard", &tempFlag);
		gameObject.componentManager->GetComponent<Transform>()->SetisUseBillboard(tempFlag);

		if (tempFlag == true)
		{
			int tempType = gameObject.componentManager->GetComponent<Transform>()->GetBillboardType();
			ImGui::RadioButton("X Axis", &tempType, 0); ImGui::SameLine();
			ImGui::RadioButton("Y Axis", &tempType, 1); ImGui::SameLine();
			ImGui::RadioButton("Z Axis", &tempType, 2); ImGui::SameLine();
			ImGui::RadioButton("All Axis", &tempType, 3);
			gameObject.componentManager->GetComponent<Transform>()->SetBillboardType(tempType);
		}
	}
}
void InspectorLayer::ShowTexture(GameObject& gameObject)
{
	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Text("Texture TagName");
		ImGui::SameLine();
		ImGui::Text(gameObject.componentManager->GetComponent<Texture>()->GetTag().c_str());
		ImGui::SameLine();

		if (ImGui::Button("    ", ImVec2(14, 14)))
		{
			ImGui::OpenPopup("Game Texture List");
		}

		if (ImGui::BeginPopup("Game Texture List"))
		{
			for (const auto& temp : gameTextureList->list)
			{
				if (ImGui::MenuItem(temp->GetTag().c_str()))
				{
					gameObject.componentManager->GetComponent<Texture>()->
						SetTexture(*temp.get());
				}
				ImGui::Separator();
			}
			ImGui::EndPopup();
		}

		ImGui::Text("Texture Color"); ImGui::SameLine();
		float tmpColor[4] =
		{
			gameObject.GetColor().r / 255,
			gameObject.GetColor().g / 255,
			gameObject.GetColor().b / 255,
			gameObject.GetColor().a / 255
		};
		ImGui::ColorEdit4("Color", tmpColor);
		gameObject.SetColor(Color(
			tmpColor[0] * 255,
			tmpColor[1] * 255,
			tmpColor[2] * 255,
			tmpColor[3] * 255));
	}
}
void InspectorLayer::ShowAddComponent(GameObject& gameObject)
{
	ImGui::Columns(3, 0, false);
	ImGui::NextColumn();
	ImGui::Button("Add Component", { 144,32 });
	ImGui::Columns(1);
}

// �R���C�_�[
void InspectorLayer::ShowCollider()
{
	// �C�e���[�^�[���擾
	decltype(sceneManager->list)::iterator it =
		next(sceneManager->list.begin(), sceneManager->GetCurrent());

	// �I�u�W�F�N�g�}�l�[�W���[�̎擾
	const auto& objManager = it->get()->GetObjManager();

	// �I�u�W�F�N�g���X�g�̎擾
	const list<std::unique_ptr<GameObject>>& objList = objManager->list;

	// �I�𒆂̃I�u�W�F�N�g�̖��O
	//static string currentObjName;
	static int currentIndex = -1;

	for (const auto& tempObj : objList)
	{
		// �I�������I�u�W�F�N�g�̃f�[�^��Inspector�E�B���h�E�ɕ\��
		if (tempObj->GetisShowDataToInspector())
		{
			for (const auto& tempCollider : tempObj->colliderManager->list)
			{
				switch (tempCollider->GetColliderType())
				{
				case SphereShape:
					ShowSphereCollider(*tempCollider, currentIndex);
					ImGui::Separator();
					break;
				}
				// �폜
				if (tempCollider->GetisDestroy() == true)
				{
					tempObj->colliderManager->DestroyCollider(*tempCollider);
					break;
				}
			}
			ShowAddCollider(*tempObj);

		}
	}
}

void InspectorLayer::ShowSphereCollider(Collider& collider, int& index)
{
	string indexStr = to_string(collider.GetColliderIndex());

	if (ImGui::CollapsingHeader(("Collider " + indexStr).c_str()))
	{
		ImGui::Text("Sphere Collider");
		float* tempCenterPos[3] =
		{
			&collider.GetSphereCollider()->posOffSet.x,
			&collider.GetSphereCollider()->posOffSet.y,
			&collider.GetSphereCollider()->posOffSet.z,
		};
		ImGui::DragFloat3(("Center Postion " + indexStr).c_str(), *tempCenterPos, 0.05);
		collider.GetSphereCollider()->posOffSet.x = *tempCenterPos[0];
		collider.GetSphereCollider()->posOffSet.y = *tempCenterPos[1];
		collider.GetSphereCollider()->posOffSet.z = *tempCenterPos[2];

		float* tempRadius = &collider.GetSphereCollider()->radius;
		ImGui::DragFloat(("Radius " + indexStr).c_str(), tempRadius, 0.05);
		collider.GetSphereCollider()->radius = *tempRadius;

		// �폜
		if (ImGui::Button("Destroy\nCollider", { 80, 48 }))
		{
			collider.SetisDestroy(true);
		}
	}
}
void InspectorLayer::ShowAddCollider(GameObject& gameObject)
{
	ImGui::Columns(3, 0, false);
	ImGui::NextColumn();
	if (ImGui::Button("Add Collider", { 144,32 }))
	{
		ImGui::OpenPopup("Collider Context");
	}

	if (ImGui::BeginPopup("Collider Context"))
	{
		if (ImGui::MenuItem("Sphere"))
		{
			gameObject.colliderManager->Add(move(make_unique<SphereCollider>()));
		}
		ImGui::Separator();

		ImGui::EndPopup();
	}

	ImGui::Columns(1);
}

// �p�[�e�B�N���G�f�B�^�[
void InspectorLayer::ShowParticleEditor(GameObject& gameObject)
{
	if (gameObject.GetObjectType() != ParticleEmitter) return;

	Emitter* emitter = dynamic_cast<Emitter*>(&gameObject);

	if (ImGui::CollapsingHeader("Particle Editor"))
	{
		ShowParticleVec(*emitter);		ImGui::Separator();	// �x�N�g��
		ShowParticleScale(*emitter);	ImGui::Separator();	// �X�P�[��
		ShowParticleColor(*emitter);	ImGui::Separator();	// �F
	}
}

void InspectorLayer::ShowParticleVec(Emitter& emitter)
{
	if (ImGui::TreeNode("Vec"))
	{
		ImGui::Checkbox("Random Vec", &emitter.initialParticle.vec.srp.isRandom);
		if (emitter.initialParticle.vec.srp.isRandom == false)
		{
			float tempVec[3] =
			{
				emitter.initialParticle.vec.start.x,
				emitter.initialParticle.vec.start.y,
				emitter.initialParticle.vec.start.z,
			};
			ImGui::DragFloat3("Vec", tempVec, 0.05, -1, 1);
			emitter.initialParticle.vec.start =
			{
				tempVec[0],
				tempVec[1],
				tempVec[2],
			};
		}
		else
		{
			float tempRandomValue[2][3] =
			{
				emitter.initialParticle.vec.srp.minValue.x,
				emitter.initialParticle.vec.srp.minValue.y,
				emitter.initialParticle.vec.srp.minValue.z,
				emitter.initialParticle.vec.srp.maxValue.x,
				emitter.initialParticle.vec.srp.maxValue.y,
				emitter.initialParticle.vec.srp.maxValue.z,
			};
			ImGui::DragFloat3("Random Start Vec Min Value", *tempRandomValue + 0, -1, 1);
			ImGui::DragFloat3("Random Start Vec Max Value", *tempRandomValue + 3, -1, 1);

			emitter.initialParticle.vec.srp.minValue =
			{
				tempRandomValue[0][0],
				tempRandomValue[0][1],
				tempRandomValue[0][2],
			};
			emitter.initialParticle.vec.srp.maxValue =
			{
				tempRandomValue[1][0],
				tempRandomValue[1][1],
				tempRandomValue[1][2],
			};
		}
		ImGui::TreePop();
	}
}
void InspectorLayer::ShowParticleScale(Emitter& emitter)
{
	static bool isRandomStartScale = false;
	static bool isRandomEndScale = false;

	if (ImGui::TreeNode("Scale"))
	{
		// �X�^�[�g�X�P�[��
		ImGui::Checkbox("Random Start Scale", &emitter.initialParticle.scale.srp.isRandom);
		if (emitter.initialParticle.scale.srp.isRandom == false)
		{
			ImGui::DragFloat("Start Scale", &emitter.initialParticle.scale.start, 0.1, 0, 100);
		}
		else
		{
			float tempStartScale[2] =
			{
				emitter.initialParticle.scale.srp.minValue,
				emitter.initialParticle.scale.srp.maxValue,
			};

			ImGui::DragFloat2("Random Start Scale Value", tempStartScale, 0.1, 0, 100);
			emitter.initialParticle.scale.srp.minValue = tempStartScale[0];
			emitter.initialParticle.scale.srp.maxValue = tempStartScale[1];
		}

		// �G���h�X�P�[��
		ImGui::Checkbox("Random End Scale", &isRandomEndScale);
		if (isRandomEndScale == false)
		{
			ImGui::DragFloat("End Scale", &emitter.initialParticle.scale.end, 0.1, 0, 100);
		}
		else
		{
			float tempStartScale[2] =
			{
				emitter.initialParticle.scale.erp.minValue,
				emitter.initialParticle.scale.erp.maxValue,
			};

			ImGui::DragFloat2("Random End Scale Value", tempStartScale, 0.1, 0, 100);
			emitter.initialParticle.scale.erp.minValue = tempStartScale[0];
			emitter.initialParticle.scale.erp.maxValue = tempStartScale[1];
		}

		ImGui::TreePop();
	}
}
void InspectorLayer::ShowParticleColor(Emitter& emitter)
{
	if (ImGui::TreeNode("Color"))
	{
		// �X�^�[�g�F
		ImGui::Checkbox("Random Start Color", &emitter.initialParticle.color.srp.isRandom);
		if (emitter.initialParticle.color.srp.isRandom == false)
		{
			float tempStartColor[4] =
			{
				emitter.initialParticle.color.start.r,
				emitter.initialParticle.color.start.g,
				emitter.initialParticle.color.start.b,
				emitter.initialParticle.color.start.a,
			};
			ImGui::ColorEdit4("Start Color", tempStartColor);
			emitter.initialParticle.color.start.r = tempStartColor[0];
			emitter.initialParticle.color.start.g = tempStartColor[1];
			emitter.initialParticle.color.start.b = tempStartColor[2];
			emitter.initialParticle.color.start.a = tempStartColor[3];
		}
		else
		{
			float tempRandomValue[2][3] =
			{
				emitter.initialParticle.color.srp.minValue.r,
				emitter.initialParticle.color.srp.minValue.g,
				emitter.initialParticle.color.srp.minValue.b,
				emitter.initialParticle.color.srp.maxValue.r,
				emitter.initialParticle.color.srp.maxValue.g,
				emitter.initialParticle.color.srp.maxValue.b,
			};
			ImGui::ColorEdit3("Random Start Color Min Value", *tempRandomValue + 0);
			ImGui::ColorEdit3("Random Start Color Max Value", *tempRandomValue + 3);

			emitter.initialParticle.color.srp.minValue =
			{
				tempRandomValue[0][0],
				tempRandomValue[0][1],
				tempRandomValue[0][2],
			};
			emitter.initialParticle.color.srp.maxValue =
			{
				tempRandomValue[1][0],
				tempRandomValue[1][1],
				tempRandomValue[1][2],
			};
		}
		ImGui::Separator();
		// �G���h�F
		ImGui::Checkbox("Random End Color", &emitter.initialParticle.color.erp.isRandom);
		if (emitter.initialParticle.color.erp.isRandom == false)
		{
			float tempEndColor[4] =
			{
				emitter.initialParticle.color.end.r,
				emitter.initialParticle.color.end.g,
				emitter.initialParticle.color.end.b,
				emitter.initialParticle.color.end.a,
			};
			ImGui::ColorEdit4("End Color", tempEndColor);
			emitter.initialParticle.color.end.r = tempEndColor[0];
			emitter.initialParticle.color.end.g = tempEndColor[1];
			emitter.initialParticle.color.end.b = tempEndColor[2];
			emitter.initialParticle.color.end.a = tempEndColor[3];

		}
		else
		{
			float tempRandomValue[2][3] =
			{
				emitter.initialParticle.color.erp.minValue.r,
				emitter.initialParticle.color.erp.minValue.g,
				emitter.initialParticle.color.erp.minValue.b,
				emitter.initialParticle.color.erp.maxValue.r,
				emitter.initialParticle.color.erp.maxValue.g,
				emitter.initialParticle.color.erp.maxValue.b,
			};
			ImGui::ColorEdit3("Random End Color Min Value", *tempRandomValue + 0);
			ImGui::ColorEdit3("Random End Color Max Value", *tempRandomValue + 3);

			emitter.initialParticle.color.erp.minValue =
			{
				tempRandomValue[0][0],
				tempRandomValue[0][1],
				tempRandomValue[0][2],
			};
			emitter.initialParticle.color.erp.maxValue =
			{
				tempRandomValue[1][0],
				tempRandomValue[1][1],
				tempRandomValue[1][2],
			};
		}
		ImGui::TreePop();
	}
}
