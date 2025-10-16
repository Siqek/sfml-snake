#include "stdafx.hpp"
#include "states/overlays/EndGameOverlay.hpp"

#include "config/Colors.hpp"

EndGameOverlay::EndGameOverlay(sf::Vector2f windowSize, const sf::Font& font)
    : ButtonOverlayBase(windowSize, font,
        {{ { "Restart", "Restart" }, { "BackToMenu", "Back to Menu" } }})
{
    this->title.setFont(font);
    this->title.setFillColor(sf::Color(Colors::Hex::TextMain));
    this->title.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    this->updateUIScaling(windowSize);
}

void EndGameOverlay::setTitle(const std::string& text)
{
    this->title.setString(text);
    auto lb = this->title.getLocalBounds();
    this->title.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));
}

void EndGameOverlay::render(sf::RenderTarget& target)
{
    if (!this->getIsActive())
        return;

    ButtonOverlayBase::render(target);

    target.draw(this->title);
}

void EndGameOverlay::updateUIScaling(sf::Vector2f newWindowSize)
{
    ButtonOverlayBase::updateUIScaling(newWindowSize);

    const auto position = sf::Vector2f(newWindowSize.x / 2.f, newWindowSize.y / 3.f);
    const auto characterSize = static_cast<unsigned>(std::min(newWindowSize.x / 12.f, newWindowSize.y / 16.f));
    const auto outlineThickness = static_cast<float>(characterSize) / 32.f;

    this->title.setPosition(position);
    this->title.setCharacterSize(characterSize);
    this->title.setOutlineThickness(outlineThickness);

    const auto lb = this->title.getLocalBounds();
    this->title.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));
}
