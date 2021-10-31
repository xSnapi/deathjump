#include "stdafx.h"
#include "Application.hpp"

Application::Application
(
    const float& dt, 
    const float& fdt
)
    : m_dt(dt), m_fdt(fdt)
{
    InitWindow();
    LoadShaders();
    LoadMusicsAndSounds();

    SaveFile::Load("res/save");

    InitStates();

    Transition::Get().Init(); // transition has to be initialized after loading shaders
    KeyCheck::Init();
}


Application::~Application() {
    delete m_machine;

    // could be in destructor
    ShaderLibrary::Get().Clear();

    SaveFile::Save("res/save");
}

void Application::Update() {
    HandleEvents();

    Camera::Get().Update(m_dt);
    MusicController::Get().Update();

    m_machine->Update();

    Transition::Get().Update(m_dt);
    KeyCheck::Update();
}

void Application::FixedUpdate() {
    m_machine->FixedUpdate();

    //LOG(1 / m_dt, '\n');

    //LOG(sf::Mouse::getPosition(m_window).x, ", ", sf::Mouse::getPosition(m_window).y, '\n');
}

void Application::Render() {
    m_window.setView(Camera::Get().GetView());

    m_window.clear();
    m_machine->Render();
    m_window.display();
}

void Application::MoveWindow() {
    const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
    const int offset = 30;

    if (m_event.type == sf::Event::MouseButtonPressed && m_event.mouseButton.button == sf::Mouse::Left) {
        if (mousePos.x > 0 && mousePos.x < offset ||
            mousePos.x > WINDOW_WIDTH - offset && mousePos.x < WINDOW_WIDTH ||
            mousePos.y > 0 && mousePos.y < offset ||
            mousePos.y > WINDOW_HEIGHT - offset && mousePos.y < WINDOW_HEIGHT)
        {
            m_windowOffset = m_window.getPosition() - sf::Mouse::getPosition();
            m_windowGrabbed = true;
        }
    }
    else if (m_event.type == sf::Event::MouseButtonReleased && m_event.mouseButton.button == sf::Mouse::Left) {
        m_windowGrabbed = false;
    }
    else if (m_event.type == sf::Event::MouseMoved && m_windowGrabbed) {
        m_window.setPosition(sf::Mouse::getPosition() + m_windowOffset);
    }
}

void Application::LoadMusicsAndSounds() {
    //MUSIC//
    MusicController::Get().AddMusic("menu", "res/music/menu.ogg");
    MusicController::Get().AddMusic("main_track", "res/music/main.ogg");
    //MusicController::Get().AddMusic("pause", "res/music/pause.ogg");

    //SOUNDS//
    MusicController::Get().AddSound("button", "res/sounds/button.wav");
    MusicController::Get().AddSound("player_jump", "res/sounds/playerJump.wav");
    MusicController::Get().AddSound("player_crouch", "res/sounds/playerCrouch.wav");
    MusicController::Get().AddSound("blow_up", "res/sounds/blowUp.wav");
    MusicController::Get().AddSound("pick_up", "res/sounds/pickUp.wav");
    //MusicController::Get().AddSound("ghost", "res/sounds/ghost.wav"); <- it dosen't work and i dont know why B))

    //MusicController::Get().AddSound("3", "res/sounds/3.wav");
    //MusicController::Get().AddSound("2", "res/sounds/2.wav");
    //MusicController::Get().AddSound("1", "res/sounds/1.wav");
    //MusicController::Get().AddSound("GO", "res/sounds/go.wav");
}

void Application::SetShaderUniforms() {
    ShaderLibrary::Get().GetShader("transition").setUniform("u_resolution", sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    ShaderLibrary::Get().GetShader("fog").setUniform("u_resolution", sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

}

void Application::LoadShaders() {
    ShaderLibrary::Get().LoadShader("transition", "res/shaders/transitionFragment.shader",   "res/shaders/transitionVertex.shader");
    ShaderLibrary::Get().LoadShader("blur",       "res/shaders/blurFragment.shader",         "res/shaders/blurVertex.shader");
    ShaderLibrary::Get().LoadShader("fog",        "res/shaders/fogFragment.shader",          "res/shaders/fogVertex.shader");
    ShaderLibrary::Get().LoadShader("light",      "res/shaders/lightFragment.shader",        "res/shaders/lightVertex.shader");

    SetShaderUniforms();
}

void Application::InitStates() {
    m_machine = new StateMachine(m_window, m_dt, m_fdt);

    m_machine->AddStateAtTop(new Menu(*m_machine));
}

void Application::HandleEvents() {
    while (m_window.pollEvent(m_event)) {
        if (
            m_event.type == sf::Event::MouseMoved          ||
            m_event.type == sf::Event::MouseButtonPressed  ||
            m_event.type == sf::Event::MouseButtonReleased
            )
        {
           MoveWindow();
        }

        switch (m_event.type) {
        case sf::Event::Closed:
            m_window.close();
            break;
        }
    }
}

void Application::InitWindow() {
    sf::ContextSettings contextSettings;

    //contextSettings.majorVersion = 3;
    //contextSettings.minorVersion = 3;

    sf::VideoMode videoMode(WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::Image image;
    image.loadFromFile("res/textures/icon.png");

    m_window.create(videoMode, "DEATHJUMP!", sf::Style::None, contextSettings);
    m_window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
    m_window.setFramerateLimit(250);
}

bool Application::IsRunning() const {
    return m_window.isOpen();
}