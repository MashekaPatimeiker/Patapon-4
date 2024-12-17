#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <Windows.h>
#include <fstream>

using namespace sf;
using namespace std;
int counter = 0;
std::string playerName1;
RenderWindow InitWindow();
Texture LoadTexture(const std::string& filePath);
Font LoadFont(const std::string& filePath);
void StartSettings();
void OpenFileNameNiga(std::string& playerName);
Text InitText(const Font& font, const std::string& str, int size, Color color, float x, float y);
void isGoing(RenderWindow& window, const Sprite& backgroundSprite, const Text& text, std::vector<Text>& buttonLabels, Music& music);
void CreateLabels(float height, const Font& font, std::vector<Text>& buttonLabels);
void StartNewGame(RenderWindow& window, Music& music);
void MoveUnits(Sprite& spearMan, float distance);
//void MoveUnitsBackward(Sprite& spearMan, int unitCount, float distance);
void LoadBackground(Sprite& backgroundSprite);
void CreateUnits(Texture& pngTexture, Sprite& spearMan, float scaleFactor, float height);
//void DrawUnits(RenderWindow& window, const Sprite &spearMan, int unitCount);
Text CreateQuitLabel(const Font& font, float width, float height);
void HandleEvents(Sprite& backgroundSprite,Sprite& kaChik, RenderWindow& window, Text& quitLabel, Sprite& spearMan, Music& music, Sound& sound1, Sound& sound2, Sound& sound3, Sound& sound4);
void InitializeSounds(SoundBuffer& soundBuffer1, SoundBuffer& soundBuffer2,SoundBuffer& soundBuffer3, SoundBuffer& soundBuffer4, Sound& sound1, Sound& sound2, Sound& sound3, Sound& sound4);
//bool CheckInputSequence(char input, std::string& sequence);
void SavePlayerName(const std::string& playerName);
//float currentFrame = 0;
int main()
{
    RenderWindow window = InitWindow();
    Texture texture_window = LoadTexture("images/233229.jpg");

    Sprite backgroundSprite(texture_window);
    float width = static_cast<float>(window.getSize().x);
    float height = static_cast<float>(window.getSize().y);
    backgroundSprite.setScale(width / static_cast<float>(texture_window.getSize().x), height / static_cast<float>(texture_window.getSize().y));

    Font font = LoadFont("font/Patapon.ttf");
    Text text = InitText(font, "Patapon", 300, Color(169, 169, 169), width / 2, -80);

    Music music;
    if (!music.openFromFile("music/fonmain.ogg"))
    {
        std::cerr << "Error: Could not open sound file from " << "music/fonmain.ogg" << std::endl;
        exit(-1);
    }
    music.setLoop(true);
    music.play();
    vector<Text> buttonLabels;
    CreateLabels(height, font, buttonLabels);

    isGoing(window, backgroundSprite, text, buttonLabels, music);

    return EXIT_SUCCESS;
}

RenderWindow InitWindow()
{
    return RenderWindow(VideoMode::getDesktopMode(), "Patapon", Style::Fullscreen);
}

Texture LoadTexture(const std::string& filePath)
{
    Texture texture;
    if (!texture.loadFromFile(filePath))
    {
        std::cerr << "Error: Could not load texture from " << filePath << std::endl;
        exit(-1);
    }
    return texture;
}

Font LoadFont(const std::string& filePath)
{
    Font font;
    if (!font.loadFromFile(filePath))
    {
        cerr << "Error: Could not load font from " << filePath << std::endl;
        exit(-1);
    }
    return font;
}

Text InitText(const Font& font, const std::string& str, int size, Color color, float x, float y)
{
    Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(x - text.getGlobalBounds().width / 2, y);
    return text;
}

void CreateLabels(float height, const Font& font, std::vector<Text>& buttonLabels)
{
    const float labelSpacing = 45.f;
    const float startX = 160.f;
    const float startY = height - 340.f;
    vector<std::string> labelTexts = {
        "New game",
        "Continue",
        "Settings",
        "Quit"
    };

    for (size_t i = 0; i < labelTexts.size(); ++i) {
        Text label = InitText(font, labelTexts[i], 60, Color::White, startX, startY + i * (labelSpacing + 30));
        buttonLabels.push_back(label);
    }
}

