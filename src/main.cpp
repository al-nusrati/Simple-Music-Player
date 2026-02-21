// ============================================================
//  HIVE — main.cpp
// ============================================================

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "DoublyLinkedList.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <optional>

using namespace std;

// ============================================================
//  Helpers
// ============================================================
static string formatDuration(int totalSec) {
    stringstream ss;
    ss << totalSec / 60 << ":" << setfill('0') << setw(2) << totalSec % 60;
    return ss.str();
}

static string formatSfTime(sf::Time t) {
    int s = (int)t.asSeconds();
    stringstream ss;
    ss << s / 60 << ":" << setfill('0') << setw(2) << s % 60;
    return ss.str();
}

// ============================================================
//  Track
// ============================================================
struct Track {
    int    id;
    string title;
    string artist;
    int    duration;   // seconds (0 = unknown)
    string filePath;

    bool operator==(const Track& o) const { return id == o.id; }
};

// ============================================================
//  Playlist
// ============================================================
class Playlist {
private:
    DoublyLinkedList<Track> dll;
    node<Track>*            currentNode;
    int                     nextId;

    Track makeTrack(const string& title, const string& artist,
                    int duration, const string& filePath) {
        return { nextId++, title, artist, duration, filePath };
    }

    void syncCurrent() {
        if (!currentNode) currentNode = dll.getHead();
    }

public:
    Playlist() : currentNode(nullptr), nextId(1) {}

    void addAtBeginning(const string& title, const string& artist,
                        int duration, const string& filePath = "") {
        dll.insertAtBeginning(makeTrack(title, artist, duration, filePath));
        syncCurrent();
    }

    void addAtEnd(const string& title, const string& artist,
                  int duration, const string& filePath = "") {
        dll.insertAtEnd(makeTrack(title, artist, duration, filePath));
        syncCurrent();
    }

    void addAtPosition(int pos, const string& title, const string& artist,
                       int duration, const string& filePath = "") {
        dll.insertAtAnyPos(pos, makeTrack(title, artist, duration, filePath));
        syncCurrent();
    }

