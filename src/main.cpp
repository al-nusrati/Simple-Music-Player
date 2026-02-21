#include <iostream>
#include <string>
#include <limits>
#include <SFML/Audio.hpp>
#include "ConsoleUtils.h"
#include "DoublyLinkedList.h" 

using namespace std;

// ==========================================
// 1. DATA ENTITY
// ==========================================
struct Track {
    int id;
    string title;
    string artist;
    int duration;
    string filePath;

    bool operator==(const Track& other) const { return id == other.id; }
    
    friend ostream& operator<<(ostream& os, const Track& t) {
        os << "[" << t.id << "] " << t.title << " by " << t.artist;
        return os;
    }
};

// ==========================================
// 2. DOMAIN LOGIC (Playlist Class)
// ==========================================
class Playlist {
private:
    DoublyLinkedList<Track> dll;
    node<Track>* currentTrackNode;
    int nextId;

public:
    Playlist() : currentTrackNode(nullptr), nextId(1) {}

    void addTrack(const string& title, const string& artist, int duration, const string& path) {
        Track newTrack = { nextId++, title, artist, duration, path };
        dll.insertAtEnd(newTrack);
        
        // If it's the first track, point current to it
        if (dll.nodeCount() == 1) {
            currentTrackNode = dll.getHead();
        }
    }

    void removeTrack(int id) {
        // Find position of ID (O(N) search)
        node<Track>* temp = dll.getHead();
        int pos = 1;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                // Safety: If deleting the playing track, move pointer to next
                if (temp == currentTrackNode) {
                    moveNext(); 
                    if (temp == currentTrackNode) currentTrackNode = nullptr; // If it was the only track
                }
                dll.deleteAtAnyPos(pos);
                return;
            }
            temp = temp->next;
            pos++;
        }
    }

    void moveNext() {
        if (currentTrackNode && currentTrackNode->next) {
            currentTrackNode = currentTrackNode->next;
        } else {
            currentTrackNode = dll.getHead(); // Loop back to start
        }
    }

    void movePrev() {
        if (currentTrackNode && currentTrackNode->prev) {
            currentTrackNode = currentTrackNode->prev;
        }
    }

    Track* getCurrentTrack() {
        if (currentTrackNode) return &(currentTrackNode->data);
        return nullptr;
    }

    void displayPlaylist() {
        dll.traverseForward();
    }

    int getTotalTracks() {
        return dll.nodeCount(); // This must be O(1)
    }
};

// ==========================================
// 3. PRESENTATION LAYER (MusicPlayer Class)
// ==========================================
class MusicPlayer {
private:
    Playlist& playlist;
    sf::Music music;
    ConsoleUtils utils;
    bool isPlaying;

    void playAudio() {
        Track* current = playlist.getCurrentTrack();
        if (!current) return;

        music.stop();
        if (music.openFromFile(current->filePath)) {
            music.play();
            isPlaying = true;
        } else {
            isPlaying = false;
        }
    }

    void drawDashboard() {
        utils.clearConsole();

        // Header
        utils.setForegroundColor(ConsoleColor::BrightCyan);
        cout << "+------------------------------------------------+\n";
        cout << "|                      HIVE                      |\n";
        cout << "+------------------------------------------------+\n";

        // 2. Your Signature 
        utils.setForegroundColor(ConsoleColor::BrightYellow);
        // 39 spaces ensures 'D' aligns perfectly with the box edge
        cout << "                                  BY: JAWAD AHMED\n\n";

        // Now Playing
        Track* current = playlist.getCurrentTrack();
        if (current) {
            utils.setForegroundColor(ConsoleColor::BrightGreen);
            cout << ">>> NOW PLAYING <<<\n";
            utils.setForegroundColor(ConsoleColor::White);
            cout << "Title  : " << current->title << "\n";
            cout << "Artist : " << current->artist << "\n";
            
            if (isPlaying) {
                utils.setForegroundColor(ConsoleColor::BrightYellow);
                cout << "Status : [ PLAYING ]\n\n";
            } else {
                utils.setForegroundColor(ConsoleColor::BrightRed);
                cout << "Status : [ PAUSED ]\n\n";
            }
        } else {
            utils.setForegroundColor(ConsoleColor::BrightRed);
            cout << ">>> PLAYLIST EMPTY <<<\n\n";
        }

        // Playlist Overview
        utils.setForegroundColor(ConsoleColor::BrightMagenta);
        cout << "--- PLAYLIST (" << playlist.getTotalTracks() << " Tracks) ---\n";
        utils.setForegroundColor(ConsoleColor::White);
        playlist.displayPlaylist();
        cout << "\n";

        // Controls
        utils.setForegroundColor(ConsoleColor::BrightCyan);
        cout << "+------------------------------------------------+\n";
        utils.setForegroundColor(ConsoleColor::White);
        cout << "[1] Play/Pause    [2] Next Track    [3] Prev Track\n";
        cout << "[4] Add Song      [5] Remove Song   [6] Exit\n";
        cout << "+------------------------------------------------+\n";
        utils.setForegroundColor(ConsoleColor::BrightGreen);
        cout << "Enter choice: ";
        utils.setDefaultColor();
    }