void CreateUnits(Texture& pngTexture, Sprite& spearMan, float scaleFactor, float height) {
    spearMan.setTexture(pngTexture);
    spearMan.setScale(scaleFactor, scaleFactor);
    spearMan.setPosition(30 + 1 * (static_cast<float>(pngTexture.getSize().x) * scaleFactor + 10), (height - static_cast<float>(pngTexture.getSize().y) * scaleFactor) / 2 + 307); // Размещение в ряд с отступом
}
void DrawUnits(RenderWindow& window, Sprite& spearMan) {
    window.draw(spearMan);
}

void MoveUnits(Sprite& spearMan, float distance) {
    spearMan.move(distance, 0);
}

Text CreateQuitLabel(const Font& font, float width, float height) {
    Text quitLabel;
    quitLabel.setFont(font);
    quitLabel.setString("Quit");
    quitLabel.setCharacterSize(60);
    quitLabel.setFillColor(Color::White);
    quitLabel.setPosition(width - 150, 30);
    return quitLabel;
}

void InitializeSounds(SoundBuffer& soundBuffer1, SoundBuffer& soundBuffer2,SoundBuffer& soundBuffer3, SoundBuffer& soundBuffer4, Sound& sound1, Sound& sound2,  Sound& sound3,  Sound& sound4) {
    if (!soundBuffer1.loadFromFile("music/Pata-1.wav")) {
        std::cerr << "Error: Could not open sound file for sound1." << std::endl;
        exit(-1);
    }
    if (!soundBuffer2.loadFromFile("music/Pon-1.wav")) {
        std::cerr << "Error: Could not open sound file for sound2." << std::endl;
        exit(-1);
    }
    if (!soundBuffer3.loadFromFile("music/Don-1.wav")) {
        std::cerr << "Error: Could not open sound file for sound2." << std::endl;
        exit(-1);
    }
    if (!soundBuffer4.loadFromFile("music/Chaka-1.wav")) {
        std::cerr << "Error: Could not open sound file for sound2." << std::endl;
        exit(-1);
    }
    sound1.setBuffer(soundBuffer1);
    sound2.setBuffer(soundBuffer2);
    sound3.setBuffer(soundBuffer3);
    sound4.setBuffer(soundBuffer4);
}

void LoadBackground(Sprite& backgroundSprite) {
    static Texture newGameTexture;
    if (!newGameTexture.loadFromFile("images/FONIMAGE.png")) {
        std::cerr << "Error: Could not load background texture." << std::endl;
        exit(-1);
    }
    backgroundSprite.setTexture(newGameTexture);
    backgroundSprite.setPosition(0, -80);
}

void UpdateBackgroundPosition(Sprite& spearMan, Sprite& kaChik, int unitCount, Sprite& backgroundSprite, float playerMovement, const RenderWindow& window) {
    const Texture* texture = backgroundSprite.getTexture();
    if (!texture) {
        std::cerr << "Error: Background sprite has no texture." << std::endl;
        return;
    }

    float textureWidth = (static_cast<float>(texture->getSize().x));
    float windowWidth = (static_cast<float>(window.getSize().x) );

    float currentX = backgroundSprite.getPosition().x;
    if (playerMovement > 0 && currentX <= -(textureWidth - windowWidth)) {
        MoveUnits(spearMan, 500.0f);
        return;
    } else if (playerMovement < 0 && currentX >= 0) {
        MoveUnits(spearMan, 500.0f);
        return;
    }
    backgroundSprite.move(-playerMovement, 0);
}

