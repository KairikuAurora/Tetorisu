#include "headers/Settings.hpp"
#include <iostream>

Settings::Settings(currentGameState& state) : concerned_setting(SETTINGS_MENU)
{
    this->initKeys();
    this->initControls(state);

    std::ifstream resolutionFile("savedDatas/resolution.txt");

    if (resolutionFile.is_open())
    {
        resolutionFile >> state.windowWidth;
        resolutionFile >> state.windowHeight;
    }
    else
    {
        std::cerr << "Fichier des paramètres de l'écran inaccessible." << std::endl;
    }
}

/*--------------------------------------------------------------------------------------------------------------*/

void Settings::initKeys()
{
    std::ifstream keyFile("savedDatas/Keys.txt");
    std::string key;
    int key_value;

    if (keyFile.is_open())
    {
        // Assignation des touches à leurs noms et inversement
        while (keyFile >> key >> key_value)
        {
            this->inputKeys[key] = (sf::Keyboard::Key)key_value;
            this->keysInputs[key_value] = key;
        }

        keyFile.close();
    }
}

/*--------------------------------------------------------------------------------------------------------------*/

void Settings::initControls(currentGameState& state)
{
    std::ifstream controlsFile("savedDatas/controls.txt");
    int action;
    std::string key;

    if (controlsFile.is_open())
    {
        while (controlsFile >> action >> key)
        {
            state.controls[action] = this->inputKeys[key];
            this->keys[action] = key;
        }

        controlsFile.close();
    }
}

/*--------------------------------------------------------------------------------------------------------------*/

void Settings::changeControl(int control_to_change, int keyPressed, currentGameState& state)
{
    std::ofstream controlsFile("savedDatas/controls.txt");
    int i = 0;

    while (controlsFile.is_open() && i <= NB_KEYS)
    {
        if (i == control_to_change) controlsFile << i << " " << this->keysInputs[keyPressed] << std::endl;
        else controlsFile << i << " " << this->keysInputs[state.controls[i]] << std::endl;

        i++;
    }

    controlsFile.close();

    this->initControls(state);
}

/*--------------------------------------------------------------------------------------------------------------*/

void Settings::display(sf::RenderWindow& window, sf::View& view, currentGameState& state)
{
    switch (this->concerned_setting)
    {
    case SETTINGS_MENU:
        displayMenuSettings(window, state);
        break;
    case KEYS:
        displayKeysSettings(window, state);
        break;
    case SCREEN_RESOLUTION:
        displayResolutionSettings(window, state);
        break;
    }
}

/*--------------------------------------------------------------------------------------------------------------*/

void Settings::displayMenuSettings(sf::RenderWindow& window, currentGameState& state)
{
    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    Button back(BACK_BUTTON_X, BACK_BUTTON_Y, "BACK", state.font, 50, CLICKABLE);

    std::array<Button, 2> settings =
    {
        Button(window.getSize().x / 2, window.getSize().y / 2, "Bindings", state.font, 50, CLICKABLE),
        Button(window.getSize().x / 2, window.getSize().y / 2 + 100, "Resolution", state.font, 50, CLICKABLE)
    };

    back.display(window, mousePosition);
    for (unsigned char i = 0; i < settings.size(); i++) { settings[i].display(window, mousePosition); }

    if (back.isClicked(mousePosition, state.isLBMpressed)) state.state = GAME_MENU;
    else if (settings[KEYS].isClicked(mousePosition, state.isLBMpressed)) this->concerned_setting = KEYS;
    else if (settings[SCREEN_RESOLUTION].isClicked(mousePosition, state.isLBMpressed)) this->concerned_setting = SCREEN_RESOLUTION;
}

/*--------------------------------------------------------------------------------------------------------------*/

