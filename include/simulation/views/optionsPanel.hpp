#pragma once

class MainController;


class OptionsPanel {
public:
    explicit OptionsPanel(MainController& controller):
        controller(controller) {}

    void Render() const;

private:
    MainController& controller;
};
