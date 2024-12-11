#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
using namespace sf;
using namespace std;
class Spear {
public:
    Sprite sprite; // The sprite representing the spear
    Vector2f position; // Current position of the spear
    float speed; // Speed of the spear

    Spear(Texture& texture, Vector2f startPosition, float spearSpeed) {
        sprite.setTexture(texture);
        position = startPosition;
        sprite.setPosition(position);
        speed = spearSpeed;
    }

    void update() {
        position.x += speed; // Move the spear in the x direction
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }
};

Spear* spear = nullptr;

bool spearInFlight = false;

RenderWindow InitWindow();
Texture LoadTexture(const std::string& filePath);
Font LoadFont(const std::string& filePath);
Text InitText(const Font& font, const std::string& str, int size, Color color, float x, float y);
void isGoing(RenderWindow& window, const Sprite& backgroundSprite, const Text& text, std::vector<Text>& buttonLabels, Music& music);
void CreateLabels(float height, const Font& font, std::vector<Text>& buttonLabels);
void StartNewGame(RenderWindow& window, Music& music);
void MoveUnits(Sprite& spearMan, int unitCount, float distance);
void MoveUnitsBackward(Sprite& spearMan, int unitCount, float distance);
void LoadBackground(const RenderWindow& window, Sprite& backgroundSprite);
void CreateUnits(Texture& pngTexture, Sprite& spearMan, int unitCount, float scaleFactor, float height);
void DrawUnits(RenderWindow& window, const Sprite &spearMan, int unitCount);
Text CreateQuitLabel(const Font& font, float width, float height);
void HandleEvents(Sprite& backgroundSprite,Sprite& kaChik, RenderWindow& window, Text& quitLabel, Sprite& spearMan, int unitCount, Music& music, Sound& sound1, Sound& sound2, Sound& sound3, Sound& sound4);
void InitializeSounds(SoundBuffer& soundBuffer1, SoundBuffer& soundBuffer2,SoundBuffer& soundBuffer3, SoundBuffer& soundBuffer4, Sound& sound1, Sound& sound2, Sound& sound3, Sound& sound4);
bool CheckInputSequence(char input, std::string& sequence);
float currentFrame = 0;
int main()
{
    RenderWindow window = InitWindow();
    Texture texture_window = LoadTexture("images/233229.jpg");

    Sprite backgroundSprite(texture_window);
    float width = window.getSize().x;
    float height = window.getSize().y;
    backgroundSprite.setScale(width / texture_window.getSize().x, height / texture_window.getSize().y);

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

void CreateUnits(Texture& pngTexture, Sprite& spearMan, int unitCount, float scaleFactor, float height) {
    spearMan.setTexture(pngTexture);
    spearMan.setScale(scaleFactor, scaleFactor);
    spearMan.setPosition(30 + 1 * (pngTexture.getSize().x * scaleFactor + 10), (height - pngTexture.getSize().y * scaleFactor) / 2 + 307); // Размещение в ряд с отступом
}
void DrawUnits(RenderWindow& window, Sprite& spearMan, int unitCount) {
    window.draw(spearMan);
}

void MoveUnits(Sprite& spearMan, int unitCount, float distance) {
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

void MoveUnitsBackward(Sprite& spearMan, int unitCount, float distance) {
    spearMan.move(-distance, 0);

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

void LoadBackground(const RenderWindow& window, Sprite& backgroundSprite) {
    static Texture newGameTexture;
    if (!newGameTexture.loadFromFile("images/FONIMAGE.png")) {
        std::cerr << "Error: Could not load background texture." << std::endl;
        exit(-1);
    }
    backgroundSprite.setTexture(newGameTexture);
    backgroundSprite.setPosition(0, -80);
}


// Function to update the background position based on player movement
void UpdateBackgroundPosition(Sprite& spearMan, Sprite& kaChik, int unitCount, Sprite& backgroundSprite, float playerMovement, const RenderWindow& window) {
    // Get the texture size and the window size
    const Texture* texture = backgroundSprite.getTexture();
    if (!texture) {
        std::cerr << "Error: Background sprite has no texture." << std::endl;
        return;
    }

    float textureWidth = texture->getSize().x;
    float windowWidth = window.getSize().x;

    // Get the current position of the background sprite
    float currentX = backgroundSprite.getPosition().x;

    // Check if the background can still move
    if (playerMovement > 0 && currentX <= -(textureWidth - windowWidth)) {
        MoveUnits(spearMan, unitCount, 500.0f);
        return;
    } else if (playerMovement < 0 && currentX >= 0) {
        MoveUnits(spearMan, unitCount, 500.0f);
        return;
    }
    // Move the background sprite to create a scrolling effect
    backgroundSprite.move(-playerMovement, 0);
}

void CreateKaChik(Texture& pngTexture, Sprite& spearMan, int unitCount, float scaleFactor, float height) {
    spearMan.setTexture(pngTexture);
    spearMan.setScale(scaleFactor, scaleFactor);
    spearMan.setPosition(400 + 1 * (pngTexture.getSize().x * scaleFactor + 10), (height - pngTexture.getSize().y * scaleFactor) / 2 + 307); // Размещение в ряд с отступом
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
    LoadBackground(window, newGameBackgroundSprite);
    Texture pngTexture = LoadTexture("images/patapon_mark_2.png");
    Texture pngText = LoadTexture("images/kachik.png");
    float scaleFactor = 0.25f;
    const int unitCount = 1;
    Sprite spearMan;
    Sprite kaChik;
    float backgroundWidth = newGameBackgroundSprite.getGlobalBounds().width - 1600.0f;
    CreateUnits(pngTexture, spearMan, unitCount, scaleFactor, window.getSize().y);
    CreateKaChik(pngText, kaChik, 2, scaleFactor, window.getSize().y - 75);
    Font font = LoadFont("font/Patapon.ttf");
    Text quitLabel = CreateQuitLabel(font, window.getSize().x, window.getSize().y);

    // Variables for pulse effect
    float pulseSize = 0.0f;
    float pulseMaxSize = 200.0f; // Maximum pulse size
    float pulseSpeed = 400.0f; // Pulse growth speed

    Clock clock; // Create a Clock object to track time

    SoundBuffer soundBuffer1, soundBuffer2, soundBuffer3, soundBuffer4;
    Sound sound1, sound2, sound3, sound4;
    InitializeSounds(soundBuffer1, soundBuffer2, soundBuffer3, soundBuffer4, sound1, sound2, sound3, sound4);

    while (window.isOpen()) {
        HandleEvents(newGameBackgroundSprite, kaChik, window, quitLabel, spearMan, unitCount, music, sound1, sound2, sound3, sound4); // Handle events

        // Update pulse effect
        float deltaTime = clock.restart().asSeconds(); // Get time since last frame
        pulseSize += pulseSpeed * deltaTime; // Use deltaTime for smoothness

        // Draw pulse effect as a white bar
        RectangleShape pulse(Vector2f(pulseSize, 10));
        if (pulseSize >= pulseMaxSize) {
            pulseSize = 0.0f; // Reset pulse size
        }
        pulse.setFillColor(Color::White); // White color
        pulse.setOrigin(pulseSize / 2, 5); // Center vertically
        pulse.setPosition(window.getSize().x / 2, 20); // Position at the top of the window

        // Clear the window
        window.clear();
        window.draw(newGameBackgroundSprite);
        DrawUnits(window, spearMan, unitCount);
        window.draw(kaChik);
        window.draw(quitLabel);
        window.draw(pulse); // Draw pulse effect

        // Check if spearMan has reached the end of the background
        if (spearMan.getPosition().x >= backgroundWidth) {
            std::cout << "Game Over: SpearMan reached the end of the background!" << std::endl;
            music1.stop(); // Stop the music
            window.close(); // Close the window
            return; // Exit the function
        }

        window.display(); // Display the contents of the window
    }
}
void HandleEvents(Sprite& backgroundSprite,Sprite& kaChik, RenderWindow& window, Text& quitLabel, Sprite& spearMan, int unitCount, Music& music, Sound& sound1, Sound& sound2, Sound& sound3, Sound& sound4) {
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
            } else if (event.key.code == Keyboard::Num4) {
                inputSequence += '4';
                sound4.play();
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
                    MoveUnits(kaChik, unitCount, -100.0f);
                }
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
                    MoveUnits(kaChik, unitCount, 100.0f);
                }
            }else if (inputSequence == "2212") {
                Texture spearTexture;
                if (!spearTexture.loadFromFile("images/rapgod.png")) {
                    std::cerr << "Error: Could not load spear texture." << std::endl;
                    exit(-1);
                }

                Sprite speargod;
                speargod.setTexture(spearTexture);
                speargod.setScale(0.25f, 0.25f);
                speargod.setPosition(spearMan.getPosition().x, spearMan.getPosition().y-100.0f); // Positioning the spear

                // Flight parameters
                float flightDistance = 500.0f; // Distance to fly
                float flightSpeed = 300.0f; // Increased speed of the flight
                float currentDistance = 0.0f; // Distance traveled

                // Main loop for the flight
                while (currentDistance < flightDistance) {
                    // Handle events (this is crucial for window responsiveness)
                    sf::Event event{};
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }

                    // Update the position of the spear
                    speargod.move(flightSpeed * 0.016f, 0); // Move right along the x-axis
                    currentDistance += flightSpeed * 0.016f;

                    window.draw(backgroundSprite);
                    DrawUnits(window, spearMan, unitCount);
                    window.draw(kaChik);
                    window.draw(quitLabel);


                    window.draw(speargod);

                    window.display();
                    window.clear();
                    // Simulate a short delay for the sake of this example
                    std::this_thread::sleep_for(std::chrono::milliseconds(8)); // Simulate frame delay
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
    speargod.setPosition(100, 300); // Установите начальную позицию копья

    // Used for timing
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
