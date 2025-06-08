#include "stdafx.hpp"
#include "states/overlays/GameOverOverlay.hpp"

GameOverOverlay::GameOverOverlay(const sf::Vector2f& windowSize)
    : Overlay(windowSize, sf::Color(0xFFFFFF80))
{}

void GameOverOverlay::update()
{
}

void GameOverOverlay::render(sf::RenderTarget& target)
{
    if (!this->getIsActive())
        return;

    this->renderWindowOverlay(target);
}

void GameOverOverlay::updateUIScaling(const sf::Vector2f& newWindowSize)
{
}
