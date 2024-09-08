#include "Useless/UselessLib.h"

#include "Useless/System/Application.h" // Application Loop
#include "Useless/Graphic/Device/CreateScreen.h" // Screen Factory
#include "Useless/Graphic/FileIO/TGALoader.h" // Support TGA images
#include "Useless/Graphic/Planes/Image.h" // Image Object
#include "Useless/Sound/Device/DSound/DSSoundCard.h" // Sound card
#include "Useless/Sound/SampleFile.h" // Sample object
#include "Useless/Sound/FileIO/WaveLoader.h" // Suppose WAV samples

class SimpleApp
{
public:
	SimpleApp() : m_image("bg.tga"), m_sample("bg.wav")
	{
		m_screen = Useless::CreateScreen("Useless Example");
		m_soundCard = new Useless::DSSoundCard(*m_screen);
	}

	void Run()
	{
		m_screen->OpenWindowed(640, 480);

		Tie2Signal(m_screen->OnResize, this, &SimpleApp::Draw);
		Tie2Signal(m_screen->OnMouseBtn, this, &SimpleApp::Click);

		Draw();

		Useless::Application::Run();
	}

	void Click(int buttons, int changed)
	{
		if ((buttons & 1) && (changed & 1)) { Play(); }
	}

	void Play()
	{
		m_soundCard->Play(m_sample);
	}

	void Draw()
	{
		m_image.SetDestWidth(m_screen->GetWidth());
		m_image.SetDestHeight(m_screen->GetHeight());

		m_image.BlitTo(0, 0, *m_screen);

		m_screen->Swap();
	}

private:
	Useless::SPointer<Useless::Screen> m_screen;
	Useless::SPointer< Useless::SoundCard > m_soundCard;
	Useless::Image m_image;
	Useless::SampleFile m_sample;
};

int Main()
{
	try {
		SimpleApp{}.Run();
	}
	catch (Useless::Error& err) {
		::OutputDebugStringA(err.what());
	}
	return 0;
}