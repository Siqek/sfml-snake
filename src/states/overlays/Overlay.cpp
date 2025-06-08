#include "stdafx.hpp"
#include "states/overlays/Overlay.hpp"

Overlay::Overlay(const sf::Vector2f& windowSize, sf::Color backgroundColor)
    : isActive(false)
{
    this->windowOverlay.setSize(windowSize);
    this->windowOverlay.setFillColor(backgroundColor);
}

void Overlay::onWindowResize(const sf::Vector2f& newWindowSize)
{
    this->windowOverlay.setSize(newWindowSize);
    this->updateUIScaling(newWindowSize);
}
