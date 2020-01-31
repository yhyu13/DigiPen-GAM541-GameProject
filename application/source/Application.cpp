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
#include "engine/renderer/Renderer2D.h"

class Application : public gswy::Engine {

public:

	Application() 
		: m_CameraController(1280.0f / 720.0f)
	{
		m_Texture = gswy::Texture2D::Create("../assets/container.png");
		gswy::Renderer2D::Init();
	}

	virtual ~Application() {
	}

	virtual void Update(double ts) {
		m_CameraController.OnUpdate(ts);

		gswy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		gswy::RenderCommand::Clear();

		gswy::Renderer2D::BeginScene(m_CameraController.GetCamera());
		gswy::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, m_Texture);
		gswy::Renderer2D::EndScene();
	}

protected:

private:
	gswy::OrthographicCameraController m_CameraController;
	std::shared_ptr<gswy::Texture2D> m_Texture;
};

gswy::Engine* gswy::CreateEngineApplication() {
	return new Application();
}