    bool removeByID(int id) {
        node<Track>* temp = dll.getHead();
        while (temp) {
            if (temp->data.id == id) {
                if (temp == currentNode) {
                    if      (temp->next) currentNode = temp->next;
                    else if (temp->prev) currentNode = temp->prev;
                    else                 currentNode = nullptr;
                }
                dll.deleteNode(temp);
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    node<Track>* searchByTitle(const string& title) const {
        node<Track>* temp = dll.getHead();
        while (temp) {
            if (temp->data.title == title) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    bool moveNext() {
        if (!currentNode) return false;
        currentNode = currentNode->next ? currentNode->next : dll.getHead();
        return true;
    }

    bool movePrev() {
        if (!currentNode || !currentNode->prev) return false;
        currentNode = currentNode->prev;
        return true;
    }

    int          count()            const { return dll.nodeCount(); }
    Track*       getCurrentTrack()        { return currentNode ? &currentNode->data : nullptr; }
    node<Track>* getCurrentNode()         { return currentNode; }
    node<Track>* getHeadNode()            { return dll.getHead(); }
};

// ============================================================
//  MusicPlayer
// ============================================================
class MusicPlayer {
private:
    sf::RenderWindow      window;
    sf::Music             music;
    sf::Font              font;
    optional<sf::Texture> bgTexture;
    optional<sf::Sprite>  bgSprite;
    Playlist&             playlist;

    enum class Status { Stopped, Playing, Paused } status = Status::Stopped;

    // ---- Audio ----
    void playCurrent() {
        Track* t = playlist.getCurrentTrack();
        if (!t) return;
        music.stop();
        if (music.openFromFile(t->filePath)) {
            music.play();
            status = Status::Playing;
        } else {
            cerr << "Could not open: " << t->filePath << "\n";
            status = Status::Stopped;
        }
    }

    // ---- Util ----
    int currentPosition() {
        int pos = 1;
        node<Track>* temp = playlist.getHeadNode();
        while (temp && temp != playlist.getCurrentNode()) { pos++; temp = temp->next; }
        return pos;
    }

    // ---- Draw ----
    void drawUI() {
        window.clear(sf::Color(20, 20, 25));

        // Background image
        if (bgSprite.has_value()) window.draw(bgSprite.value());

        // Dark overlay over entire window
        sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        window.draw(overlay);

        // ── LEFT PANEL (0–499) ──────────────────────────────
        Track* t = playlist.getCurrentTrack();

        // Status dot
        sf::Color statusColor =
            (status == Status::Playing) ? sf::Color(80, 220, 80)  :
            (status == Status::Paused)  ? sf::Color(220, 180, 50) :
                                          sf::Color(200, 70,  70);
        sf::CircleShape dot(6.f);
        dot.setFillColor(statusColor);
        dot.setPosition({28.f, 34.f});
        window.draw(dot);

        string statusStr = (status == Status::Playing) ? "PLAYING" :
                           (status == Status::Paused)  ? "PAUSED"  : "STOPPED";
        sf::Text statusTxt(font, statusStr, 13);
        statusTxt.setPosition({46.f, 28.f});
        statusTxt.setFillColor(statusColor);
        window.draw(statusTxt);

        // Song title
        sf::Text titleTxt(font, t ? t->title : "No Track", 22);
        titleTxt.setPosition({28.f, 60.f});
        titleTxt.setFillColor(sf::Color::White);
        window.draw(titleTxt);

        // Artist
        sf::Text artistTxt(font, t ? t->artist : "", 15);
        artistTxt.setPosition({28.f, 92.f});
        artistTxt.setFillColor(sf::Color(160, 160, 160));
        window.draw(artistTxt);

        // Track counter
        sf::Text counterTxt(font,
            "Track " + to_string(currentPosition()) + " of " + to_string(playlist.count()), 13);
        counterTxt.setPosition({28.f, 118.f});
        counterTxt.setFillColor(sf::Color(100, 100, 100));
        window.draw(counterTxt);

        // Progress bar bg
        sf::RectangleShape progBg({440.f, 8.f});
        progBg.setPosition({28.f, 175.f});
        progBg.setFillColor(sf::Color(55, 55, 55));
        window.draw(progBg);

        // Progress fill
        float ratio = 0.f;
        if (music.getDuration().asSeconds() > 0)
            ratio = music.getPlayingOffset().asSeconds() / music.getDuration().asSeconds();
        if (ratio > 1.f) ratio = 1.f;
        sf::RectangleShape progFill({440.f * ratio, 8.f});
        progFill.setPosition({28.f, 175.f});
        progFill.setFillColor(sf::Color(80, 200, 80));
        window.draw(progFill);

        // Time
        sf::Text timeTxt(font,
            formatSfTime(music.getPlayingOffset()) + "  /  " + formatSfTime(music.getDuration()), 13);
        timeTxt.setPosition({28.f, 192.f});
        timeTxt.setFillColor(sf::Color(170, 170, 170));
        window.draw(timeTxt);

        // Controls hint
        sf::Text ctrlTxt(font, "SPACE: Play/Pause   N: Next   P: Previous   ESC: Quit", 12);
        ctrlTxt.setPosition({28.f, 408.f});
        ctrlTxt.setFillColor(sf::Color(80, 80, 80));
        window.draw(ctrlTxt);

        // ── RIGHT PANEL (500–799) ───────────────────────────
        sf::RectangleShape rightBg({300.f, 430.f});
        rightBg.setPosition({500.f, 0.f});
        rightBg.setFillColor(sf::Color(10, 10, 15, 220));
        window.draw(rightBg);

        sf::RectangleShape divider({1.f, 430.f});
        divider.setPosition({500.f, 0.f});
        divider.setFillColor(sf::Color(45, 45, 45));
        window.draw(divider);

        sf::Text plHeader(font, "PLAYLIST", 14);
        plHeader.setPosition({516.f, 14.f});
        plHeader.setFillColor(sf::Color(80, 200, 80));
        window.draw(plHeader);

        sf::RectangleShape plLine({264.f, 1.f});
        plLine.setPosition({508.f, 38.f});
        plLine.setFillColor(sf::Color(40, 40, 40));
        window.draw(plLine);

        node<Track>* cur = playlist.getHeadNode();
        float yPos = 46.f;
        int   idx  = 1;
        while (cur && yPos < 395.f) {
            bool isCur = (cur == playlist.getCurrentNode());

            if (isCur) {
                sf::RectangleShape hl({276.f, 26.f});
                hl.setPosition({504.f, yPos - 2.f});
                hl.setFillColor(sf::Color(25, 65, 25));
                window.draw(hl);
            }

            sf::Text tTitle(font, to_string(idx) + ".  " + cur->data.title, 13);
            tTitle.setPosition({512.f, yPos});
            tTitle.setFillColor(isCur ? sf::Color(100, 240, 100) : sf::Color(210, 210, 210));
            window.draw(tTitle);

            sf::Text tArtist(font, "     " + cur->data.artist, 11);
            tArtist.setPosition({512.f, yPos + 15.f});
            tArtist.setFillColor(isCur ? sf::Color(60, 160, 60) : sf::Color(110, 110, 110));
            window.draw(tArtist);

            yPos += 36.f;
            idx++;
            cur = cur->next;
        }

        // Track count footer
        sf::Text footTxt(font, to_string(playlist.count()) + " tracks", 11);
        footTxt.setPosition({512.f, 408.f});
        footTxt.setFillColor(sf::Color(60, 60, 60));
        window.draw(footTxt);

        window.display();
    }

public:
    explicit MusicPlayer(Playlist& pl)
        : window(sf::VideoMode({800, 430}), "HIVE"), playlist(pl) {
        window.setFramerateLimit(60);
    }

    bool init() {
        // Font
        if (!font.openFromFile("../assets/fonts/arial.ttf")) {
            cerr << "[ERROR] Font not found: ../assets/fonts/arial.ttf\n";
            return false;
        }

        // Background (optional — continues without it)
        sf::Texture tex;
        if (tex.loadFromFile("../assets/images/image.jpg")) {
            bgTexture = move(tex);
            bgSprite.emplace(bgTexture.value());
            auto ws = window.getSize();
            auto ts = bgTexture->getSize();
            bgSprite->setScale({ (float)ws.x / ts.x, (float)ws.y / ts.y });
        } else {
            cout << "[INFO] Background image not found, using flat color.\n";
        }

        return true;
    }

    void run() {
        if (playlist.count() > 0) playCurrent();

        while (window.isOpen()) {
            while (const optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();

                if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    switch (key->code) {
                    case sf::Keyboard::Key::Space:
                        if (status == Status::Playing) { music.pause(); status = Status::Paused; }
                        else                           { music.play();  status = Status::Playing; }
                        break;
                    case sf::Keyboard::Key::N:
                        playlist.moveNext(); playCurrent(); break;
                    case sf::Keyboard::Key::P:
                        playlist.movePrev(); playCurrent(); break;
                    case sf::Keyboard::Key::Escape:
                        window.close(); break;
                    default: break;
                    }
                }
            }

            // Auto-advance when track ends
            if (status == Status::Playing &&
                music.getStatus() == sf::Music::Status::Stopped) {
                playlist.moveNext();
                playCurrent();
            }

            drawUI();
        }
    }
};

// ============================================================
//  main
// ============================================================
int main() {
    Playlist playlist;

    // Add your songs here
    playlist.addAtEnd("Balaghal Ula Bi Kamaalihi", "Ali Zafar",   0, "../assets/music/Balaghal Ula Bi Kamaalihi.mp3");
    playlist.addAtEnd("Faslon Ko Takkaluf",         "Ai-Fi Covers", 0, "../assets/music/Faslon Ko Takkaluf.mp3");
    playlist.addAtEnd("MUSTAFA JAAN E REHMAT",      "Atif Aslam",  0, "../assets/music/MUSTAFA JAAN E REHMAT.mp3");

    MusicPlayer player(playlist);
    if (!player.init()) return -1;

    player.run();
    return 0;
}