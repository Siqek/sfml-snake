#include "stdafx.hpp"
#include "mgui/Button.hpp"

namespace mgui
{
    Button::Button()
        : prevState(ButtonState::Idle), state(ButtonState::Idle)
    {
        this->applyColorScheme(this->idleColor);
    }

    void Button::setSize(const sf::Vector2f& size)
    {
        this->shape.setSize(size);
        this->updateTextOrigin();
    }

    void Button::setPosition(const sf::Vector2f& position)
    {
        this->shape.setPosition(position);
        this->label.setPosition(position);
    }

    void Button::setOrigin(const sf::Vector2f& origin)
    {
        this->shape.setOrigin(origin);
        this->updateTextOrigin();
    }

    void Button::setText(const std::string& text)
    {
        this->label.setString(text);
        this->updateTextOrigin();
    }

    void Button::setCharacterSize(unsigned size)
    {
        this->label.setCharacterSize(size);
        this->updateTextOrigin();
    }

    void Button::setFillColor(ButtonState state, const sf::Color& color)
    {
        switch (state)
        {
        case ButtonState::Idle:
            this->idleColor.fillColor = color;
            break;
        case ButtonState::Hover:
            this->hoverColor.fillColor = color;
            break;
        case ButtonState::Active:
            this->activeColor.fillColor = color;
            break;
        default:
            break;
        }
        if (this->state == state)
            this->updateColor();
    }

    void Button::setAccentColor(ButtonState state, const sf::Color& color)
    {
        switch (state)
        {
        case ButtonState::Idle:
            this->idleColor.accentColor = color;
            break;
        case ButtonState::Hover:
            this->hoverColor.accentColor = color;
            break;
        case ButtonState::Active:
            this->activeColor.accentColor = color;
            break;
        default:
            break;
        }
        if (this->state == state)
            this->updateColor();
    }

    void Button::update(const sf::RenderWindow& window)
    {
        this->updateState(window);
    }

    void Button::render(sf::RenderTarget& target)
    {
        target.draw(this->shape);
        target.draw(this->label);
    }

    void Button::updateState(const sf::RenderWindow& window)
    {
        this->prevState = this->state;

        if (this->shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                this->state = ButtonState::Active;
            } else {
                this->state = ButtonState::Hover;
            }
        } else {
            this->state = ButtonState::Idle;
        }

        if (this->prevState != this->state)
            this->updateColor();
    }

    void Button::updateTextOrigin()
    {
        const auto& shapeSize = this->shape.getSize();
        const auto& shapeOrigin = this->shape.getOrigin();
        const auto lb = this->label.getLocalBounds();
        this->label.setOrigin(sf::Vector2f(
            lb.left + lb.width / 2.f - shapeSize.x / 2.f + shapeOrigin.x,
            lb.top + lb.height / 2.f - shapeSize.y / 2.f + shapeOrigin.y
        ));
    }

    void Button::updateColor()
    {
        switch (this->state)
        {
        case ButtonState::Idle:
            this->applyColorScheme(this->idleColor);
            break;
        case ButtonState::Hover:
            this->applyColorScheme(this->hoverColor);
            break;
        case ButtonState::Active:
            this->applyColorScheme(this->activeColor);
            break;
        default:
            break;
        }
    }

    void Button::applyColorScheme(const ColorScheme& colorScheme)
    {
        this->shape.setFillColor(colorScheme.fillColor);
        this->shape.setOutlineColor(colorScheme.accentColor);
        this->label.setFillColor(colorScheme.accentColor);
    }
}
