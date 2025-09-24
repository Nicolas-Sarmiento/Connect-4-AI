#include <connect4/game.hpp>
#include <connect4/agent.hpp>
#include <connect4/board_state.hpp>


#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>



const float CELL_SIZE = 100.f;
const float PIECE_RADIUS = 40.f;
const float BORDER_THICKNESS = 5.f;
const float WINDOW_WIDTH = COLS * CELL_SIZE;
const float WINDOW_HEIGHT = (ROWS + 1) * CELL_SIZE;


enum class GameState { SETUP, PLAYING, GAME_OVER };
enum class GameMode { HUMAN_VS_AI, AI_VS_AI };
enum class AIDifficulty { EASY, MEDIUM, HARD };

std::vector<std::vector<int>> board(ROWS, std::vector<int>(COLS, 0)); //Board represented as a matrix, but agents use board state

BoardState board_state; // It is necessary to use both board and board_state at the same time.

Weights a_weights = { 
    .three_attack_open = 5000 ,
    .three_attack_semi_open = 500 ,
    .two_attack_open = 1000 ,
    .two_attack_semi_open = 100 ,
    .three_defense_open = 6000,
    .three_defense_semi_open = 6000,
    .two_defense_open = 1000,
    .two_defense_semi_open = 500
};
Weights b_weights = { 
    .three_attack_open = 5000 ,
    .three_attack_semi_open = 500 ,
    .two_attack_open = 1000 ,
    .two_attack_semi_open = 100 ,
    .three_defense_open = 6000,
    .three_defense_semi_open = 6000,
    .two_defense_open = 1000,
    .two_defense_semi_open = 500
};

Agent red_agent (Player::A,a_weights );
Agent yellow_agent (Player::B, b_weights);

bool isValidMove(int col) {
    if (col >= 0 && col < COLS) {
        return board[0][col] == 0;
    }
    return false;
}

void makeMove( int player, int col ) {
    short player_id = static_cast<short>(player);
    board_state.make_movement(player_id, col);
    for (int r = ROWS - 1; r >= 0; --r) {
        if (board[r][col] == 0) {
            board[r][col] = player;
            return;
        }
    }

}


int checkWin() {
    if( board_state.is_player_winner(PLAYER_A) ) return PLAYER_A;
    
    if( board_state.is_player_winner(PLAYER_B) ){
        for( int i = 0; i < (COLS * ROWS); i++ ){
            map_heuristics[i] += board_state.board[i] == PLAYER_B ? 1 : 0;
        }
        return PLAYER_B;
    }
    if (board_state.is_tie()) return TIE;

    return 0;
}

int aiMove( AIDifficulty difficulty, int player ) {
    int depth;
    switch (difficulty){
        case AIDifficulty::EASY: 
            depth = 2;
            break;
        case AIDifficulty::MEDIUM:
            depth = 4;
            break;
        case AIDifficulty::HARD:
            depth = 5;
            break;
    }
    int col = 0;
    if (player == PLAYER_B ){
        col =  yellow_agent.find_best_move(board_state,  depth  );
    }else if (player == PLAYER_A) {
        col =  red_agent.find_best_move(board_state,  depth );
    }

    return col;
}



void drawRoundedRect(sf::RenderWindow& window, const sf::Vector2f& size, const sf::Vector2f& position, float radius, const sf::Color& color) {
    sf::RectangleShape rect(size - sf::Vector2f(2 * radius, 2 * radius));
    rect.setPosition(position + sf::Vector2f(radius, radius));
    rect.setFillColor(color);
    window.draw(rect);

    sf::CircleShape circle(radius);
    circle.setFillColor(color);

    circle.setPosition(position);
    window.draw(circle);
    circle.setPosition(position + sf::Vector2f(size.x - 2 * radius, 0));
    window.draw(circle);
    circle.setPosition(position + sf::Vector2f(0, size.y - 2 * radius));
    window.draw(circle);
    circle.setPosition(position + sf::Vector2f(size.x - 2 * radius, size.y - 2 * radius));
    window.draw(circle);

    sf::RectangleShape rect1(sf::Vector2f(size.x - 2 * radius, size.y));
    rect1.setPosition(position + sf::Vector2f(radius, 0));
    rect1.setFillColor(color);
    window.draw(rect1);

    sf::RectangleShape rect2(sf::Vector2f(size.x, size.y - 2 * radius));
    rect2.setPosition(position + sf::Vector2f(0, radius));
    rect2.setFillColor(color);
    window.draw(rect2);
}


