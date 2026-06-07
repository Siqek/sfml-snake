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

#include "render/RenderContext.hpp"

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

    ButtonOverlayBase(sf::Vector2f windowSize, const sf::Font& font, const std::array<ButtonInit, N>& buttonInits);
    virtual ~ButtonOverlayBase() = default;

    void SetOnReleaseButtonCallback(E id, mgui::Button::Callback callback);

    virtual void Render(sf::RenderTarget& target) override;

    virtual void FillContext(RenderContext& context) override;

    virtual void OnWindowResize(const sf::Event::SizeEvent& windowSize) override;

    virtual void OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton);

    virtual void OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton);

    virtual void OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove);

private:
    void UpdateUIScaling(sf::Vector2f windowSize);

    struct OverlayButton {
        E Id;
        mgui::Button Button;
    };

    std::array<OverlayButton, N> Buttons;
};

template<typename E, size_t N>
ButtonOverlayBase<E, N>::ButtonOverlayBase(sf::Vector2f windowSize, const sf::Font& font, const std::array<ButtonInit, N>& buttonInits)
    : Overlay(windowSize, sf::Color(Colors::Hex::OverlayBackground))
{
    for (size_t i = 0; i < N; ++i)
    {
        Buttons[i].Id = buttonInits[i].Id;

        Buttons[i].Button.SetFont(font);
        Buttons[i].Button.SetText(buttonInits[i].Text);

        Buttons[i].Button.SetFillColor(mgui::Button::EState::Idle,    sf::Color(Colors::Hex::ButtonIdleBg));
        Buttons[i].Button.SetFillColor(mgui::Button::EState::Hovered, sf::Color(Colors::Hex::ButtonHoverBg));
        Buttons[i].Button.SetFillColor(mgui::Button::EState::Pressed, sf::Color(Colors::Hex::ButtonActiveBg));

        Buttons[i].Button.SetAccentColor(mgui::Button::EState::Idle,    sf::Color(Colors::Hex::ButtonIdleOutline));
        Buttons[i].Button.SetAccentColor(mgui::Button::EState::Hovered, sf::Color(Colors::Hex::ButtonHoverOutline));
        Buttons[i].Button.SetAccentColor(mgui::Button::EState::Pressed, sf::Color(Colors::Hex::ButtonActiveOutline));
    }
    UpdateUIScaling(windowSize);
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::SetOnReleaseButtonCallback(E id, mgui::Button::Callback callback)
{
    for (OverlayButton& button : Buttons)
    {
        if (button.Id == id)
        {
            button.Button.SetOnReleaseCallback(callback);
            break;
        }
    }
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::Render(sf::RenderTarget& target)
{
    if (!IsActive())
    {
        return;
    }

    Overlay::Render(target);

    for (auto& button : Buttons)
    {
        button.Button.Render(target);
    }
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::FillContext(RenderContext& context)
{
    if (!IsActive())
    {
        return;
    }

    Overlay::FillContext(context);

    for (const OverlayButton& button : Buttons)
    {
        button.Button.FillContext(context);
    }
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::OnWindowResize(const sf::Event::SizeEvent& size)
{
    const sf::Vector2f windowSize(size.width, size.height);

    Overlay::OnWindowResize(size);
    UpdateUIScaling(windowSize);
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton)
{
    for (OverlayButton& button : Buttons)
    {
        button.Button.OnMouseButtonPressed(mouseButton);
    }
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton)
{
    for (OverlayButton& button : Buttons)
    {
        button.Button.OnMouseButtonReleased(mouseButton);
    }
}

template<typename E, size_t N>
void ButtonOverlayBase<E, N>::OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove)
{
    for (OverlayButton& button : Buttons)
    {
        button.Button.OnMouseMoved(mouseMove);
    }
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

        button.SetSize(size);
        button.SetPosition(position + static_cast<float>(i) * positionOffset);
        button.SetOrigin(size / 2.f);
        button.SetOutlineThickness(outlineThickness);
        button.SetCharacterSize(characterSize);
    }
}

#endif // BUTTONOVERLAYBASE_HPP
