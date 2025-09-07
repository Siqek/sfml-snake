#include "stdafx.hpp"
#include "states/overlays/VictoryOverlay.hpp"

#include "config/Colors.hpp"

VictoryOverlay::VictoryOverlay(const sf::Vector2f& windowSize, const sf::Font& font)
    : Overlay(windowSize, sf::Color(Colors::Hex::OverlayBackground))
{
    this->victoryText.setFont(font);
    this->victoryText.setString("Snake is full. So is your glory!");
    this->victoryText.setFillColor(sf::Color(Colors::Hex::TextMain));
    this->victoryText.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    this->backToMenuButton.setFont(font);
    this->backToMenuButton.setText("Back to Menu");

    this->backToMenuButton.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
    this->backToMenuButton.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
    this->backToMenuButton.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

    this->backToMenuButton.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
    this->backToMenuButton.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
    this->backToMenuButton.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));

    this->restartButton.setFont(font);
    this->restartButton.setText("Restart");

    this->restartButton.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
    this->restartButton.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
    this->restartButton.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

    this->restartButton.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
    this->restartButton.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
    this->restartButton.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));

    this->updateUIScaling(windowSize);
}

void VictoryOverlay::update(const sf::RenderWindow& window)
{
    this->backToMenuButton.update(window);
    this->restartButton.update(window);
}

void VictoryOverlay::render(sf::RenderTarget& target)
{
    if (!this->getIsActive())
        return;

    this->renderWindowOverlay(target);

    target.draw(this->victoryText);

    this->backToMenuButton.render(target);
    this->restartButton.render(target);
}

void VictoryOverlay::updateUIScaling(const sf::Vector2f& newWindowSize)
{
    this->victoryText.setPosition(newWindowSize / 2.f);
    this->victoryText.setCharacterSize(static_cast<unsigned>(std::min(newWindowSize.y * 0.1f, newWindowSize.x * 0.04f)));
    this->victoryText.setOutlineThickness(static_cast<float>(this->victoryText.getCharacterSize()) / 32.f);

    const auto lb = this->victoryText.getLocalBounds();
    this->victoryText.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));

    sf::Vector2f firstButtonPosition = sf::Vector2f(newWindowSize.x / 2.f, newWindowSize.y * (2.f / 3.f));
    unsigned characterSize = static_cast<unsigned>(std::min(newWindowSize.x / 32.f, newWindowSize.y / 32.f));
    sf::Vector2f buttonSize;

    // backToMenuButton
    this->backToMenuButton.setCharacterSize(characterSize);

    const auto tlb = this->backToMenuButton.geTextLocalBounds();
    buttonSize = sf::Vector2f(tlb.width * 2.5f, tlb.height * 1.8f);

    this->backToMenuButton.setSize(buttonSize);
    this->backToMenuButton.setOrigin(buttonSize / 2.f);
    this->backToMenuButton.setPosition(sf::Vector2f(firstButtonPosition.x, firstButtonPosition.y + buttonSize.y * 2.5f));
    this->backToMenuButton.setOutlineThickness(buttonSize.y / 16.f);

    // restartButton
    this->restartButton.setCharacterSize(characterSize);
    this->restartButton.setSize(buttonSize);
    this->restartButton.setOrigin(buttonSize / 2.f);
    this->restartButton.setPosition(firstButtonPosition);
    this->restartButton.setOutlineThickness(buttonSize.y / 16.f);
}