void drawText(sf::RenderWindow& window, const std::string& str, const sf::Font& font, int size, const sf::Vector2f& position, const sf::Color& color) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(position);
    window.draw(text);
}



void drawButton(sf::RenderWindow& window, const std::string& text, const sf::Font& font, const sf::FloatRect& bounds, const sf::Color& bgColor, bool isSelected) {
    drawRoundedRect(window, sf::Vector2f(bounds.width, bounds.height), sf::Vector2f(bounds.left, bounds.top), 10.f, bgColor);
    if (isSelected) {
        sf::RectangleShape highlight(sf::Vector2f(bounds.width, bounds.height));
        highlight.setPosition(bounds.left, bounds.top);
        highlight.setFillColor(sf::Color(255, 255, 255, 50));
        window.draw(highlight);
    }
    drawText(window, text, font, 24, {bounds.left + bounds.width / 2, bounds.top + bounds.height / 2}, sf::Color::White);
}


void resetGame() {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            board[r][c] = 0;
        }
    }

    board_state.reset();
}


int GUI() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Connect 4", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    
    if (!font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf") && !font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    GameState currentState = GameState::SETUP;
    GameMode currentMode = GameMode::HUMAN_VS_AI;
    AIDifficulty currentDifficulty = AIDifficulty::MEDIUM;
    int currentPlayer = 1;
    int winner = 0;

    
    sf::FloatRect pveButton(50, 150, 200, 50);
    sf::FloatRect eveButton(450, 150, 200, 50);
    sf::FloatRect easyButton(50, 300, 150, 50);
    sf::FloatRect mediumButton(275, 300, 150, 50);
    sf::FloatRect hardButton(500, 300, 150, 50);
    sf::FloatRect startButton(WINDOW_WIDTH/2 - 100, 450, 200, 60);
    sf::FloatRect replayButton(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 50, 200, 60);

    sf::Clock aiMoveClock;
    const sf::Time aiTurnDelay = sf::seconds(0.5f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    
                    if (currentState == GameState::SETUP) {
                        if (pveButton.contains(mousePos)) currentMode = GameMode::HUMAN_VS_AI;
                        if (eveButton.contains(mousePos)) currentMode = GameMode::AI_VS_AI;
                        if (easyButton.contains(mousePos)) currentDifficulty = AIDifficulty::EASY;
                        if (mediumButton.contains(mousePos)) currentDifficulty = AIDifficulty::MEDIUM;
                        if (hardButton.contains(mousePos)) currentDifficulty = AIDifficulty::HARD;
                        if (startButton.contains(mousePos)) {
                            resetGame();
                            currentPlayer = 1;
                            winner = 0;
                            currentState = GameState::PLAYING;
                        }
                    } else if (currentState == GameState::PLAYING) {
                        if (currentMode == GameMode::HUMAN_VS_AI && currentPlayer == 1) {
                            int col = static_cast<int>(mousePos.x / CELL_SIZE);
                            if (isValidMove(col)) {
                                makeMove(currentPlayer, col);
                                winner = checkWin();
                                if (winner != 0) {
                                    currentState = GameState::GAME_OVER;
                                } else {
                                    currentPlayer = 2; 
                                }
                            }
                        }
                    } else if (currentState == GameState::GAME_OVER) {
                         if (replayButton.contains(mousePos)) {
                            currentState = GameState::SETUP;
                         }
                    }
                }
            }
        }

        
        if (currentState == GameState::PLAYING) {
             if ((currentMode == GameMode::AI_VS_AI) || (currentMode == GameMode::HUMAN_VS_AI && currentPlayer == 2)) {
                
                if (aiMoveClock.getElapsedTime() >= aiTurnDelay) {
                    int col = aiMove(currentDifficulty, currentPlayer);
                    
                    if(isValidMove(col)) {
                        makeMove(currentPlayer, col);
                        winner = checkWin();
                        if(winner != 0) {
                            currentState = GameState::GAME_OVER;
                        } else {
                            currentPlayer = (currentPlayer == 1) ? 2 : 1;
                        }
                    }
                    aiMoveClock.restart(); // Restart the clock for the next turn
                }
                
            }
        }

       
        window.clear(sf::Color(15, 30, 60)); 

        if (currentState == GameState::SETUP) {
            drawText(window, "Connect 4", font, 60, {WINDOW_WIDTH/2, 60}, sf::Color::White);
            
            drawText(window, "Game Mode:", font, 30, {WINDOW_WIDTH/2, 120}, sf::Color(200, 200, 200));
            drawButton(window, "Human vs AI", font, pveButton, sf::Color(40, 80, 150), currentMode == GameMode::HUMAN_VS_AI);
            drawButton(window, "AI vs AI", font, eveButton, sf::Color(40, 80, 150), currentMode == GameMode::AI_VS_AI);

            drawText(window, "AI Difficulty:", font, 30, {WINDOW_WIDTH/2, 270}, sf::Color(200, 200, 200));
            drawButton(window, "Easy", font, easyButton, sf::Color(40, 150, 80), currentDifficulty == AIDifficulty::EASY);
            drawButton(window, "Medium", font, mediumButton, sf::Color(180, 150, 40), currentDifficulty == AIDifficulty::MEDIUM);
            drawButton(window, "Hard", font, hardButton, sf::Color(180, 40, 40), currentDifficulty == AIDifficulty::HARD);

            drawButton(window, "Start Game", font, startButton, sf::Color(50, 120, 50), false);

        } else if (currentState == GameState::PLAYING || currentState == GameState::GAME_OVER) {
            
            drawRoundedRect(window, {WINDOW_WIDTH, ROWS * CELL_SIZE}, {0, CELL_SIZE}, 20.f, sf::Color(25, 118, 210)); // Blue board

            
            for (int r = 0; r < ROWS; ++r) {
                for (int c = 0; c < COLS; ++c) {
                    sf::CircleShape circle(PIECE_RADIUS);
                    circle.setOrigin(PIECE_RADIUS, PIECE_RADIUS);
                    circle.setPosition(c * CELL_SIZE + CELL_SIZE / 2, r * CELL_SIZE + CELL_SIZE + CELL_SIZE / 2);
                    
                    if (board[r][c] == 0) {
                        circle.setFillColor(sf::Color(15, 30, 60));
                    } else if (board[r][c] == 1) {
                        circle.setFillColor(sf::Color::Red);
                    } else {
                        circle.setFillColor(sf::Color::Yellow);
                    }
                    window.draw(circle);
                }
            }

        
            if (currentState == GameState::PLAYING && currentMode == GameMode::HUMAN_VS_AI && currentPlayer == 1) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (mousePos.y > CELL_SIZE) {
                    int col = mousePos.x / CELL_SIZE;
                    if (col >= 0 && col < COLS) {
                        sf::RectangleShape highlight(sf::Vector2f(CELL_SIZE, ROWS * CELL_SIZE));
                        highlight.setPosition(col * CELL_SIZE, CELL_SIZE);
                        highlight.setFillColor(sf::Color(255, 255, 255, 30));
                        window.draw(highlight);
                    }
                }
            }
        }
        
        if (currentState == GameState::GAME_OVER) {
            sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(overlay);

            std::string winText;
            if (winner == 1) winText = "Player 1 (Red) Wins!";
            else if (winner == 2) winText = "Player 2 (Yellow) Wins!";
            else winText = "It's a Draw!";

            drawText(window, winText, font, 50, {WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 50}, sf::Color::White);
            drawButton(window, "Replay", font, replayButton, sf::Color(50, 120, 50), false);
        }

        window.display();
    }
    return 0;
}

