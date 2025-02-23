#include "stdafx.hpp"
#include "states/GameState.hpp"
#include "utils/IniParser.hpp"

void GameState::initKeybinds()
{
    std::unique_ptr<IniParser> iniParser = std::make_unique<IniParser>("config/gamestateKeybinds.ini");
    const auto& keybinds = iniParser->getSection("Snake");

    for (const auto& [bind, key] : keybinds) {
        auto it = this->supportedKeys.find(key);
        if (it != this->supportedKeys.end()) {
            this->keybinds[bind] = it->second;
        }
    }
}

void GameState::initKeyStateTracker()
{
    this->keyStateTracker = new KeyStateTracker(this->keybinds);
}

GameState::GameState(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys)
    : State(window, supportedKeys)
{
    this->circle.setRadius(200.f);
    this->circle.setFillColor(sf::Color::Green);

    this->initKeybinds();
    this->initKeyStateTracker();
}

GameState::~GameState()
{
    delete this->keyStateTracker;
}

void GameState::updateInput()
{
    this->keyStateTracker->updateKeyStates();
}

void GameState::update(const float& dt)
{
    this->updateInput();

    sf::Vector2f vel(0.f, 0.f);
    if (this->keyStateTracker->isKeyPressed("MoveUp"))
        vel.y -= 1.f;
    if (this->keyStateTracker->isKeyPressed("MoveDown"))
        vel.y += 1.f;
    if (this->keyStateTracker->isKeyPressed("MoveLeft"))
        vel.x -= 1.f;
    if (this->keyStateTracker->isKeyPressed("MoveRight"))
        vel.x += 1.f;

    this->circle.move(vel * dt * 100.f);
}

void GameState::render(sf::RenderTarget* target)
{
    if (!target)
        target = this->window;

    target->draw(this->circle);
}
