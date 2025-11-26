#include "stdafx.hpp"
#include "states/overlays/Overlay.hpp"

Overlay::Overlay(const sf::Vector2f& windowSize, sf::Color backgroundColor)
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

void Overlay::OnWindowResize(const sf::Vector2f& windowSize)
{
    WindowOverlay.setSize(windowSize);
}

void Overlay::Update(const sf::RenderWindow& /*window*/) {}

void Overlay::Render(sf::RenderTarget &target)
{
    target.draw(WindowOverlay);
}
