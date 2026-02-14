#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

class MusicPlayer {
private:
    sf::RenderWindow window;
    sf::Music music;
    sf::Font font;
    std::optional<sf::Texture> background;
    std::optional<sf::Sprite> bgSprite;
    
    struct Song {
        std::string title;
        std::string path;
    };
    
    std::vector<Song> playlist;
    size_t currentIdx = 0;

public:
    MusicPlayer() : window(sf::VideoMode({600, 400}), "Simple Music Player") {
        window.setFramerateLimit(60);
    }

    bool init() {
        if (!font.openFromFile("../assets/fonts/arial.ttf")) {
            std::cerr << "Font not found at ../assets/fonts/arial.ttf" << std::endl;
            return false;
        }
        
        sf::Texture tempTexture;
        if (!tempTexture.loadFromFile("../assets/images/image.jpg")) {
            std::cout << "Background not found, using flat color.\n";
        } else {
            background = std::move(tempTexture);
            bgSprite.emplace(background.value());
        }

        playlist = {
            {"Song 1", "../assets/music/song1.mp3"},
            {"Song 2", "../assets/music/song2.mp3"},
            {"Song 3", "../assets/music/song3.mp3"}
        };
        return true;
    }

    void play() {
        if (playlist.empty()) return;
        if (music.openFromFile(playlist[currentIdx].path)) {
            music.play();
            std::cout << "Playing: " << playlist[currentIdx].title << std::endl;
        } else {
            std::cerr << "Failed to load: " << playlist[currentIdx].path << std::endl;
        }
    }

    void run() {
        play(); 
        
        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Space) {
                        if (music.getStatus() == sf::Music::Status::Playing) {
                            music.pause();
                            std::cout << "Paused" << std::endl;
                        } else {
                            music.play();
                            std::cout << "Resumed" << std::endl;
                        }
                    }
                    if (keyPressed->code == sf::Keyboard::Key::N) {
                        currentIdx = (currentIdx + 1) % playlist.size();
                        play();
                    }
                    if (keyPressed->code == sf::Keyboard::Key::P) {
                        if (currentIdx == 0) {
                            currentIdx = playlist.size() - 1;
                        } else {
                            currentIdx--;
                        }
                        play();
                    }
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                }
            }

            if (music.getStatus() == sf::Music::Status::Stopped && !playlist.empty()) {
                currentIdx = (currentIdx + 1) % playlist.size();
                play();
            }

            window.clear(sf::Color(30, 30, 30));
            
            if (bgSprite.has_value()) {
                window.draw(bgSprite.value());
            }
            
            sf::Text titleText(font, playlist[currentIdx].title, 24);
            titleText.setPosition({50.f, 50.f});
            titleText.setFillColor(sf::Color::White);
            window.draw(titleText);
            
            sf::Text controlsText(font, "Controls: SPACE=Play/Pause | N=Next | P=Previous | ESC=Exit", 16);
            controlsText.setPosition({50.f, 350.f});
            controlsText.setFillColor(sf::Color(200, 200, 200));
            window.draw(controlsText);
            
            window.display();
        }
    }
};

int main() {
    MusicPlayer player;
    if (!player.init()) {
        std::cerr << "Failed to initialize assets!" << std::endl;
        std::cerr << "Make sure you have the following structure:" << std::endl;
        std::cerr << "  assets/fonts/arial.ttf" << std::endl;
        std::cerr << "  assets/images/image.jpg" << std::endl;
        std::cerr << "  assets/music/song1.mp3, song2.mp3, song3.mp3" << std::endl;
        return -1;
    }
    player.run();
    return 0;
}