void Settings::displayKeysSettings(sf::RenderWindow& window, currentGameState& state)
{
    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    Button back(BACK_BUTTON_X, BACK_BUTTON_Y, "BACK", state.font, 50, CLICKABLE);

    // Initialisation des boutons uniquement affichables
    std::array<Button, 6> controls_names =
    {
        Button(INSTANT_DESCENT_X, INSTANT_DESCENT_Y,  "INSTANT DESCENT", state.font, 50, NOT_CLICKABLE),
        Button(QUICK_DESCENT_X,   QUICK_DESCENT_Y,    "QUICK DESCENT",   state.font, 50, NOT_CLICKABLE),
        Button(LEFT_ROTATE_X,     LEFT_ROTATE_Y,      "LEFT ROTATION",   state.font, 50, NOT_CLICKABLE),
        Button(RIGHT_ROTATE_X,    RIGHT_ROTATE_KEY_Y, "RIGHT ROTATION",  state.font, 50, NOT_CLICKABLE),
        Button(MOVE_LEFT_X,       MOVE_LEFT_Y,        "MOVE LEFT",       state.font, 50, NOT_CLICKABLE),
        Button(MOVE_RIGHT_X,      MOVE_RIGHT_Y,       "MOVE RIGHT",      state.font, 50, NOT_CLICKABLE)
    };

    // Initialisation des touches
    std::array<Button, 6> associated_keys =
    {
        Button(INSTANT_DESCENT_KEY_X, INSTANT_DESCENT_KEY_Y, keys[INSTANT_DESCENT], state.font, 50, NOT_CLICKABLE),
        Button(QUICK_DESCENT_KEY_X,   QUICK_DESCENT_KEY_Y,   keys[QUICK_DESCENT],   state.font, 50, NOT_CLICKABLE),
        Button(LEFT_ROTATE_KEY_X,     LEFT_ROTATE_KEY_Y,     keys[LEFT_ROTATION],   state.font, 50, NOT_CLICKABLE),
        Button(RIGHT_ROTATE_KEY_X,    RIGHT_ROTATE_KEY_Y,    keys[RIGHT_ROTATION],  state.font, 50, NOT_CLICKABLE),
        Button(MOVE_LEFT_KEY_X,       MOVE_LEFT_KEY_Y,       keys[MOVE_LEFT],       state.font, 50, NOT_CLICKABLE),
        Button(MOVE_RIGHT_KEY_X,      MOVE_RIGHT_KEY_Y,      keys[MOVE_RIGHT],      state.font, 50, NOT_CLICKABLE)
    };

    // Initialisation des boutons cliquables
    std::array<Button, 6> change_the_control_key =
    {
        Button(CHANGE_INSTANT_DESCENT_X, CHANGE_INSTANT_DESCENT_Y, "CHANGE", state.font, 50, CLICKABLE),
        Button(CHANGE_QUICK_DESCENT_X,   CHANGE_QUICK_DESCENT_Y,   "CHANGE", state.font, 50, CLICKABLE),
        Button(CHANGE_LEFT_ROTATE_X,     CHANGE_LEFT_ROTATE_Y,     "CHANGE", state.font, 50, CLICKABLE),
        Button(CHANGE_RIGHT_ROTATE_X,    CHANGE_RIGHT_ROTATE_Y,    "CHANGE", state.font, 50, CLICKABLE),
        Button(CHANGE_MOVE_LEFT_X,       CHANGE_MOVE_LEFT_Y,       "CHANGE", state.font, 50, CLICKABLE),
        Button(CHANGE_MOVE_RIGHT_X,      CHANGE_MOVE_RIGHT_Y,      "CHANGE", state.font, 50, CLICKABLE)
    };

    back.display(window, mousePosition);

    // Affichage des boutons
    for (unsigned char i = 0; i < controls_names.size(); i++)
    {
        controls_names[i].display(window, mousePosition);
        associated_keys[i].display(window, mousePosition);
        change_the_control_key[i].display(window, mousePosition);

        if (change_the_control_key[i].isClicked(mousePosition, state.isLBMpressed)) state.settingsControls = i;
    }

    if (back.isClicked(mousePosition, state.isLBMpressed)) this->concerned_setting = SETTINGS_MENU;
}

/*--------------------------------------------------------------------------------------------------------------*/

void Settings::displayResolutionSettings(sf::RenderWindow& window, currentGameState& state)
{
    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    Button back(BACK_BUTTON_X, BACK_BUTTON_Y, "BACK", state.font, 50, CLICKABLE);

    back.display(window, mousePosition);

    if (back.isClicked(mousePosition, state.isLBMpressed))
    {
        this->concerned_setting = SETTINGS_MENU;
    }
}

/*--------------------------------------------------------------------------------------------------------------*/