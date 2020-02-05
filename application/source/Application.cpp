/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Application.cpp
Purpose			: Game application class. Creates an instance of the game application which plugs into the engine.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#include "EngineExport.h"

using namespace gswy;	

class Application : public Engine {

public:


	Application() 
		: m_CameraController(1280.0f / 720.0f)
	{
		gswy::Renderer2D::Init();
		//Texture Test
		m_Texture = gswy::Texture2D::Create("./asset/container.png");

		ResourceAllocator::GetInstance()->SetPath("./asset/");
		int spriteExampleID = ResourceAllocator::GetInstance()->Add("SpriteSheetExample.png", 8, 4);

		//Sprite Test
		m_ControlSprite = ResourceAllocator::GetInstance()->Get(spriteExampleID);//std::make_shared<gswy::Sprite>("./asset/SpriteSheetExample.png", 8, 4);
	}

	virtual ~Application() {
	}

	virtual void Run() override
	{
		// TODO : remove audio test code below
		AudioManager::GetInstance()->PlaySound("./asset/breakout.mp3", AudioVector3{ 0, 0, 0 }, 1);

		FramerateController* rateController = FramerateController::GetInstance(60);
		Input* input = Input::GetInstance();
		while (m_isRunning) {
			rateController->FrameStart();

#ifdef _DEBUG
			std::stringstream stream;
			stream << "Frame Time: " << rateController->GetFrameTime() * 1000 << "ms";
			m_window->UpdateTitle(stream.str());
#endif
			m_window->Update();

			if (input->IsKeyPressed(GLFW_KEY_A)) {
				PRINT("KEY A PRESSED!");
			}

			if (input->IsKeyTriggered(GLFW_KEY_SPACE)) {
				PRINT("KEY SPACE TRIGGERED!");
			}

			if (input->IsKeyReleased(GLFW_KEY_A)) {
				PRINT("KEY A RELEASED!");
			}

			if (input->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
				PRINT("Mouse button 1 PRESSED!");
			}

			if (input->IsMouseButtonReleased(GLFW_MOUSE_BUTTON_1)) {
				PRINT("Mouse button 1 RELEASED!");
			}

			std::stringstream stream1;
			stream1 << "cursor-x: " << input->GetMousePositionX() << "\t";
			stream1 << "cursor-y: " << input->GetMousePositionY();
			PRINT(stream1.str());

			//Control Sprite Trigger
			if (input->IsKeyTriggered(GLFW_KEY_W)) {
				PRINT("KEY W TRIGGERED!");
				m_ControlSprite->SetAnimSequence(16, 8);
				m_ControlSprite->SetAnimLooping(true);
			}
			else if (input->IsKeyTriggered(GLFW_KEY_S)) {
				PRINT("KEY S TRIGGERED!");
				m_ControlSprite->SetAnimSequence(24, 8);
				m_ControlSprite->SetAnimLooping(true);
			}
			else if (input->IsKeyTriggered(GLFW_KEY_A)) {
				PRINT("KEY A TRIGGERED!");
				m_ControlSprite->SetAnimSequence(8, 8);
				m_ControlSprite->SetAnimLooping(true);
			}
			else if (input->IsKeyTriggered(GLFW_KEY_D)) {
				PRINT("KEY D TRIGGERED!");
				m_ControlSprite->SetAnimSequence(0, 8);
				m_ControlSprite->SetAnimLooping(true);
			}

			//Control Sprite KeyPress
			if (input->IsKeyPressed(GLFW_KEY_W)) {
				PRINT("KEY W PRESSED!");
				m_ControlSpritePos.x += -sin(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
				m_ControlSpritePos.y += cos(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
				m_ControlSprite->SetSpritePosition(m_ControlSpritePos);
			}
			else if (input->IsKeyPressed(GLFW_KEY_S)) {
				PRINT("KEY S PRESSED!");
				m_ControlSpritePos.x -= -sin(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
				m_ControlSpritePos.y -= cos(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
				m_ControlSprite->SetSpritePosition(m_ControlSpritePos);
			}
			
			if (input->IsKeyPressed(GLFW_KEY_A)) {
				PRINT("KEY A PRESSED!");
				m_ControlSpritePos.x -= cos(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
				m_ControlSpritePos.y -= sin(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
				m_ControlSprite->SetSpritePosition(m_ControlSpritePos);
			}
			else if (input->IsKeyPressed(GLFW_KEY_D)) {
				PRINT("KEY D PRESSED!");
				m_ControlSpritePos.x += cos(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
				m_ControlSpritePos.y += sin(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
				m_ControlSprite->SetSpritePosition(m_ControlSpritePos);
			}

			Update(rateController->GetFrameTime());
			Input::GetInstance()->Update();
			
			m_isRunning = !m_window->ShouldExit();
			rateController->FrameEnd();
		}
	}

	virtual void Update(double ts) {
		m_CameraController.OnUpdate(ts);

		gswy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		gswy::RenderCommand::Clear();

		gswy::Renderer2D::BeginScene(m_CameraController.GetCamera());
		
		//Draw Quad with Texture
		//gswy::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, m_Texture);
		
		//Draw Sprite Sheet
		m_ControlSprite->Update(ts * 1000.0f);
		m_ControlSprite->Draw();

		gswy::Renderer2D::EndScene();
	}

protected:

private:
	gswy::OrthographicCameraController m_CameraController;
	std::shared_ptr<gswy::Texture2D> m_Texture;

	//Control Sprite
	std::shared_ptr<gswy::Sprite> m_ControlSprite;
	glm::vec3 m_ControlSpritePos = glm::vec3(0.0f, 0.0f, 1.0f);
	float m_ControlSpriteMoveSpeed = 5.0f;
};

Engine* gswy::CreateEngineApplication() {
	return new Application();
}

