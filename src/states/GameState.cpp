#include "stdafx.hpp"
#include "states/GameState.hpp"
#include "utils/IniParser.hpp"

void GameState::initKeybinds()
{
    IniParser iniParser("config/gamestateKeybinds.ini");
    const auto& keybinds = iniParser.getSection("Snake");

    this->keybinds.reserve(keybinds.size());
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

GameState::GameState(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys, const sf::Font& font)
    : State(window, supportedKeys, font)
{
    this->circle.setRadius(200.f);
    this->circle.setFillColor(sf::Color::Green);

    this->text.setFont(this->font);
    this->text.setString("Hello World!");
    this->text.setCharacterSize(24);
    this->text.setPosition(sf::Vector2f(1920.f / 2.f, 500.f));
    this->text.setFillColor(sf::Color(255, 255, 255));
    this->text.setOutlineThickness(2.f);
    this->text.setOutlineColor(sf::Color(0, 0, 0));

    // centerize text
    auto lb = this->text.getLocalBounds();
    this->text.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));

    this->initKeybinds();
    this->initKeyStateTracker();
}

GameState::~GameState()
{
    delete this->keyStateTracker;
}

void GameState::updateInput()
{
    if (!this->window->hasFocus())
        return;

    this->keyStateTracker->updateKeyStates();
}

void GameState::update(const float& dt)
{
    this->updateInput();

    /* TEMP: REMOVER LATER: This is a temporary workaround */
    if (!this->window->hasFocus())
        return;

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

    target->draw(this->text);
}
