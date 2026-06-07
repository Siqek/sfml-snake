#include "stdafx.hpp"
#include "states/overlays/EndGameOverlay.hpp"

#include "config/Colors.hpp"

#include "render/RenderContext.hpp"

EndGameOverlay::EndGameOverlay(sf::Vector2f windowSize, const sf::Font& font)
    : ButtonOverlayBase(windowSize, font,
        {{ { EButton::Restart, "Restart" }, { EButton::BackToMenu, "Back to Menu" } }})
{
    Title.setFont(font);
    Title.setFillColor(sf::Color(Colors::Hex::TextMain));
    Title.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    UpdateUIScaling(windowSize);
}

void EndGameOverlay::SetTitle(const std::string& text)
{
    Title.setString(text);
    auto lb = Title.getLocalBounds();
    Title.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));
}

void EndGameOverlay::Render(sf::RenderTarget& target)
{
    if (!IsActive())
    {
        return;
    }

    ButtonOverlayBase::Render(target);

    target.draw(Title);
}

void EndGameOverlay::FillContext(RenderContext& context)
{
    if (!IsActive())
    {
        return;
    }

    ButtonOverlayBase::FillContext(context);

    context.Drawables.emplace_back(Title);
}

void EndGameOverlay::OnWindowResize(const sf::Event::SizeEvent& size)
{
    const sf::Vector2f windowSize(size.width, size.height);

    ButtonOverlayBase::OnWindowResize(size);
    UpdateUIScaling(windowSize);
}

void EndGameOverlay::UpdateUIScaling(sf::Vector2f windowSize)
{
    const auto position = sf::Vector2f(windowSize.x / 2.f, windowSize.y / 3.f);
    const auto characterSize = static_cast<unsigned>(std::min(windowSize.x / 12.f, windowSize.y / 16.f));
    const auto outlineThickness = static_cast<float>(characterSize) / 32.f;

    Title.setPosition(position);
    Title.setCharacterSize(characterSize);
    Title.setOutlineThickness(outlineThickness);

    const auto lb = Title.getLocalBounds();
    Title.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));
}
