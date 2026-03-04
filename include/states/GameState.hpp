#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

namespace UIConfig {
    static constexpr float ScoreHeightRatio = 0.2f;
    static constexpr float GridHeightRatio = 0.75f;
}

class IGrid;
class ISnake;

#include "states/State.hpp"

#include "game/snake/SnakeRenderer.hpp"
#include "game/AppleCluster.hpp"

#include "states/overlays/GameInstructionsOverlay.hpp"
#include "states/overlays/PauseOverlay.hpp"
#include "states/overlays/EndGameOverlay.hpp"

class GameState final
    : public IState
{
public:
    GameState(StateContext& context);
    ~GameState();

    void Update(float dt) override;

    void Render(sf::RenderTarget& target) override;

    void OnWindowResize(const sf::Event::SizeEvent& size) override;

    void OnKeyPressed(const sf::Event::KeyEvent& key) override;

    void OnKeyReleased(const sf::Event::KeyEvent& key) override;

private:
    void InitKeybinds();

    void UpdateScoreText();

    void UpdateUIScaling();

    std::shared_ptr<IGrid> CreateGrid();

    std::unique_ptr<ISnake> CreateSnake();

    void Restart();

    enum class EAction : uint16_t
    {
        MoveUp       = 1 << 0,
        MoveDown     = 1 << 1,
        MoveRight    = 1 << 2,
        MoveLeft     = 1 << 3,
        AltMoveUp    = 1 << 4,
        AltMoveDown  = 1 << 5,
        AltMoveRight = 1 << 6,
        AltMoveLeft  = 1 << 7,
        TogglePause  = 1 << 8
    };

    struct KeyDownState {
        using Mask = uint16_t;

        KeyDownState(Mask flags)
            : Flags(flags) {}

        inline bool IsDown(EAction action) const
        {
            return Flags & static_cast<Mask>(action);
        }

        inline void SetDown(EAction action)
        {
            Flags |= static_cast<Mask>(action);
        }

        inline void UnsetDown(EAction action)
        {
            Flags &= ~static_cast<Mask>(action);
        }

    private:
        Mask Flags;
    };

    std::unordered_map<EAction, sf::Keyboard::Key> Keybinds;

    KeyDownState KeyState;

    std::shared_ptr<IGrid> Grid;

    sf::Vector2f GridOffset;
    float TileSize;
    sf::RectangleShape Tile;

    std::unique_ptr<ISnake> PlayerSnake;
    SnakeRenderer PlayerSnakeRenderer;

    AppleCluster Apples;

    unsigned int Score;
    sf::Text ScoreText;

    GameInstructionsOverlay InstructionsOverlay;
    PauseOverlay PauseMenu;
    EndGameOverlay EndGameMenu;
};

#endif
