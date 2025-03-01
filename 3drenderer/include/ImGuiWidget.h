#pragma once

class ImGuiWidget
{
public:
    virtual ~ImGuiWidget() {}
    virtual void render() = 0;
};
