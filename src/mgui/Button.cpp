#include "stdafx.hpp"
#include "mgui/Button.hpp"

#include "render/RenderContext.hpp"

namespace mgui
{
    Button::Button()
        : CurrentState(EState::Idle),
          OnReleaseCallback(nullptr)
    {
        ApplyColorScheme(IdleColor);
    }

    void Button::SetSize(sf::Vector2f size)
    {
        Shape.setSize(size);
        UpdateTextOrigin();
    }

    void Button::SetPosition(sf::Vector2f position)
    {
        Shape.setPosition(position);
        Label.setPosition(position);
    }

    void Button::SetOrigin(sf::Vector2f origin)
    {
        Shape.setOrigin(origin);
        UpdateTextOrigin();
    }

    void Button::SetOutlineThickness(float thickness)
    {
        Shape.setOutlineThickness(thickness);
    }

    void Button::SetText(const std::string& text)
    {
        Label.setString(text);
        UpdateTextOrigin();
    }

    void Button::SetFont(const sf::Font& font)
    {
        Label.setFont(font);
    }

    void Button::SetCharacterSize(unsigned size)
    {
        Label.setCharacterSize(size);
        UpdateTextOrigin();
    }

    void Button::SetFillColor(EState state, sf::Color color)
    {
        switch (state)
        {
            case EState::Idle:
                IdleColor.FillColor = color;
                break;

            case EState::Hovered:
                HoveredColor.FillColor = color;
                break;

            case EState::Pressed:
                PressedColor.FillColor = color;
                break;

            default:
                break;
        }

        if (CurrentState == state)
        {
            UpdateColor();
        }
    }

    void Button::SetAccentColor(EState state, sf::Color color)
    {
        switch (state)
        {
            case EState::Idle:
                IdleColor.AccentColor = color;
                break;

            case EState::Hovered:
                HoveredColor.AccentColor = color;
                break;

            case EState::Pressed:
                PressedColor.AccentColor = color;
                break;

            default:
                break;
        }

        if (CurrentState == state)
        {
            UpdateColor();
        }
    }

    void Button::SetOnReleaseCallback(Callback callback)
    {
        OnReleaseCallback = std::move(callback);
    }

    sf::Vector2f Button::GetSize() const
    {
        return Shape.getSize();
    }

    sf::FloatRect Button::GetTextLocalBounds() const
    {
        return Label.getLocalBounds();
    }

    bool Button::IsPressed() const
    {
        return CurrentState == EState::Pressed;
    }

    void Button::Render(sf::RenderTarget& target)
    {
        target.draw(Shape);
        target.draw(Label);
    }

    void Button::FillContext(RenderContext& context) const
    {
        context.Drawables.emplace_back(Shape);
        context.Drawables.emplace_back(Label);
    }

    void Button::OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton)
    {
        if (IsMouseOnButton(sf::Vector2f(mouseButton.x, mouseButton.y)) &&
            mouseButton.button == sf::Mouse::Button::Left &&
            CurrentState == EState::Hovered)
        {
            SetState(EState::Pressed);
        }
    }

    void Button::OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton)
    {
        if (IsMouseOnButton(sf::Vector2f(mouseButton.x, mouseButton.y)) &&
            mouseButton.button == sf::Mouse::Button::Left &&
            CurrentState == EState::Pressed)
        {
            SetState(EState::Hovered);

            if (OnReleaseCallback)
            {
                OnReleaseCallback();
            }
        }
    }

    void Button::OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove)
    {
        if (!IsMouseOnButton(sf::Vector2f(mouseMove.x, mouseMove.y)))
        {
            SetState(EState::Idle);
            return;
        }

        if (CurrentState == EState::Pressed)
        {
            return;
        }

        SetState(EState::Hovered);
    }

    void Button::SetState(EState newState)
    {
        if (CurrentState == newState)
        {
            return;
        }

        CurrentState = newState;

        UpdateColor();
    }

    inline bool Button::IsMouseOnButton(sf::Vector2f mousePosition) const
    {
        return Shape.getGlobalBounds().contains(mousePosition);
    }

    void Button::UpdateTextOrigin()
    {
        const auto shapeSize = Shape.getSize();
        const auto shapeOrigin = Shape.getOrigin();
        const auto lb = Label.getLocalBounds();
        Label.setOrigin(sf::Vector2f(
            lb.left + lb.width / 2.f - shapeSize.x / 2.f + shapeOrigin.x,
            lb.top + lb.height / 2.f - shapeSize.y / 2.f + shapeOrigin.y
        ));
    }

    void Button::UpdateColor()
    {
        switch (CurrentState)
        {
            case EState::Idle:
                ApplyColorScheme(IdleColor);
                break;

            case EState::Hovered:
                ApplyColorScheme(HoveredColor);
                break;

            case EState::Pressed:
                ApplyColorScheme(PressedColor);
                break;

            default:
                break;
        }
    }

    void Button::ApplyColorScheme(const ColorScheme& colorScheme)
    {
        Shape.setFillColor(colorScheme.FillColor);
        Shape.setOutlineColor(colorScheme.AccentColor);
        Label.setFillColor(colorScheme.AccentColor);
    }
}
