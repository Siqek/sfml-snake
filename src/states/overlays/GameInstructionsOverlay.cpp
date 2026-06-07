#include "stdafx.hpp"
#include "states/overlays/GameInstructionsOverlay.hpp"

#include "config/Colors.hpp"

GameInstructionsOverlay::GameInstructionsOverlay(sf::Vector2f windowSize, const sf::Font& font)
    : Overlay(windowSize, sf::Color(Colors::Hex::OverlayBackground)),
    KeyPrompts({ "W", "S", "A", "D" }),
    ArrowPrompts({ 0.f, 180.f, 270.f, 90.f })
{
    KeyPromptRendererInstance.Shape.setFillColor(sf::Color(Colors::Hex::ButtonIdleBg));
    KeyPromptRendererInstance.Shape.setOutlineColor(sf::Color(Colors::Hex::TextMain));
    KeyPromptRendererInstance.Label.setFont(font);
    KeyPromptRendererInstance.Label.setFillColor(sf::Color(Colors::Hex::TextMain));

    ArrowPromptRendererInstance.Shape.setFillColor(sf::Color(Colors::Hex::ButtonIdleBg));
    ArrowPromptRendererInstance.Shape.setOutlineColor(sf::Color(Colors::Hex::TextMain));
    ArrowPromptRendererInstance.Arrow.setPointCount(3);
    ArrowPromptRendererInstance.Arrow.setScale(sf::Vector2f(0.8f, 1.f));
    ArrowPromptRendererInstance.Arrow.setFillColor(sf::Color(Colors::Hex::TextMain));

    OrText.setFont(font);
    OrText.setString("or");
    OrText.setFillColor(sf::Color(Colors::Hex::TextMain));
    OrText.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    PressAnyKeyText.setFont(font);
    PressAnyKeyText.setString("Press any key to start");
    PressAnyKeyText.setFillColor(sf::Color(Colors::Hex::TextMain));
    PressAnyKeyText.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    UpdateUIScaling(windowSize);
}

void GameInstructionsOverlay::Render(sf::RenderTarget& target)
{
    if (!IsActive())
        return;

    Overlay::Render(target);

    for (const auto& keyPrompt : KeyPrompts)
        KeyPromptRendererInstance.Render(target, keyPrompt);

    for (const auto& arrowPrompt : ArrowPrompts)
        ArrowPromptRendererInstance.Render(target, arrowPrompt);

    target.draw(OrText);
    target.draw(PressAnyKeyText);
}

void GameInstructionsOverlay::FillContext(RenderContext& context)
{
    if (!IsActive())
    {
        return;
    }

    Overlay::FillContext(context);

    for (const auto& keyPrompt : KeyPrompts)
    {
        KeyPromptRendererInstance.FillContext(context, keyPrompt);
    }

    for (const auto& arrowPrompt : ArrowPrompts)
    {
        ArrowPromptRendererInstance.FillContext(context, arrowPrompt);
    }

    context.Drawables.emplace_back(OrText);
    context.Drawables.emplace_back(PressAnyKeyText);
}

void GameInstructionsOverlay::OnWindowResize(const sf::Event::SizeEvent& size)
{
    const sf::Vector2f windowSize(size.width, size.height);

    Overlay::OnWindowResize(size);
    UpdateUIScaling(windowSize);
}

void GameInstructionsOverlay::UpdateUIScaling(sf::Vector2f windowSize)
{
    const auto shapeSideSize = std::min(windowSize.x / 16.f, windowSize.y / 16.f);
    const auto shapeSize = sf::Vector2f(shapeSideSize, shapeSideSize);
    const auto outlineThickness = shapeSideSize / 20.f;
    const auto characterSize = static_cast<unsigned>(shapeSideSize / 2.f);

    KeyPromptRendererInstance.Shape.setSize(shapeSize);
    KeyPromptRendererInstance.Shape.setOrigin(shapeSize / 2.f);
    KeyPromptRendererInstance.Shape.setOutlineThickness(outlineThickness);
    KeyPromptRendererInstance.Label.setCharacterSize(characterSize);

    const auto radius = shapeSideSize / 3.5f;

    ArrowPromptRendererInstance.Shape.setSize(shapeSize);
    ArrowPromptRendererInstance.Shape.setOrigin(shapeSize / 2.f);
    ArrowPromptRendererInstance.Shape.setOutlineThickness(outlineThickness);
    ArrowPromptRendererInstance.Arrow.setRadius(radius);
    ArrowPromptRendererInstance.Arrow.setOrigin(sf::Vector2f(radius, radius));

    const auto promptXOffset = shapeSideSize * 1.2f;
    const auto promptYOffset = shapeSideSize * 0.6f;

    const auto keyPromptXPos = windowSize.x / 3.f;
    const auto keyPromptYPos = windowSize.y / 2.f;

    KeyPrompts[0].Position = sf::Vector2f(keyPromptXPos, keyPromptYPos - promptYOffset);
    KeyPrompts[1].Position = sf::Vector2f(keyPromptXPos, keyPromptYPos + promptYOffset);
    KeyPrompts[2].Position = sf::Vector2f(keyPromptXPos - promptXOffset, keyPromptYPos + promptYOffset);
    KeyPrompts[3].Position = sf::Vector2f(keyPromptXPos + promptXOffset, keyPromptYPos + promptYOffset);

    sf::Text tempText(KeyPromptRendererInstance.Label);
    for (auto& keyPrompt : KeyPrompts)
    {
        tempText.setString(keyPrompt.Label);
        const auto lb = tempText.getLocalBounds();
        keyPrompt.LabelOrigin = sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    }

    const auto arrowPromptXPos = windowSize.x / 3.f * 2.f;
    const auto arrowPromptYPos = windowSize.y / 2.f;

    ArrowPrompts[0].Position = sf::Vector2f(arrowPromptXPos, arrowPromptYPos - promptYOffset);
    ArrowPrompts[1].Position = sf::Vector2f(arrowPromptXPos, arrowPromptYPos + promptYOffset);
    ArrowPrompts[2].Position = sf::Vector2f(arrowPromptXPos - promptXOffset, arrowPromptYPos + promptYOffset);
    ArrowPrompts[3].Position = sf::Vector2f(arrowPromptXPos + promptXOffset, arrowPromptYPos + promptYOffset);

    const auto updateTextOrigin = [](sf::Text& text) {
        const auto lb = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));
    };

    const auto textCharacterSize = static_cast<unsigned>(static_cast<float>(characterSize) * 1.6f);
    const auto textOutlineThickness = static_cast<float>(textCharacterSize) / 20.f;

    OrText.setPosition(windowSize / 2.f);
    OrText.setCharacterSize(textCharacterSize);
    OrText.setOutlineThickness(textOutlineThickness);
    updateTextOrigin(OrText);

    PressAnyKeyText.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 3.f * 2.f));
    PressAnyKeyText.setCharacterSize(textCharacterSize);
    PressAnyKeyText.setOutlineThickness(textOutlineThickness);
    updateTextOrigin(PressAnyKeyText);
}
