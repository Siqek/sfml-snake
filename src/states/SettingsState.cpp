#include "stdafx.hpp"
#include "states/SettingsState.hpp"

#include "states/StateStackManager.hpp"
#include "states/MainMenuState.hpp"

#include "config/GameSettingsOptions.hpp"

#include "config/Colors.hpp"

SettingsState::SettingsState(StateContext& context)
    : IState(context),
    GridSizeSelector(GameSettingsOptions::GridSizeOptions, Context.AppFont, GameSettingsOptions::DefaultGridSizeOptionIndex),
    SnakeSpeedSelector(GameSettingsOptions::SnakeSpeedOptions, Context.AppFont, GameSettingsOptions::DefaultSnakeSpeedOptionIndex),
    MaxAppleCountSelector(GameSettingsOptions::MaxAppleCountOptions, Context.AppFont, GameSettingsOptions::DefaultMaxAppleCountOptionIndex),
    Settings(Context.CurrentGameSettings)
{
    const auto setElementColors = [](auto& element){
        element.SetFillColor(mgui::ButtonTypes::EState::Idle,    sf::Color(Colors::Hex::ButtonIdleBg));
        element.SetFillColor(mgui::ButtonTypes::EState::Hovered, sf::Color(Colors::Hex::ButtonHoverBg));
        element.SetFillColor(mgui::ButtonTypes::EState::Pressed, sf::Color(Colors::Hex::ButtonActiveBg));

        element.SetAccentColor(mgui::ButtonTypes::EState::Idle,    sf::Color(Colors::Hex::ButtonIdleOutline));
        element.SetAccentColor(mgui::ButtonTypes::EState::Hovered, sf::Color(Colors::Hex::ButtonHoverOutline));
        element.SetAccentColor(mgui::ButtonTypes::EState::Pressed, sf::Color(Colors::Hex::ButtonActiveOutline));
    };

    setElementColors(GridSizeSelector);
    setElementColors(SnakeSpeedSelector);
    setElementColors(MaxAppleCountSelector);

    GridSizeSelector.SetActiveOption(Settings.GridSize.Id);
    SnakeSpeedSelector.SetActiveOption(Settings.SnakeSpeed.Id);
    MaxAppleCountSelector.SetActiveOption(Settings.MaxAppleCount.Id);

    GridSizeLabel.setFont(Context.AppFont);
    GridSizeLabel.setString("Grid Size:");

    SnakeSpeedLabel.setFont(Context.AppFont);
    SnakeSpeedLabel.setString("Snake Speed:");

    MaxAppleCountLabel.setFont(Context.AppFont);
    MaxAppleCountLabel.setString("Max Apple Count:");

    SaveSettingsButton.SetFont(Context.AppFont);
    SaveSettingsButton.SetText("Save");
    setElementColors(SaveSettingsButton);

    SaveAndExitButton.SetFont(Context.AppFont);
    SaveAndExitButton.SetText("Save & Exit");
    setElementColors(SaveAndExitButton);

    SetButtonCallbacks();

    UpdateUIScaling();
}

void SettingsState::Update([[maybe_unused]] float dt)
{
    // GridSizeSelector.Update();
    // SnakeSpeedSelector.Update();
    // MaxAppleCountSelector.Update();

    UpdateSettings();
}

void SettingsState::Render(sf::RenderTarget& target)
{
    target.draw(GridSizeLabel);
    target.draw(SnakeSpeedLabel);
    target.draw(MaxAppleCountLabel);

    GridSizeSelector.Render(target);
    SnakeSpeedSelector.Render(target);
    MaxAppleCountSelector.Render(target);

    SaveSettingsButton.Render(target);
    SaveAndExitButton.Render(target);
}

void SettingsState::OnWindowResize(const sf::Event::SizeEvent& size)
{
    UpdateUIScaling();
}

void SettingsState::OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton)
{
    GridSizeSelector.OnMouseButtonPressed(mouseButton);
    SnakeSpeedSelector.OnMouseButtonPressed(mouseButton);
    MaxAppleCountSelector.OnMouseButtonPressed(mouseButton);

    SaveSettingsButton.OnMouseButtonPressed(mouseButton);
    SaveAndExitButton.OnMouseButtonPressed(mouseButton);
}

void SettingsState::OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton)
{
    GridSizeSelector.OnMouseButtonReleased(mouseButton);
    SnakeSpeedSelector.OnMouseButtonReleased(mouseButton);
    MaxAppleCountSelector.OnMouseButtonReleased(mouseButton);

    SaveSettingsButton.OnMouseButtonReleased(mouseButton);
    SaveAndExitButton.OnMouseButtonReleased(mouseButton);
}

void SettingsState::OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove)
{
    GridSizeSelector.OnMouseMoved(mouseMove);
    SnakeSpeedSelector.OnMouseMoved(mouseMove);
    MaxAppleCountSelector.OnMouseMoved(mouseMove);

    SaveSettingsButton.OnMouseMoved(mouseMove);
    SaveAndExitButton.OnMouseMoved(mouseMove);
}

