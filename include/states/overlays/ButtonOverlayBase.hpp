#ifndef BUTTONOVERLAYBASE_HPP
#define BUTTONOVERLAYBASE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <string>
#include <array>

#include "states/overlays/Overlay.hpp"

#include "mgui/Button.hpp"

#include "config/Colors.hpp"

template<typename E, size_t N>
class ButtonOverlayBase
    : public Overlay
{
    static_assert(std::is_enum_v<E>, "Template parameter must be an enum type");
public:
    struct ButtonInit {
        E Id;
        std::string Text;

        ButtonInit(E id, const char* text)
            : Id(id), Text(text) {}
    };

    ButtonOverlayBase(const sf::Vector2f& windowSize, const sf::Font& font, const std::array<ButtonInit, N>& buttonInits);
    virtual ~ButtonOverlayBase() = default;

    bool IsButtonReleased(E id) const;

    virtual void OnWindowResize(const sf::Vector2f& windowSize) override;

    virtual void Update(const sf::RenderWindow& window) override;
    virtual void Render(sf::RenderTarget& target) override;

private:
    void UpdateUIScaling(sf::Vector2f windowSize);

    struct OverlayButton {
        E Id;
        mgui::Button Button;
    };

    std::array<OverlayButton, N> Buttons;
};

template<typename E, size_t N>
ButtonOverlayBase<E, N>::ButtonOverlayBase(const sf::Vector2f& windowSize, const sf::Font& font, const std::array<ButtonInit, N>& buttonInits)
    : Overlay(windowSize, sf::Color(Colors::Hex::OverlayBackground))
{
    for (size_t i = 0; i < N; ++i)
    {
        Buttons[i].Id = buttonInits[i].Id;

        Buttons[i].Button.setFont(font);
        Buttons[i].Button.setText(buttonInits[i].Text);

        Buttons[i].Button.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
        Buttons[i].Button.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
        Buttons[i].Button.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

        Buttons[i].Button.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
        Buttons[i].Button.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
        Buttons[i].Button.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));
    }
    UpdateUIScaling(windowSize);
}

template<typename E, size_t N>
bool ButtonOverlayBase<E, N>::IsButtonReleased(E id) const
{
    for (const auto& button : Buttons)
    {
        if (button.Id == id)
            return button.Button.isReleased();
    }

    return false;
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::OnWindowResize(const sf::Vector2f& windowSize)
{
    Overlay::OnWindowResize(windowSize);
    UpdateUIScaling(windowSize);
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::Update(const sf::RenderWindow& window)
{
    for (auto& button : Buttons)
        button.Button.update(window);
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::Render(sf::RenderTarget& target)
{
    if (!IsActive())
        return;

    Overlay::Render(target);

    for (auto& button : Buttons)
        button.Button.render(target);
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::UpdateUIScaling(sf::Vector2f windowSize)
{
    const auto size = sf::Vector2f(windowSize.x / 3.f, windowSize.y / 16.f);
    const auto position = windowSize / 2.f;
    const auto positionOffset = sf::Vector2f(0.f, size.y * 1.4f);
    const auto outlineThickness = size.y / 16.f;
    const auto characterSize = static_cast<unsigned>(std::min(size.y / 2.f, size.x / 12.f));

    for (size_t i = 0; i < Buttons.size(); ++i)
    {
        mgui::Button& button = Buttons[i].Button;

        button.setSize(size);
        button.setPosition(position + static_cast<float>(i) * positionOffset);
        button.setOrigin(size / 2.f);
        button.setOutlineThickness(outlineThickness);
        button.setCharacterSize(characterSize);
    }
}

#endif // BUTTONOVERLAYBASE_HPP
