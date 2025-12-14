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
        element.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
        element.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
        element.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

        element.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
        element.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
        element.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));
    };

    setElementColors(GridSizeSelector);
    setElementColors(SnakeSpeedSelector);
    setElementColors(MaxAppleCountSelector);

    GridSizeSelector.setActiveOption(Settings.GridSize.Id);
    SnakeSpeedSelector.setActiveOption(Settings.SnakeSpeed.Id);
    MaxAppleCountSelector.setActiveOption(Settings.MaxAppleCount.Id);

    GridSizeLabel.setFont(Context.AppFont);
    GridSizeLabel.setString("Grid Size:");

    SnakeSpeedLabel.setFont(Context.AppFont);
    SnakeSpeedLabel.setString("Snake Speed:");

    MaxAppleCountLabel.setFont(Context.AppFont);
    MaxAppleCountLabel.setString("Max Apple Count:");

    SaveSettingsButton.setFont(Context.AppFont);
    SaveSettingsButton.setText("Save");
    setElementColors(SaveSettingsButton);

    SaveAndExitButton.setFont(Context.AppFont);
    SaveAndExitButton.setText("Save & Exit");
    setElementColors(SaveAndExitButton);

    UpdateUIScaling();
}

void SettingsState::Update(float /*dt*/)
{
    UpdateSelectors();
    UpdateButtons();
}

void SettingsState::Render(sf::RenderTarget& target)
{
    target.draw(GridSizeLabel);
    target.draw(SnakeSpeedLabel);
    target.draw(MaxAppleCountLabel);

    GridSizeSelector.render(target);
    SnakeSpeedSelector.render(target);
    MaxAppleCountSelector.render(target);

    SaveSettingsButton.render(target);
    SaveAndExitButton.render(target);
}

void SettingsState::OnWindowResize()
{
    UpdateUIScaling();
}

void SettingsState::UpdateSelectors()
{
    GridSizeSelector.update(*Context.Window);
    SnakeSpeedSelector.update(*Context.Window);
    MaxAppleCountSelector.update(*Context.Window);

    const auto updateSettingIfChanged = [](const auto& selector, auto& setting)
    {
        if (selector.hasActiveOptionChanged())
        {
            const auto& option = selector.getActiveOption();
            setting.Value = option.value;
            setting.Id = option.id;
        }
    };

    updateSettingIfChanged(GridSizeSelector, Settings.GridSize);
    updateSettingIfChanged(SnakeSpeedSelector, Settings.SnakeSpeed);
    updateSettingIfChanged(MaxAppleCountSelector, Settings.MaxAppleCount);
}

void SettingsState::UpdateButtons()
{
    SaveSettingsButton.update(*Context.Window);
    SaveAndExitButton.update(*Context.Window);

    if (SaveSettingsButton.isReleased())
    {
        Context.CurrentGameSettings = Settings;
        Context.CurrentGameSettings.SaveToFile("config/game_settings.ini");
    }

    if (SaveAndExitButton.isReleased())
    {
        Context.CurrentGameSettings = Settings;
        Context.CurrentGameSettings.SaveToFile("config/game_settings.ini");
        Context.StateStack.QueueAttach(std::make_shared<MainMenuState>(Context));
        MarkToBeDetached();
    }
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
    GridSizeSelector.setPosition(firstSelectorPosition);
    GridSizeSelector.setSize(selectorSize);
    GridSizeSelector.setOrigin(selectorSize / 2.f);
    GridSizeSelector.setOutlineThickness(selectorOutlineThickness);

    SnakeSpeedSelector.setPosition(firstSelectorPosition + selectorPositionOffset);
    SnakeSpeedSelector.setSize(selectorSize);
    SnakeSpeedSelector.setOrigin(selectorSize / 2.f);
    SnakeSpeedSelector.setOutlineThickness(selectorOutlineThickness);

    MaxAppleCountSelector.setPosition(firstSelectorPosition + 2.f * selectorPositionOffset);
    MaxAppleCountSelector.setSize(selectorSize);
    MaxAppleCountSelector.setOrigin(selectorSize / 2.f);
    MaxAppleCountSelector.setOutlineThickness(selectorOutlineThickness);

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
    SaveSettingsButton.setPosition(sf::Vector2f(buttonMarginFromWindowBorder, buttonPositionY));
    SaveSettingsButton.setSize(buttonSize);
    SaveSettingsButton.setCharacterSize(buttonCharacterSize);
    SaveSettingsButton.setOutlineThickness(buttonOutlineThickness);

    // Exit button
    SaveAndExitButton.setPosition(sf::Vector2f(windowSize.x - buttonSize.x - buttonMarginFromWindowBorder, buttonPositionY));
    SaveAndExitButton.setSize(buttonSize);
    SaveAndExitButton.setCharacterSize(buttonCharacterSize);
    SaveAndExitButton.setOutlineThickness(buttonOutlineThickness);
}
