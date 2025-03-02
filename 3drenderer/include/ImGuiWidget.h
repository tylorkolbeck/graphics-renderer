#pragma once

class ImGuiWidget
{
public:
    virtual ~ImGuiWidget() {}
    virtual void render() = 0;
    virtual void update() = 0;
};
