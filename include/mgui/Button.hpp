#ifndef MGUI_BUTTON_HPP
#define MGUI_BUTTON_HPP

namespace mgui
{
    enum class ButtonState {
        Idle,
        Hover,
        Active
    };

    class Button {
    public:
        Button();
        ~Button() = default;

        void setSize(const sf::Vector2f& size);
        void setPosition(const sf::Vector2f& position);
        void setOrigin(const sf::Vector2f& origin);

        void setText(const std::string& text);
        void setFont(const sf::Font& font) { label.setFont(font); };
        void setCharacterSize(unsigned size);

        void setFillColor(ButtonState state, const sf::Color& color);
        void setOutlineThickness(float thickness) { shape.setOutlineThickness(thickness); };

        ////////////////////////////////////////////////////////////
        /// \brief Sets the accent color (text and outline)
        ///        for the specified button state.
        ///
        /// This function assigns a common color to both the
        /// button's text and its outline depending on its state.
        ///
        /// \param state The button state for which the accent
        ///              color should be applied.
        /// \param color The color to be used for the text and
        ///              outline in the given state.
        ///
        ////////////////////////////////////////////////////////////
        void setAccentColor(ButtonState state, const sf::Color& color);

        const sf::Vector2f& getSize() const { return shape.getSize(); };
        sf::FloatRect geTextLocalBounds() const { return label.getLocalBounds(); };

        bool isPressed() const { return state == ButtonState::Active; };
        bool isReleased() const { return prevState == ButtonState::Active && state == ButtonState::Hover; };

        void update(const sf::RenderWindow& window);
        void render(sf::RenderTarget& target);

    private:
        struct ColorScheme {
            sf::Color fillColor;
            sf::Color accentColor;

            ColorScheme()
                : fillColor(0x000000FF), accentColor(0xFFFFFFFF) {}
        };

        sf::RectangleShape shape;
        sf::Text label;

        ColorScheme idleColor;
        ColorScheme hoverColor;
        ColorScheme activeColor;

        ButtonState prevState;
        ButtonState state;

        void updateState(const sf::RenderWindow& window);

        void updateTextOrigin();

        void updateColor();
        void applyColorScheme(const ColorScheme& colorScheme);
    };
}

#endif // MGUI_BUTTON_HPP
