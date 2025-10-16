#include "stdafx.hpp"
#include "states/overlays/PauseOverlay.hpp"

#include "config/Colors.hpp"

PauseOverlay::PauseOverlay(const sf::Vector2f& windowSize, const sf::Font& font)
    : ButtonOverlayBase(windowSize, font,
        {{ { "Continue", "Continue" }, { "Restart", "Restart" }, { "BackToMenu", "Back to Menu" } }})
{
    this->pauseText.setFont(font);
    this->pauseText.setString("Pause");

    this->pauseText.setFillColor(sf::Color(Colors::Hex::TextMain));
    this->pauseText.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));
}

void PauseOverlay::render(sf::RenderTarget& target)
{
    if (!this->getIsActive())
        return;

    ButtonOverlayBase::render(target);

    target.draw(this->pauseText);
}

void PauseOverlay::updateUIScaling(sf::Vector2f newWindowSize)
{
    ButtonOverlayBase::updateUIScaling(newWindowSize);

    const auto position = sf::Vector2f(newWindowSize.x / 2.f, newWindowSize.y / 3.f);
    const auto characterSize = static_cast<unsigned>(std::min(newWindowSize.x / 12.f, newWindowSize.y / 16.f));
    const auto outlineThickness = static_cast<float>(characterSize) / 32.f;

    this->pauseText.setPosition(position);
    this->pauseText.setCharacterSize(characterSize);
    this->pauseText.setOutlineThickness(outlineThickness);

    const auto lb = this->pauseText.getLocalBounds();
    this->pauseText.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));
}
