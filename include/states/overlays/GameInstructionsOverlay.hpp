#ifndef GAMEINSTRUCTIONSOVERLAY_HPP
#define GAMEINSTRUCTIONSOVERLAY_HPP

#include "states/overlays/Overlay.hpp"

class GameInstructionsOverlay
    : public Overlay
{
public:
    GameInstructionsOverlay(sf::Vector2f windowSize, const sf::Font& font);
    ~GameInstructionsOverlay() = default;

    void update(const sf::RenderWindow& window);
    void render(sf::RenderTarget& target);

private:
    void updateUIScaling(sf::Vector2f newWindowSize);

    struct KeyPrompt {
        sf::Vector2f position;
        sf::Vector2f labelOrigin;
        std::string label;

        KeyPrompt(const char* label)
            : position(0.f, 0.f), labelOrigin(0.f, 0.f), label(label) {}
    };

    struct ArrowPrompt {
        float rotation;
        sf::Vector2f position;

        ArrowPrompt(float rotation, sf::Vector2f position = sf::Vector2f(0.f, 0.f))
            : rotation(rotation), position(position) {}
    };

    struct KeyPromptRenderer {
        sf::RectangleShape shape;
        sf::Text label;

        void render(sf::RenderTarget& target, const KeyPrompt& keyPrompt)
        {
            shape.setPosition(keyPrompt.position);
            label.setPosition(keyPrompt.position);
            label.setOrigin(keyPrompt.labelOrigin);
            label.setString(keyPrompt.label);
            target.draw(shape);
            target.draw(label);
        }
    };

    struct ArrowPromptRenderer {
        sf::RectangleShape shape;
        sf::CircleShape arrow;

        void render(sf::RenderTarget& target, const ArrowPrompt& arrowPrompt)
        {
            shape.setPosition(arrowPrompt.position);
            arrow.setPosition(arrowPrompt.position);
            arrow.setRotation(arrowPrompt.rotation);
            target.draw(shape);
            target.draw(arrow);
        }
    };

    KeyPromptRenderer keyPromptRenderer;
    ArrowPromptRenderer arrowPromptRenderer;

    std::array<KeyPrompt, 4> keyPrompts;
    std::array<ArrowPrompt, 4> arrowPrompts;

    sf::Text orText;
    sf::Text pressAnyKeyText;
};

#endif // GAMEINSTRUCTIONSOVERLAY_HPP
