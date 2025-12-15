#include "stdafx.hpp"
#include "states/MainMenuState.hpp"

#include "states/StateStackManager.hpp"
#include "states/GridSelectionState.hpp"
#include "states/SettingsState.hpp"

#include "config/Colors.hpp"

MainMenuState::MainMenuState(StateContext& context)
    : IState(context)
{
    PlayButton.setFont(Context.AppFont);
    PlayButton.setText("Play");

    GoToSettingsButton.setFont(Context.AppFont);
    GoToSettingsButton.setText("Settings");

    ExitButton.setFont(Context.AppFont);
    ExitButton.setText("Exit");

    const auto setButtonColors = [](mgui::Button& button){
        button.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
        button.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
        button.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

        button.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
        button.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
        button.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));
    };

    setButtonColors(PlayButton);
    setButtonColors(GoToSettingsButton);
    setButtonColors(ExitButton);

    UpdateUIScaling();
}

void MainMenuState::Update(float /*dt*/)
{
    UpdateButtons();
}

void MainMenuState::Render(sf::RenderTarget& target)
{
    PlayButton.render(target);
    GoToSettingsButton.render(target);
    ExitButton.render(target);
}

void MainMenuState::OnWindowResize()
{
    UpdateUIScaling();
}

void MainMenuState::UpdateButtons()
{
    PlayButton.update(*Context.Window);
    GoToSettingsButton.update(*Context.Window);
    ExitButton.update(*Context.Window);

    if (PlayButton.isReleased())
    {
        Context.StateStack.QueueAttach(std::make_shared<GridSelectionState>(Context));
        MarkToBeDetached();
    }

    if (GoToSettingsButton.isReleased())
    {
        Context.StateStack.QueueAttach(std::make_shared<SettingsState>(Context));
        MarkToBeDetached();
    }

    if (ExitButton.isReleased())
    {
        MarkToBeDetached();
    }
}

void MainMenuState::UpdateUIScaling()
{
    const auto windowSize = sf::Vector2f(Context.Window->getSize());

    const std::array<mgui::Button*, 3> buttons{
        &PlayButton,
        &GoToSettingsButton,
        &ExitButton
    };

    const auto buttonSize = sf::Vector2f(windowSize.x / 3.5f, windowSize.y / 16.f);
    const auto buttonCharacterSize = static_cast<unsigned>(buttonSize.y / 2.f);
    const auto buttonOutlineThickness = buttonSize.y / 16.f;

    const auto buttonYPositionOffset = buttonSize.y * 1.6f;
    const auto firstButtonPosition = sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f - buttonYPositionOffset);

    for (size_t i = 0; i < buttons.size(); ++i) {
        mgui::Button* button = buttons[i];
        button->setPosition(sf::Vector2f(firstButtonPosition.x, firstButtonPosition.y + buttonYPositionOffset * static_cast<float>(i)));
        button->setSize(buttonSize);
        button->setCharacterSize(buttonCharacterSize);
        button->setOrigin(buttonSize / 2.f);
        button->setOutlineThickness(buttonOutlineThickness);
    }
}
