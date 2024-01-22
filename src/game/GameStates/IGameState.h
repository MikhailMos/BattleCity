#pragma once

#include <array>

class IGameState {
public:
    virtual void Render() const = 0;
    virtual void Update(const double delta) = 0;
    virtual ~IGameState() = default;
    virtual void ProcessInput(const std::array<bool, 349>& keys) {};

    virtual unsigned int GetStateWidth() const = 0;
    virtual unsigned int GetStateHeight() const = 0;
};