void CreateKaChik(Texture& pngTexture, Sprite& spearMan, float scaleFactor, float height) {
    spearMan.setTexture(pngTexture);
    spearMan.setScale(scaleFactor, scaleFactor);
    spearMan.setPosition(400 + 1 * (static_cast<float>(pngTexture.getSize().x) * scaleFactor + 10), (height - (static_cast<float>(pngTexture.getSize().y)) * scaleFactor) / 2 + 307); // Размещение в ряд с отступом
}
void StartNewGame(RenderWindow& window, Music& music) {
    music.stop();
    Music music1;
    if (!music1.openFromFile("music/01Gyorocchi.ogg")) {
        std::cerr << "Error: Could not open sound file from music/01Gyorocchi.ogg" << std::endl;
        exit(-1);
    }
    music1.setLoop(true);
    music1.play();

    Sprite newGameBackgroundSprite;
    LoadBackground(newGameBackgroundSprite);
    Texture pngTexture = LoadTexture("images/patapon_mark_2.png");
    Texture pngText = LoadTexture("images/kachik.png");
    float scaleFactor = 0.25f;
    Sprite spearMan;
    Sprite kaChik;
    float backgroundWidth = newGameBackgroundSprite.getGlobalBounds().width - 1600.0f;
    CreateUnits(pngTexture, spearMan, scaleFactor, static_cast<float>(window.getSize().y));
    CreateKaChik(pngText, kaChik, scaleFactor, static_cast<float>(window.getSize().y) - 75);
    Font font = LoadFont("font/Patapon.ttf");
    Text quitLabel = CreateQuitLabel(font, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    float pulseSize = 0.0f;
    float pulseMaxSize = 200.0f;
    float pulseSpeed = 400.0f;
    Clock clock;
    SoundBuffer soundBuffer1, soundBuffer2, soundBuffer3, soundBuffer4;
    Sound sound1, sound2, sound3, sound4;
    InitializeSounds(soundBuffer1, soundBuffer2, soundBuffer3, soundBuffer4, sound1, sound2, sound3, sound4);
    while (window.isOpen()) {
        HandleEvents(newGameBackgroundSprite, kaChik, window, quitLabel, spearMan, music, sound1, sound2, sound3, sound4); // Handle events

        float deltaTime = clock.restart().asSeconds();
        pulseSize += pulseSpeed * deltaTime;
        RectangleShape pulse(Vector2f(pulseSize, 10));
        if (pulseSize >= pulseMaxSize) {
            pulseSize = 0.0f;
        }
        pulse.setFillColor(Color::White);
        pulse.setOrigin(pulseSize / 2, 5);
        pulse.setPosition(static_cast<float>(window.getSize().x) / 2, 20);
        window.clear();
        window.draw(newGameBackgroundSprite);
        DrawUnits(window, spearMan);
        window.draw(quitLabel);
        window.draw(pulse);
        if (counter < 4) {
            window.draw(kaChik);
        }


        if (spearMan.getPosition().x >= backgroundWidth) {
            MessageBoxA(nullptr, "Good Game: SpearMan reached the end of the background and killed some kaChik!", "Game Over", MB_OK | MB_ICONASTERISK);

            music1.stop();
            window.close();
            return;
        }

        window.display();
    }
}

void HandleEvents(Sprite& backgroundSprite,Sprite& kaChik, RenderWindow& window, Text& quitLabel, Sprite& spearMan, Music& music, Sound& sound1, Sound& sound2, Sound& sound3, Sound& sound4) {
    Event event{};
    static std::string inputSequence;
    static float lastBeatTime = 0.0f;
    static const float beatInterval = 60.0f / 120.0f;
    static const float timingWindow = 0.2f;
    bool musicPlayed = false;
    Clock clock;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed)
            window.close();

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(window);
            FloatRect quitLabelBounds = quitLabel.getGlobalBounds();
            if (quitLabelBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                window.close();
            }
        }
        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::Num1) {
                inputSequence += '1';
                sound1.play();
            } else if (event.key.code == Keyboard::Num2) {
                inputSequence += '2';
                sound2.play();
            } else if (event.key.code == Keyboard::Num3) {
                inputSequence += '3';
                sound3.play();
                inputSequence.clear();
            } else if (event.key.code == Keyboard::Num4) {
                inputSequence += '4';
                sound4.play();
                inputSequence.clear();
            }

            if (inputSequence == "1112") {
                UpdateBackgroundPosition(spearMan, kaChik, 1, backgroundSprite, 400.0f, window);
                inputSequence.clear();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                if (!musicPlayed) {
                    if (!music.openFromFile("music/00March-01.wav")) {
                        std::cerr << "Error: Could not open sound file." << std::endl;
                        exit(-1);
                    }
                    music.setLoop(false);
                    music.play();
                    lastBeatTime = clock.getElapsedTime().asSeconds(); // Update last beat time
                    musicPlayed = true;
                    MoveUnits(kaChik, -100.0f);

                }
                inputSequence.clear();
            } else if (inputSequence == "2211") {
                UpdateBackgroundPosition(spearMan, kaChik, 1, backgroundSprite, -100.0f, window);
                inputSequence.clear();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                if (!musicPlayed) {
                    if (!music.openFromFile("music/00Retreat-03.wav")) {
                        std::cerr << "Error: Could not open sound file." << std::endl;
                        exit(-1);
                    }
                    music.setLoop(false);
                    music.play();
                    lastBeatTime = clock.getElapsedTime().asSeconds();
                    musicPlayed = true;
                    MoveUnits(kaChik, 100.0f);
                }
                inputSequence.clear();
            }
