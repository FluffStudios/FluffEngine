#pragma once

#include <core/engine.h>

class EditorState : public fluff::IGameState 
{
private:
public:
    void Configure() override;
    void Run() override;
    void Shutdown() override;
    bool ShouldShutdown() override { return false; }
};