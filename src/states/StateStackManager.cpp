#include "stdafx.hpp"
#include "states/StateStackManager.hpp"

#include "states/State.hpp"

bool StateStackManager::IsEmpty() const
{
    return StateStack.empty();
}

void StateStackManager::Attach(const std::shared_ptr<IState>& state)
{
    if (state == nullptr)
    {
        return;
    }

    const bool isStateNotYetOnStack = find(StateStack.begin(), StateStack.end(), state) == StateStack.end();
    if (isStateNotYetOnStack)
    {
        StateStack.push_back(state);
        state->OnAttach();
    }
}

void StateStackManager::QueueAttach(const std::shared_ptr<IState>& state)
{
    PendingAttachments.push_back(state);
}

void StateStackManager::Detach(const std::shared_ptr<IState>& state)
{
    auto stateIt = find(StateStack.begin(), StateStack.end(), state);

    const bool isStateOnStack = stateIt != StateStack.end();
    if (isStateOnStack)
    {
        StateStack.erase(stateIt);
        state->OnDetach();
    }
}

void StateStackManager::FlushPendingAttachments()
{
    for (auto& state : PendingAttachments)
    {
        Attach(state);
    }

    PendingAttachments.clear();
}

void StateStackManager::UpdateStates(float dt)
{
    std::vector<std::shared_ptr<IState>> statesToDetach{};

    for (auto& state : StateStack)
    {
        state->Update(dt);

        if (state->WantsToBeDetached())
        {
            statesToDetach.push_back(state);
        }
    }

    for (auto& state : statesToDetach)
    {
        Detach(state);
    }
}

void StateStackManager::RenderStates(sf::RenderTarget &target)
{
    for (auto& state : StateStack)
    {
        state->Render(target);
    }
}

void StateStackManager::OnWindowResize()
{
    for (auto& state : StateStack)
    {
        state->OnWindowResize();
    }
}

void StateStackManager::OnKeyPressed(sf::Event::KeyEvent& key)
{
    for (auto& state : StateStack)
    {
        state->OnKeyPressed(key);
    }
}

void StateStackManager::OnKeyReleased(sf::Event::KeyEvent& key)
{
    for (auto& state : StateStack)
    {
        state->OnKeyReleased(key);
    }
}

void StateStackManager::OnMouseButtonPressed(sf::Event::MouseButtonEvent& mouseButton)
{
    for (auto& state : StateStack)
    {
        state->OnMouseButtonPressed(mouseButton);
    }
}

void StateStackManager::OnMouseButtonReleased(sf::Event::MouseButtonEvent& mouseButton)
{
    for (auto& state : StateStack)
    {
        state->OnMouseButtonReleased(mouseButton);
    }
}

void StateStackManager::OnMouseMoved(sf::Event::MouseMoveEvent& mouseMove)
{
    for (auto& state : StateStack)
    {
        state->OnMouseMoved(mouseMove);
    }
}