// Inside your input handling logic
            else if (inputSequence == "2212") {

                Texture spearTexture;
                float pulseSize = 0.0f;
                float pulseMaxSize = 200.0f; // Maximum pulse size
                float pulseSpeed = 400.0f; // Pulse growth speed
                float deltaTime = clock.restart().asSeconds();
                pulseSize += pulseSpeed * deltaTime;
                RectangleShape pulse(Vector2f(pulseSize, 10));
                if (pulseSize >= pulseMaxSize) {
                    pulseSize = 0.0f;
                }
                pulse.setFillColor(Color::White);
                pulse.setOrigin(pulseSize / 2, 5);
                pulse.setPosition(static_cast<float>(window.getSize().x) / 2, 20);

                if (!spearTexture.loadFromFile("images/spear.png")) {
                    std::cerr << "Error: Could not load spear texture." << std::endl;
                    exit(-1);
                }

                Sprite speargod;
                speargod.setTexture(spearTexture);
                speargod.setScale(0.25f, 0.25f);
                speargod.setPosition(spearMan.getPosition().x, spearMan.getPosition().y - 100.0f); // Positioning the spear

                // Flight parameters
                float flightDistance = 500.0f; // Distance to fly
                float flightSpeed = 400.0f; // Increased speed of the flight
                float currentDistance = 0.0f; // Distance traveled
                std::this_thread::sleep_for(std::chrono::milliseconds(450));

                if (!musicPlayed) {
                    if (!music.openFromFile("music/17Attack-02.wav")) {
                        std::cerr << "Error: Could not open sound file." << std::endl;
                        exit(-1);
                    }
                    music.setLoop(false);
                    music.play();
                    lastBeatTime = clock.getElapsedTime().asSeconds();
                    musicPlayed = true;
                }

                // Main loop for the flight
                while (currentDistance < flightDistance) {
                    // Handle events (this is crucial for window responsiveness)
                    sf::Event eventer{};
                    while (window.pollEvent(eventer)) {
                        if (eventer.type == sf::Event::Closed)
                            window.close();
                    }

                    speargod.move(flightSpeed * 0.016f, 0); // Move right along the x-axis
                    currentDistance += flightSpeed * 0.016f;

                    window.draw(backgroundSprite);
                    DrawUnits(window, spearMan);

                    window.draw(quitLabel);
                    window.draw(pulse);
                    window.draw(speargod);
                    if (counter <4) {
                        window.draw(kaChik);
                    }
                    window.display();
                    window.clear();
                    std::this_thread::sleep_for(std::chrono::milliseconds(8));
                    inputSequence.clear();
                }
                counter++;
            }
            }
                if (inputSequence.length() > 4) {
                inputSequence.clear();
            }
        }
        Vector2i mousePos = Mouse::getPosition(window);
        FloatRect quitLabelBounds = quitLabel.getGlobalBounds();
        if (quitLabelBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            quitLabel.setFillColor(Color::Red);
        } else {
            quitLabel.setFillColor(Color::White);
        }
    }



void isGoing(RenderWindow& window, const Sprite& backgroundSprite, const Text& text, std::vector<Text>& buttonLabels, Music& music)
{
    Texture spearTexture;
    if (!spearTexture.loadFromFile("images/rapgod.png")) {
        std::cerr << "Error: Could not load spear texture." << std::endl;
    }

    Sprite speargod;

    speargod.setTexture(spearTexture);

    speargod.setScale(0.25f, 0.25f);

    speargod.setPosition(100, 300);
    auto lastTime = std::chrono::steady_clock::now();
    while (window.isOpen())
    {

        Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                FloatRect newGameLabelBounds = buttonLabels[0].getGlobalBounds();
                if (newGameLabelBounds.contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    StartNewGame(window, music);
                }
                FloatRect settingsLabelBounds = buttonLabels[2].getGlobalBounds();
                if (settingsLabelBounds.contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    StartSettings();
                }
                FloatRect quitLabelBounds = buttonLabels.back().getGlobalBounds();
                if (quitLabelBounds.contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    window.close();
                }
            }
        }

        Vector2i mousePos = Mouse::getPosition(window);
        for (auto& label : buttonLabels) {
            FloatRect labelBounds = label.getGlobalBounds();
            if (labelBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                label.setFillColor(Color::Red);
            } else {
                label.setFillColor(Color::White);
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(text);
        for (const auto& label : buttonLabels) {
            window.draw(label);
        }
        window.display();
    }
}
void LoadBackgroundSettings(Sprite& backgroundSprite) {
    static Texture newGameTexture;
    if (!newGameTexture.loadFromFile("images/1332252.png")) {
        std::cerr << "Error: Could not load background texture." << std::endl;
        exit(-1);
    }
    backgroundSprite.setTexture(newGameTexture);
    backgroundSprite.setPosition(0, -80);
}

