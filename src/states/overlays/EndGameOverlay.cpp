#include "stdafx.hpp"
#include "states/overlays/EndGameOverlay.hpp"

#include "config/Colors.hpp"

EndGameOverlay::EndGameOverlay(sf::Vector2f windowSize, const sf::Font& font)
    : Overlay(windowSize, sf::Color(Colors::Hex::OverlayBackground))
{
    this->title.setFont(font);
    this->title.setFillColor(sf::Color(Colors::Hex::TextMain));
    this->title.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    auto initButton = [&font](mgui::Button& button, const std::string& text) {
        button.setFont(font);
        button.setText(text);

        button.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
        button.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
        button.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

        button.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
        button.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
        button.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));
    };

    initButton(this->restartButton, "Restart");
    initButton(this->backToMenuButton, "Back to Menu");

    this->updateUIScaling(windowSize);
}

void EndGameOverlay::setTitle(const std::string& text)
{
    this->title.setString(text);
    auto lb = this->title.getLocalBounds();
    this->title.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));
}

void EndGameOverlay::update(const sf::RenderWindow& window)
{
    this->restartButton.update(window);
    this->backToMenuButton.update(window);
}

void EndGameOverlay::render(sf::RenderTarget& target)
{
    if (!this->getIsActive())
        return;

    this->renderWindowOverlay(target);

    target.draw(this->title);

    this->restartButton.render(target);
    this->backToMenuButton.render(target);
}

void EndGameOverlay::updateUIScaling(sf::Vector2f newWindowSize)
{
    this->title.setPosition(newWindowSize / 2.f);
    this->title.setCharacterSize(static_cast<unsigned>(std::min(newWindowSize.y * 0.1f, newWindowSize.x * 0.04f)));
    this->title.setOutlineThickness(static_cast<float>(this->title.getCharacterSize()) / 32.f);

    const auto lb = this->title.getLocalBounds();
    this->title.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));

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
