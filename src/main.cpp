#include <iostream>
#include <string>
#include <SFML/Audio.hpp>
#include "ConsoleUtils.h"
#include "DoublyLinkedList.h" 

using namespace std;
struct Track {
    int id;
    string title;
    string artist;
    int duration;
    string filePath;
    
    friend ostream& operator<<(ostream& os, const Track& t) {
        os << "[" << t.id << "] " << t.title << " by " << t.artist;
        return os;
    }
};

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
        if (dll.nodeCount() == 1)    currentTrackNode = dll.getHead();
        
    }

    void removeTrack(int id) {
        node<Track>* temp = dll.getHead();
        int pos = 1;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                if (temp == currentTrackNode) {
                    moveNext(); 
                
                    if (temp == currentTrackNode) currentTrackNode = nullptr;
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
        } 
        else {
            currentTrackNode = dll.getHead(); 
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
        return dll.nodeCount(); 
    }
};

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

        utils.setForegroundColor(ConsoleColor::BrightCyan);
        cout << "+------------------------------------------------+\n";
        cout << "|                      HIVE                      |\n";
        cout << "+------------------------------------------------+\n";

        utils.setForegroundColor(ConsoleColor::BrightYellow);
        cout << "                                  BY: JAWAD AHMED\n\n";

        Track* current = playlist.getCurrentTrack();
        if (current) {
            utils.setForegroundColor(ConsoleColor::BrightGreen);
            cout << ">>> NOW PLAYING <<<\n";
            utils.setForegroundColor(ConsoleColor::White);
            cout << "Title  : " << current->title << "\n";
            cout << "Artist : " << current->artist << "\n";
            
            if (isPlaying) {
                utils.setForegroundColor(ConsoleColor::BrightYellow);
                cout << "Status : [[ PLAYING ]]\n\n";
            } 
            else {
                utils.setForegroundColor(ConsoleColor::BrightRed);
                cout << "Status : [[ PAUSED ]]\n\n";
            }
        } 
        
        else {
            utils.setForegroundColor(ConsoleColor::BrightRed);
            cout << ">>> PLAYLIST EMPTY <<<\n\n";
        }

        utils.setForegroundColor(ConsoleColor::BrightMagenta);
        cout << "--- PLAYLIST (" << playlist.getTotalTracks() << " Tracks) ---\n";
        utils.setForegroundColor(ConsoleColor::White);
        playlist.displayPlaylist();
        cout << "\n";

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
            case 1: 
                if (isPlaying) {
                    music.pause();
                    isPlaying = false;
                } 
                else {
                    if (music.getDuration() == sf::Time::Zero) {
                        playAudio();
                    } 
                    else {
                        music.play();
                        isPlaying = true;
                    }
                }
                break;
            case 2:
                playlist.moveNext();
                playAudio();
                break;
            case 3: 
                playlist.movePrev();
                playAudio();
                break;
            case 4: {
                string title, artist, path;
                cout << "Enter Title: "; cin.ignore(); getline(cin, title);
                cout << "Enter Artist: "; getline(cin, artist);
                cout << "Enter Filepath (.mp3): "; getline(cin, path);
                playlist.addTrack(title, artist, 0, path);
                break;
            }
            case 5: {
                int id;
                cout << "Enter Track ID to delete: ";
                cin >> id;
                playlist.removeTrack(id);
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

int main() {
    Playlist myPlaylist;

    myPlaylist.addTrack("Faslon Ko Takkaluf", "Atif Aslam", 101, "assets/music/Faslon Ko Takkaluf.mp3");
    myPlaylist.addTrack("Balaghal Ula Bi Kamaalihi", "Ali Zafar", 102, "assets/music/Balaghal Ula Bi Kamaalihi.mp3");
    myPlaylist.addTrack("MUSTAFA JAAN E REHMAT", "Atif Aslam", 103, "assets/music/MUSTAFA JAAN E REHMAT.mp3");

    MusicPlayer player(myPlaylist);
    player.run();

    system("pause>0");
    return 0;
}