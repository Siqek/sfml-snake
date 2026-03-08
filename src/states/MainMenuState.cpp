#include "stdafx.hpp"
#include "states/MainMenuState.hpp"

#include "states/StateStackManager.hpp"
#include "states/GridSelectionState.hpp"
#include "states/SettingsState.hpp"

#include "config/Colors.hpp"

MainMenuState::MainMenuState(StateContext& context)
    : IState(context)
{
    PlayButton.SetFont(Context.AppFont);
    PlayButton.SetText("Play");

    GoToSettingsButton.SetFont(Context.AppFont);
    GoToSettingsButton.SetText("Settings");

    ExitButton.SetFont(Context.AppFont);
    ExitButton.SetText("Exit");

    SetButtonCallbacks();

    const auto setButtonColors = [](mgui::Button& button){
        button.SetFillColor(mgui::Button::EState::Idle,    sf::Color(Colors::Hex::ButtonIdleBg));
        button.SetFillColor(mgui::Button::EState::Hovered, sf::Color(Colors::Hex::ButtonHoverBg));
        button.SetFillColor(mgui::Button::EState::Pressed, sf::Color(Colors::Hex::ButtonActiveBg));

        button.SetAccentColor(mgui::Button::EState::Idle,    sf::Color(Colors::Hex::ButtonIdleOutline));
        button.SetAccentColor(mgui::Button::EState::Hovered, sf::Color(Colors::Hex::ButtonHoverOutline));
        button.SetAccentColor(mgui::Button::EState::Pressed, sf::Color(Colors::Hex::ButtonActiveOutline));
    };

    setButtonColors(PlayButton);
    setButtonColors(GoToSettingsButton);
    setButtonColors(ExitButton);

    UpdateUIScaling();
}

void MainMenuState::Render(sf::RenderTarget& target)
{
    PlayButton.Render(target);
    GoToSettingsButton.Render(target);
    ExitButton.Render(target);
}

void MainMenuState::OnWindowResize(const sf::Event::SizeEvent& size)
{
    UpdateUIScaling();
}

void MainMenuState::OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton)
{
    PlayButton.OnMouseButtonPressed(mouseButton);
    GoToSettingsButton.OnMouseButtonPressed(mouseButton);
    ExitButton.OnMouseButtonPressed(mouseButton);
}

void MainMenuState::OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton)
{
    PlayButton.OnMouseButtonReleased(mouseButton);
    GoToSettingsButton.OnMouseButtonReleased(mouseButton);
    ExitButton.OnMouseButtonReleased(mouseButton);
}

void MainMenuState::OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove)
{
    PlayButton.OnMouseMoved(mouseMove);
    GoToSettingsButton.OnMouseMoved(mouseMove);
    ExitButton.OnMouseMoved(mouseMove);
}

void MainMenuState::SetButtonCallbacks()
{
    PlayButton.SetOnReleaseCallback([this]{
        Context.StateStack.QueueAttach(std::make_shared<GridSelectionState>(Context));
        MarkToBeDetached();
    });

    GoToSettingsButton.SetOnReleaseCallback([this]{
        Context.StateStack.QueueAttach(std::make_shared<SettingsState>(Context));
        MarkToBeDetached();
    });

    ExitButton.SetOnReleaseCallback([this]{
        MarkToBeDetached();
    });
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
        button->SetPosition(sf::Vector2f(firstButtonPosition.x, firstButtonPosition.y + buttonYPositionOffset * static_cast<float>(i)));
        button->SetSize(buttonSize);
        button->SetCharacterSize(buttonCharacterSize);
        button->SetOrigin(buttonSize / 2.f);
        button->SetOutlineThickness(buttonOutlineThickness);
    }
}