Text CreateInputText(const Font& font, const RenderWindow& window) {
    Text inputText("", font, 50);
    inputText.setFillColor(Color::White);
    inputText.setPosition(static_cast<float>(window.getSize().x) / 2 - 150, static_cast<float>(window.getSize().y) / 2 - 120);
    return inputText;
}

Text CreateNamePrompt(const Font& font, const RenderWindow& window) {
    Text namePrompt("SAY AS YOUR NAME:", font, 50);
    namePrompt.setFillColor(Color::White);
    namePrompt.setPosition(static_cast<float>(window.getSize().x) / 2 - 200, static_cast<float>(window.getSize().y) / 2 - 200);
    return namePrompt;
}

RectangleShape CreateSubmitButton(const Font& font, const RenderWindow& window) {
    std::string buttonTextStr = "Submit";
    Text buttonText(buttonTextStr, font, 25);
    buttonText.setFillColor(Color::Black);

    float buttonWidth = buttonText.getGlobalBounds().width + 250;
    float buttonHeight = buttonText.getGlobalBounds().height + 100;
    RectangleShape button(Vector2f(buttonWidth, buttonHeight));
    button.setFillColor(Color::Blue);
    button.setPosition(static_cast<float>(window.getSize().x) / 2 - buttonWidth / 2,
                       static_cast<float>(window.getSize().y) / 2 - buttonHeight / 2);

    // Center the text within the button
    buttonText.setPosition(static_cast<float>(window.getSize().x) / 2 - buttonWidth / 2 + (buttonWidth - buttonText.getGlobalBounds().width) / 2,
                           static_cast<float>(window.getSize().y) / 2 - buttonHeight / 2 + (buttonHeight - buttonText.getGlobalBounds().height) / 2);

    return button;
}



RectangleShape CreateSaveButton(const Font& font) {
    Text saveButtonText("Save", font, 25);
    saveButtonText.setFillColor(Color::Green);

    float saveButtonWidth = saveButtonText.getGlobalBounds().width +20;
    float saveButtonHeight = saveButtonText.getGlobalBounds().height + 20;
    RectangleShape saveButton(Vector2f(saveButtonWidth, saveButtonHeight));
    saveButton.setFillColor(Color::Blue);
    saveButton.setPosition(0, 0); // Позиция в верхнем левом углу

    saveButtonText.setPosition(50,50);
    return saveButton;
}
RectangleShape CreateOpenFileButton(const Font& font) {
    Text saveButtonText("Open", font, 25);
    saveButtonText.setFillColor(Color::Green);

    float saveButtonWidth = saveButtonText.getGlobalBounds().width +10;
    float saveButtonHeight = saveButtonText.getGlobalBounds().height + 20;
    RectangleShape saveButton(Vector2f(saveButtonWidth, saveButtonHeight));
    saveButton.setFillColor(Color::Blue);
    saveButton.setPosition(0, 40); // Позиция в верхнем левом углу

    saveButtonText.setPosition(50,50);
    return saveButton;
}
Text CreateButtonText(const std::string& text, const Font& font) {
    Text buttonText(text, font, 25);
    buttonText.setFillColor(Color::Green);

    return buttonText;
}
Text CreateButtonText1(const std::string& text, const Font& font) {
    Text buttonText(text, font, 25);
    buttonText.setFillColor(Color::Green);
    buttonText.setPosition(0, 40);
    return buttonText;
}
Text CreateNameLabel(const Font& font, const RenderWindow& window) {
    Text nameLabel("", font, 50);
    nameLabel.setFillColor(Color::White);
    nameLabel.setPosition(static_cast<float>(window.getSize().x) / 2 - 200, static_cast<float>(window.getSize().y) / 2 + 100);
    return nameLabel;
}

