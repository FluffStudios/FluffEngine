#pragma once

#include <core/engine.h>
#include <fluff.h>

class EditorState : public fluff::IGameState 
{
private:
public:
    void Configure() override;
    void Run() override;
    void Shutdown() override;
    inline bool ShouldShutdown() override { return false; }
};