    void handleInput(int choice) {
        switch (choice) {
            case 1: // Play/Pause
                if (isPlaying) {
                    // If currently playing, simply pause
                    music.pause();
                    isPlaying = false;
                } else {
                    // CHECK: Is the song actually loaded?
                    // If Duration is 0, it means no file is loaded yet (Fresh Start).
                    if (music.getDuration() == sf::Time::Zero) {
                        playAudio(); // Load the file and start from scratch
                    } else {
                        // File is loaded, just resume from where we left off
                        music.play();
                        isPlaying = true;
                    }
                }
                break;
            case 2: // Next
                playlist.moveNext();
                playAudio();
                break;
            case 3: // Prev
                playlist.movePrev();
                playAudio();
                break;
            case 4: { // Add
                string title, artist, path;
                cout << "Enter Title: "; cin.ignore(); getline(cin, title);
                cout << "Enter Artist: "; getline(cin, artist);
                cout << "Enter Filepath (.mp3): "; getline(cin, path);
                playlist.addTrack(title, artist, 0, path);
                break;
            }
            case 5: { // Remove
                int id;
                cout << "Enter Track ID to delete: ";
                cin >> id;
                playlist.removeTrack(id);
                // Resync audio in case we deleted the currently playing track
                if (playlist.getCurrentTrack() == nullptr) music.stop();
                break;
            }
            default:
                break;
        }
    }

public:
    MusicPlayer(Playlist& p) : playlist(p), isPlaying(false) {
        utils.enableVirtualTerminal();
    }

    void run() {
        bool running = true;
        int choice;

        while (running) {
            drawDashboard();
            
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue; 
            }

            if (choice == 6) {
                running = false;
                continue;
            }

            handleInput(choice);
        }

        utils.clearConsole();
        utils.setForegroundColor(ConsoleColor::BrightCyan);
        utils.showCredits();
    }
};

// ==========================================
// 4. MAIN EXECUTION
// ==========================================
int main() {
    // 1. Instantiate the Domain Layer
    Playlist myPlaylist;

    // Load initial assets
    myPlaylist.addTrack("Faslon Ko Takkaluf", "Atif Aslam", 101, "assets/music/Faslon Ko Takkaluf.mp3");
    myPlaylist.addTrack("Balaghal Ula Bi Kamaalihi", "Ali Zafar", 102, "assets/music/Balaghal Ula Bi Kamaalihi.mp3");
    myPlaylist.addTrack("MUSTAFA JAAN E REHMAT", "Atif Aslam", 103, "assets/music/MUSTAFA JAAN E REHMAT.mp3");
    //myPlaylist.addTrack("Song 4", "Artist D", 195, "assets/music/song4.mp3");
    //myPlaylist.addTrack("Song 5", "Artist E", 220, "assets/music/song5.mp3");

    // 2. Instantiate the Presentation Layer and inject the Playlist
    MusicPlayer player(myPlaylist);

    // 3. Start the application
    player.run();

    system("pause>0");
    return 0;
}