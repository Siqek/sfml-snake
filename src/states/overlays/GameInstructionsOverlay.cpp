#include "stdafx.hpp"
#include "states/overlays/GameInstructionsOverlay.hpp"

#include "config/Colors.hpp"

GameInstructionsOverlay::GameInstructionsOverlay(sf::Vector2f windowSize, const sf::Font& font)
    : Overlay(windowSize, sf::Color(Colors::Hex::OverlayBackground)),
    keyPrompts({ "W", "S", "A", "D" }),
    arrowPrompts({ 0.f, 180.f, 90.f, 270.f })
{
    this->keyPromptRenderer.shape.setFillColor(sf::Color(Colors::Hex::ButtonIdleBg));
    this->keyPromptRenderer.shape.setOutlineColor(sf::Color(Colors::Hex::ButtonIdleOutline));
    this->keyPromptRenderer.label.setFont(font);
    this->keyPromptRenderer.label.setFillColor(sf::Color(Colors::Hex::TextMain));

    this->arrowPromptRenderer.shape.setFillColor(sf::Color(Colors::Hex::ButtonIdleBg));
    this->arrowPromptRenderer.shape.setOutlineColor(sf::Color(Colors::Hex::ButtonIdleOutline));
    this->arrowPromptRenderer.arrow.setPointCount(3);
    this->arrowPromptRenderer.arrow.setScale(sf::Vector2f(0.8f, 1.f));
    this->arrowPromptRenderer.arrow.setFillColor(sf::Color(Colors::Hex::TextMain));

    this->updateUIScaling(windowSize);
}

void GameInstructionsOverlay::update(const sf::RenderWindow& /*window*/) {}

void GameInstructionsOverlay::render(sf::RenderTarget& target)
{
    if (!this->getIsActive())
        return;

    this->renderWindowOverlay(target);

    for (const auto& keyPrompt : this->keyPrompts)
        this->keyPromptRenderer.render(target, keyPrompt);

    for (const auto& arrowPrompt : this->arrowPrompts)
        this->arrowPromptRenderer.render(target, arrowPrompt);
}

void GameInstructionsOverlay::updateUIScaling(sf::Vector2f newWindowSize)
{
    const auto shapeSideSize = std::min(newWindowSize.x / 16.f, newWindowSize.y / 16.f);
    const auto shapeSize = sf::Vector2f(shapeSideSize, shapeSideSize);
    const auto outlineThickness = shapeSideSize / 20.f;
    const auto characterSize = static_cast<unsigned>(shapeSideSize / 2.f);

    this->keyPromptRenderer.shape.setSize(shapeSize);
    this->keyPromptRenderer.shape.setOrigin(shapeSize / 2.f);
    this->keyPromptRenderer.shape.setOutlineThickness(outlineThickness);
    this->keyPromptRenderer.label.setCharacterSize(characterSize);

    const auto radius = shapeSideSize / 3.5f;

    this->arrowPromptRenderer.shape.setSize(shapeSize);
    this->arrowPromptRenderer.shape.setOrigin(shapeSize / 2.f);
    this->arrowPromptRenderer.shape.setOutlineThickness(outlineThickness);
    this->arrowPromptRenderer.arrow.setRadius(radius);
    this->arrowPromptRenderer.arrow.setOrigin(sf::Vector2f(radius, radius));

    const auto promptXOffset = shapeSideSize * 1.2f;
    const auto promptYOffset = shapeSideSize * 0.6f;

    const auto keyPromptXPos = newWindowSize.x / 3.f;
    const auto keyPromptYPos = newWindowSize.y / 2.f;

    this->keyPrompts[0].position = sf::Vector2f(keyPromptXPos, keyPromptYPos - promptYOffset);
    this->keyPrompts[1].position = sf::Vector2f(keyPromptXPos, keyPromptYPos + promptYOffset);
    this->keyPrompts[2].position = sf::Vector2f(keyPromptXPos - promptXOffset, keyPromptYPos + promptYOffset);
    this->keyPrompts[3].position = sf::Vector2f(keyPromptXPos + promptXOffset, keyPromptYPos + promptYOffset);

    sf::Text tempText(this->keyPromptRenderer.label);
    for (auto& keyPrompt : this->keyPrompts)
    {
        tempText.setString(keyPrompt.label);
        const auto lb = tempText.getLocalBounds();
        keyPrompt.labelOrigin = sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    }

    const auto arrowPromptXPos = newWindowSize.x / 3.f * 2.f;
    const auto arrowPromptYPos = newWindowSize.y / 2.f;

    this->arrowPrompts[0].position = sf::Vector2f(arrowPromptXPos, arrowPromptYPos - promptYOffset);
    this->arrowPrompts[1].position = sf::Vector2f(arrowPromptXPos, arrowPromptYPos + promptYOffset);
    this->arrowPrompts[2].position = sf::Vector2f(arrowPromptXPos - promptXOffset, arrowPromptYPos + promptYOffset);
    this->arrowPrompts[3].position = sf::Vector2f(arrowPromptXPos + promptXOffset, arrowPromptYPos + promptYOffset);
}