void HandleEvents(RenderWindow& settingsWindow, std::string& playerName, Text& inputText, Text& quitLabel, RectangleShape& button, Text& nameLabel, RectangleShape& saveButton, RectangleShape& openfileButton) {
    Event event{};
    nameLabel.setString("OUR HERO NAME: " + playerName1);
    while (settingsWindow.pollEvent(event)) {
        if (event.type == Event::Closed) {
            settingsWindow.close();
        }
        if (event.type == Event::TextEntered) {
            if (event.text.unicode < 128) {
                if (event.text.unicode == '\b') {
                    if (!playerName.empty()) {
                        playerName.pop_back();
                    }
                } else {
                    playerName += static_cast<char>(event.text.unicode);
                }
                inputText.setString(playerName);
            }
        }
        if (event.type == Event::MouseButtonPressed) {
            Vector2i mousePos = Mouse::getPosition(settingsWindow);
            FloatRect quitLabelBounds = quitLabel.getGlobalBounds();
            if (quitLabelBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                settingsWindow.close();
            }
            if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                nameLabel.setString("OUR HERO NAME: " + playerName);
                playerName1 = playerName;
            }
            if (saveButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                SavePlayerName(playerName);
            }
            if (openfileButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                OpenFileNameNiga(playerName);
                nameLabel.setString("OUR HERO NAME: " + playerName);
                playerName1 = playerName;
            }
        }
    }
}

void RenderingWindow(RenderWindow& settingsWindow,const RectangleShape& openbutton,const Text& openButtonText, const Sprite& newGameBackgroundSprite, Text& quitLabel, const Text& inputText, const RectangleShape& button, const RectangleShape& saveButton, const Text& saveButtonText, const Text& nameLabel, const Text& namePrompt) {
    settingsWindow.clear();
    settingsWindow.draw(newGameBackgroundSprite);
    settingsWindow.draw(quitLabel);
    settingsWindow.draw(inputText);
    settingsWindow.draw(button);
    settingsWindow.draw(saveButton);
    settingsWindow.draw(saveButtonText);
    settingsWindow.draw(nameLabel);
    settingsWindow.draw(namePrompt);
    settingsWindow.draw(openbutton);
    settingsWindow.draw(openButtonText);
    settingsWindow.display();
    Vector2i mousePos = Mouse::getPosition(settingsWindow);
    FloatRect quitLabelBounds = quitLabel.getGlobalBounds();
    if (quitLabelBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        quitLabel.setFillColor(Color::Red);
    } else {
        quitLabel.setFillColor(Color::White);
    }
}
void StartSettings() {
    RenderWindow settingsWindow(VideoMode::getDesktopMode(), "Settings", Style::Resize);
    Sprite newGameBackgroundSprite;
    LoadBackgroundSettings(newGameBackgroundSprite);
    Font font = LoadFont("font/Patapon.ttf");

    Text quitLabel = CreateQuitLabel(font, static_cast<float>(settingsWindow.getSize().x), static_cast<float>(settingsWindow.getSize().y));
    std::string playerName;
    Text inputText = CreateInputText(font, settingsWindow);
    Text namePrompt = CreateNamePrompt(font, settingsWindow);
    RectangleShape button = CreateSubmitButton(font, settingsWindow);
    RectangleShape saveButton = CreateSaveButton(font);
    Text saveButtonText = CreateButtonText("Save", font);
    Text nameLabel = CreateNameLabel(font, settingsWindow);
    RectangleShape openbutton = CreateOpenFileButton(font);
    Text openButtonText = CreateButtonText1("Open", font);
    while (settingsWindow.isOpen()) {
        HandleEvents(settingsWindow, playerName, inputText, quitLabel, button, nameLabel, saveButton, openbutton);
        RenderingWindow(settingsWindow,openbutton,openButtonText, newGameBackgroundSprite, quitLabel, inputText, button, saveButton, saveButtonText, nameLabel, namePrompt);
    }
}
void SavePlayerName(const std::string& playerName) {
    std::ofstream outFile("files/users.txt");
    if (outFile.is_open()) {
        outFile << playerName;
        outFile.close();
        MessageBoxA(nullptr, "Saving is complete successfully!", "Saving", MB_OK | MB_ICONASTERISK);
    } else {
        std::cerr << "Ошибка: не удалось открыть файл для записи." << std::endl;
    }
}
void OpenFileNameNiga(std::string& playerName) {
    std::string filePath = "files/users.txt";
    std::ifstream inputFile(filePath);
    if (inputFile.is_open()) {
        std::getline(inputFile, playerName);
        inputFile.close();
    } else {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        playerName = "";
    }
}