void SettingsState::UpdateSettings()
{
    const auto updateSettingIfChanged = [](const auto& selector, auto& setting)
    {
        if (selector.HasActiveOptionChanged())
        {
            const auto& option = selector.GetActiveOption();
            setting.Value = option.Value;
            setting.Id = option.Id;
        }
    };

    updateSettingIfChanged(GridSizeSelector, Settings.GridSize);
    updateSettingIfChanged(SnakeSpeedSelector, Settings.SnakeSpeed);
    updateSettingIfChanged(MaxAppleCountSelector, Settings.MaxAppleCount);
}

void SettingsState::SetButtonCallbacks()
{
    SaveSettingsButton.SetOnReleaseCallback([this]{
        Context.CurrentGameSettings = Settings;
        Context.CurrentGameSettings.SaveToFile("config/game_settings.ini");
    });

    SaveAndExitButton.SetOnReleaseCallback([this]{
        Context.CurrentGameSettings = Settings;
        Context.CurrentGameSettings.SaveToFile("config/game_settings.ini");
        Context.StateStack.QueueAttach(std::make_shared<MainMenuState>(Context));
        MarkToBeDetached();
    });
}

void SettingsState::UpdateUIScaling()
{
    const auto windowSize = sf::Vector2f(Context.Window->getSize());

    const auto selectorSize = sf::Vector2f(windowSize.x / 3.5f, windowSize.y / 18.f);
    const auto firstSelectorPosition = sf::Vector2f(windowSize.x / 3.f * 2.f, windowSize.y / 2.f - 1.5f * selectorSize.y);
    const auto selectorPositionOffset = sf::Vector2f(0.f, selectorSize.y * 1.5f);
    const auto selectorOutlineThickness = selectorSize.y / 16.f;

    const auto labelCharacterSize = selectorSize.y / 2.f;
    const auto firstLabelPosition = sf::Vector2f(windowSize.x / 3.f, windowSize.y / 2.f - 1.5f * selectorSize.y);
    const auto labelPositionOffset = selectorPositionOffset;

    // Update selectors
    GridSizeSelector.SetPosition(firstSelectorPosition);
    GridSizeSelector.SetSize(selectorSize);
    GridSizeSelector.SetOrigin(selectorSize / 2.f);
    GridSizeSelector.SetOutlineThickness(selectorOutlineThickness);

    SnakeSpeedSelector.SetPosition(firstSelectorPosition + selectorPositionOffset);
    SnakeSpeedSelector.SetSize(selectorSize);
    SnakeSpeedSelector.SetOrigin(selectorSize / 2.f);
    SnakeSpeedSelector.SetOutlineThickness(selectorOutlineThickness);

    MaxAppleCountSelector.SetPosition(firstSelectorPosition + 2.f * selectorPositionOffset);
    MaxAppleCountSelector.SetSize(selectorSize);
    MaxAppleCountSelector.SetOrigin(selectorSize / 2.f);
    MaxAppleCountSelector.SetOutlineThickness(selectorOutlineThickness);

    // Update labels
    GridSizeLabel.setCharacterSize(labelCharacterSize);
    GridSizeLabel.setPosition(firstLabelPosition);

    SnakeSpeedLabel.setCharacterSize(labelCharacterSize);
    SnakeSpeedLabel.setPosition(firstLabelPosition + labelPositionOffset);

    MaxAppleCountLabel.setCharacterSize(labelCharacterSize);
    MaxAppleCountLabel.setPosition(firstLabelPosition + 2.f * labelPositionOffset);

    const auto updateLabelOrigin = [selectorSize](sf::Text& label) {
        auto lb = label.getLocalBounds();
        label.setOrigin(sf::Vector2f(selectorSize.x / 2.f, lb.height / 2.f + lb.top));
    };

    updateLabelOrigin(GridSizeLabel);
    updateLabelOrigin(SnakeSpeedLabel);
    updateLabelOrigin(MaxAppleCountLabel);

    const auto buttonPositionY = windowSize.y / 8.f * 7.f;
    const auto buttonSize = sf::Vector2f(windowSize.x / 5.f, windowSize.y / 18.f);
    const auto buttonCharacterSize = static_cast<unsigned>(buttonSize.y / 2.f);
    const auto buttonOutlineThickness = buttonSize.y / 20.f;
    const auto buttonMarginFromWindowBorder = windowSize.x / 8.f;

    // Save button
    SaveSettingsButton.SetPosition(sf::Vector2f(buttonMarginFromWindowBorder, buttonPositionY));
    SaveSettingsButton.SetSize(buttonSize);
    SaveSettingsButton.SetCharacterSize(buttonCharacterSize);
    SaveSettingsButton.SetOutlineThickness(buttonOutlineThickness);

    // Exit button
    SaveAndExitButton.SetPosition(sf::Vector2f(windowSize.x - buttonSize.x - buttonMarginFromWindowBorder, buttonPositionY));
    SaveAndExitButton.SetSize(buttonSize);
    SaveAndExitButton.SetCharacterSize(buttonCharacterSize);
    SaveAndExitButton.SetOutlineThickness(buttonOutlineThickness);
}
