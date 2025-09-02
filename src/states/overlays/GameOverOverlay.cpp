#include "stdafx.hpp"
#include "states/overlays/GameOverOverlay.hpp"

#include "config/Colors.hpp"

GameOverOverlay::GameOverOverlay(const sf::Vector2f& windowSize, const sf::Font& font)
    : Overlay(windowSize, sf::Color(Colors::Hex::OverlayBackground))
{
    this->gameOverText.setFont(font);
    this->gameOverText.setString("Game Over");
    this->gameOverText.setFillColor(sf::Color(Colors::Hex::TextMain));
    this->gameOverText.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    this->backToMenuButton.setFont(font);
    this->backToMenuButton.setText("Back to Menu");

    this->backToMenuButton.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
    this->backToMenuButton.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
    this->backToMenuButton.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

    this->backToMenuButton.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
    this->backToMenuButton.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
    this->backToMenuButton.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));

    this->updateUIScaling(windowSize);
}

void GameOverOverlay::update(const sf::RenderWindow& window)
{
    this->backToMenuButton.update(window);
}

void GameOverOverlay::render(sf::RenderTarget& target)
{
    if (!this->getIsActive())
        return;

    this->renderWindowOverlay(target);

    target.draw(this->gameOverText);

    this->backToMenuButton.render(target);
}

void GameOverOverlay::updateUIScaling(const sf::Vector2f& newWindowSize)
{
    this->gameOverText.setPosition(newWindowSize / 2.f);
    this->gameOverText.setCharacterSize(static_cast<unsigned>(std::min(newWindowSize.y * 0.1f, newWindowSize.x * 0.04f)));
    this->gameOverText.setOutlineThickness(static_cast<float>(this->gameOverText.getCharacterSize()) / 32.f);

    const auto lb = this->gameOverText.getLocalBounds();
    this->gameOverText.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));

    this->backToMenuButton.setPosition(sf::Vector2f(newWindowSize.x / 2.f, newWindowSize.y * (2.f / 3.f)));
    this->backToMenuButton.setCharacterSize(static_cast<unsigned>(std::min(newWindowSize.x / 32.f, newWindowSize.y / 32.f)));

    const auto tlb = this->backToMenuButton.geTextLocalBounds();
    this->backToMenuButton.setSize(sf::Vector2f(tlb.width * 2.5f, tlb.height * 1.8f));

    this->backToMenuButton.setOrigin(this->backToMenuButton.getSize() / 2.f);
    this->backToMenuButton.setOutlineThickness(this->backToMenuButton.getSize().y / 16.f);
}
