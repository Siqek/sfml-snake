#include "stdafx.hpp"
#include "states/overlays/PauseOverlay.hpp"

#include "config/Colors.hpp"

PauseOverlay::PauseOverlay(const sf::Vector2f& windowSize, const sf::Font& font)
    : ButtonOverlayBase(windowSize, font,
        {{ { EButton::Continue, "Continue" }, { EButton::Restart, "Restart" }, { EButton::BackToMenu, "Back to Menu" } }})
{
    PauseText.setFont(font);
    PauseText.setString("Pause");

    PauseText.setFillColor(sf::Color(Colors::Hex::TextMain));
    PauseText.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    UpdateUIScaling(windowSize);
}

void PauseOverlay::OnWindowResize(const sf::Vector2f& windowSize)
{
    ButtonOverlayBase::OnWindowResize(windowSize);
    UpdateUIScaling(windowSize);
}

void PauseOverlay::Render(sf::RenderTarget& target)
{
    if (!IsActive())
        return;

    ButtonOverlayBase::Render(target);

    target.draw(PauseText);
}

void PauseOverlay::UpdateUIScaling(sf::Vector2f newWindowSize)
{
    const auto position = sf::Vector2f(newWindowSize.x / 2.f, newWindowSize.y / 3.f);
    const auto characterSize = static_cast<unsigned>(std::min(newWindowSize.x / 12.f, newWindowSize.y / 16.f));
    const auto outlineThickness = static_cast<float>(characterSize) / 32.f;

    PauseText.setPosition(position);
    PauseText.setCharacterSize(characterSize);
    PauseText.setOutlineThickness(outlineThickness);

    const auto lb = PauseText.getLocalBounds();
    PauseText.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));
}
