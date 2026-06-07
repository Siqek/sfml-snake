#include "stdafx.hpp"
#include "states/overlays/Overlay.hpp"

#include "render/RenderContext.hpp"

Overlay::Overlay(sf::Vector2f windowSize, sf::Color backgroundColor)
    : bIsActive(false)
{
    WindowOverlay.setSize(windowSize);
    WindowOverlay.setFillColor(backgroundColor);
}

void Overlay::SetBackgroundColor(sf::Color color)
{
    WindowOverlay.setFillColor(color);
}

void Overlay::Close()
{
    bIsActive = false;
}

void Overlay::Show()
{
    bIsActive = true;
}

void Overlay::Render(sf::RenderTarget &target)
{
    target.draw(WindowOverlay);
}

void Overlay::FillContext(RenderContext& context)
{
    context.Drawables.emplace_back(WindowOverlay);
}

void Overlay::OnWindowResize(const sf::Event::SizeEvent& size)
{
    WindowOverlay.setSize(sf::Vector2f(size.width, size.height));
}
