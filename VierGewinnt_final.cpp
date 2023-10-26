#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <sstream>

//

#define RED_TEXT "\033[0;31m"
#define YELLOW_TEXT "\033[0;33m"
#define BLUE_TEXT "\033[0;34m"
#define RESET_TEXT "\033[0m"

//

std::vector<std::vector<int>> deepcopy(const std::vector<std::vector<int>>& board) {
    std::vector<std::vector<int>> board_copy;
    for (const std::vector<int>& row : board){
        std::vector<int> new_y;
        for (const int& x : row){
            new_y.push_back(x);
        }
        board_copy.push_back(new_y);
    }
    return board_copy;
}

bool is_int(int value) {
    std::string input = std::to_string(value);
    std::istringstream iss(input);
    int extracted_value;
    return (iss >> extracted_value) && (iss.eof());
}

void printboard(const std::vector<std::vector<int>>& board) {
    std::cout<<"  1   2   3   4   5   6   7"<<std::endl;
    std::cout<<BLUE_TEXT<<"+---+---+---+---+---+---+---+"<<RESET_TEXT<<std::endl;
    for (int i = 0; i <6; ++i) {
        std::cout<<BLUE_TEXT<< "| "<<RESET_TEXT;
        for (int j = 0; j < 7; ++j) {
            if (board[i][j] == 1) {std::cout<<YELLOW_TEXT<<"O"<<RESET_TEXT;}
            else if (board[i][j] == -1) {std::cout<<RED_TEXT<<"O"<<RESET_TEXT;}
            else {std::cout<<" ";}
            std::cout <<BLUE_TEXT<<" | "<<RESET_TEXT;
        }
        std::cout<<std::endl;
        std::cout<<BLUE_TEXT<<"+---+---+---+---+---+---+---+"<<RESET_TEXT<<std::endl;
    }
}

int generate_random_int(int min, int max) {
    int random_number=min+rand()%(max-min+1);
    return random_number;
}

bool gewonnen(const std::vector<std::vector<int>>& board, int player) {
    bool gewonnen = false;
    // horizontal
    for (int q = 0; q < 4; ++q) {
        for (int w = 0; w < 6; ++w) {
            if (board[w][q] == player && board[w][q + 1] == player && board[w][q + 2] == player && board[w][q + 3] == player) {gewonnen = true;}
        }
    }
    // vertikal
    for (int q = 0; q < 7; ++q) {
        for (int w = 0; w < 3; ++w) {
            if (board[w][q] == player && board[w + 1][q] == player && board[w + 2][q] == player && board[w + 3][q] == player) {gewonnen = true;}
        }
    }
    // diagonal1
    for (int q = 0; q < 4; ++q) {
        for (int w = 0; w < 3; ++w) {
            if (board[w][q] == player && board[w + 1][q + 1] == player && board[w + 2][q + 2] == player && board[w + 3][q + 3] == player) {gewonnen = true;}
        }
    }
    // diagonal2
    for (int q = 0; q < 4; ++q) {
        for (int w = 0; w < 3; ++w) {
            if (board[w][q + 3] == player && board[w + 1][q + 2] == player && board[w + 2][q + 1] == player && board[w + 3][q] == player) {gewonnen = true;}
        }
    }
    return gewonnen;
}

bool game_over(const std::vector<std::vector<int>>& board) {
    bool over = true;
    for (int q = 0; q < 6; ++q) {
        if (board[0][q] == 0) {over = false; break;}
    }
    return over;
}

int evaluate_position(const std::vector<std::vector<int>>& board, int player) {
    int score=0;
    int otherplayer;
    if (player==1) {otherplayer=-1;}
    else {otherplayer=1;}
    //horizontal
    for (int q=0; q<4; ++q) {
        for (int w=0; w<6; ++w) {
            int empty=0;
            int other=0;
            int filled=0;
            //
            if (board[w][q]==player) {filled+=1;}
            else if (board[w][q]==0) {empty+=1;}
            else if (board[w][q]==otherplayer) {other+=1;}
            //
            if (board[w][q + 1]==player) {filled+=1;}
            else if (board[w][q+1]==0) {empty+=1;}
            else if (board[w][q+1]==otherplayer) {other+=1;}
            //
            if (board[w][q+2]==player) {filled+=1;}
            else if (board[w][q+2]==0) {empty+=1;}
            else if (board[w][q+2]==otherplayer) {other+=1;}
            //
            if (board[w][q+3]==player) {filled+=1;}
            else if (board[w][q+3]==0) {empty+=1;}
            else if (board[w][q+3]==otherplayer) {other+=1;}
            //
            if (other==0) {
                if (filled==4) {score+=10000;}
                if (filled==3) {score+=1000;}
                if (filled==2) {score+=3;}
            } else if (filled==0) {
                if (other==4) {score-=10000;}
                if (other==3) {score-=100;}
                if (other==2) {score-=3;}
            } else {}
        }
    }
    //vertikal
    for (int e=0; e<7; ++e) {
        for (int r=0; r<3; ++r) {
            int empty=0;
            int other=0;
            int filled=0;
            //
            if (board[r][e]==player) {filled+=1;}
            else if (board[r][e]==0) {empty+=1;}
            else if (board[r][e]==otherplayer) {other+=1;}
            //
            if (board[r+1][e]==player) {filled+=1;}
            else if (board[r+1][e]==0) {empty+=1;}
            else if (board[r+1][e]==otherplayer) {other+=1;}
            //
            if (board[r+2][e]==player) {filled+=1;}
            else if (board[r+2][e]==0) {empty+=1;}
            else if (board[r+2][e]==otherplayer) {other+=1;}
            //
            if (board[r+3][e]==player) {filled+=1;}
            else if (board[r+3][e]==0) {empty+=1;}
            else if (board[r+3][e]==otherplayer) {other+=1;}
            //
            if (other==0) {
                if (filled==4) {score+=10000;}
                if (filled==3) {score+=10;}
                if (filled==2) {score+=1;}
            } else if (filled==0) {
                if (other==4) {score-=10000;}
                if (other==3) {score-=30;}
                if (other==2) {score-=1;}
            } else {}
        }
    }
    //diagonal1
    for (int a=0; a<4; ++a) {
        for (int s=0; s<3; ++s) {
            int empty=0;
            int other=0;
            int filled=0;
            //
            if (board[s][a]==player) {filled+=1;}
            else if (board[s][a]==0) {empty+=1;}
            else if (board[s][a]==otherplayer) {other+=1;}
            //
            if (board[s+1][a+1]==player) {filled+=1;}
            else if (board[s+1][a+1]==0) {empty+=1;}
            else if (board[s+1][a+1]==otherplayer) {other+=1;}
            //
            if (board[s+2][a+2]==player) {filled+=1;}
            else if (board[s+2][a+2]==0) {empty+=1;}
            else if (board[s+2][a+2]==otherplayer) {other+=1;}
            //
            if (board[s+3][a+3]==player) {filled+=1;}
            else if (board[s+3][a+3]==0) {empty+=1;}
            else if (board[s+3][a+3]==otherplayer) {other+=1;}
            //
            if (other==0) {
                if (filled==4) {score+=10000;}
                if (filled==3) {score+=1000;}
                if (filled==2) {score+=3;}
            } else if (filled==0) {
                if (other==4) {score-=10000;}
                if (other==3) {score-=100;}
                if (other==2) {score-=3;}
            } else {}
        }
    }
    //diagonal2
    for (int y=0; y<4; ++y) {
        for (int x=0; x<3; ++x) {
            int empty=0;
            int other=0;
            int filled=0;
            //
            if (board[x][y+3]==player) {filled+=1;}
            else if (board[x][y+3]==0) {empty+=1;}
            else if (board[x][y+3]==otherplayer) {other+=1;}
            //
            if (board[x+1][y+2]==player) {filled+=1;}
            else if (board[x+1][y+2]==0) {empty+=1;}
            else if (board[x+1][y+2]==otherplayer) {other+=1;}
            //
            if (board[x+2][y+1]==player) {filled+=1;}
            else if (board[x+2][y+1]==0) {empty+=1;}
            else if (board[x+2][y+1]==otherplayer) {other+=1;}
            //
            if (board[x+3][y]==player) {filled+=1;}
            else if (board[x+3][y]==0) {empty+=1;}
            else if (board[x+3][y]==otherplayer) {other+=1;}
            //
            if (other==0) {
                if (filled==4) {score+=10000;}
                if (filled==3) {score+=1000;}
                if (filled==2) {score+=3;}
            } else if (filled==0) {
                if (other==4) {score-=10000;}
                if (other==3) {score-=100;}
                if (other==2) {score-=3;}
            } else {}
        }
    }
    //
    //std::cout<<score<<std::endl;
    return score;
}

void fall(std::vector<std::vector<int>>& board, int y, int x, const int player) {
    while (y<5 && board[y+1][x]==0) {
        board[y+1][x] = player;
        board[y][x] = 0;
        y+=1;
    }
}

std::list<std::vector<std::vector<int>>> generate_children(const std::vector<std::vector<int>> board, const int player) {
    std::list<std::vector<std::vector<int>>> children;
    for (int x = 0; x < 7; ++x) {
        std::vector<std::vector<int>> board_copy = board;
        if (board_copy[0][x] == 0) {
            board_copy[0][x] = player;
            fall(board_copy, 0, x, player);
            children.push_back(board_copy);
        }
    }
    return children;
}

std::vector<std::vector<int>> generate_one_random_child(const std::vector<std::vector<int>> board, const int player) {
    std::vector<std::vector<int>> board_copy=board;
    int x=generate_random_int(0, 6);
    if (game_over(board)) {std::vector<std::vector<int>> empty_vector; return empty_vector;}
    while (true) {
        if (board_copy[0][x]==0) {break;}
        x=generate_random_int(0,6);
    }
    board_copy[0][x]=player;
    fall(board_copy, 0, x, player);
    return board_copy;
}

//

class HumanPlayer {
public:
    HumanPlayer(int token, std::vector<std::vector<int>> board) : token(token) {}
    //braucht board nicht, ist aber besser bei Austauche von Playertypen

    std::vector<std::vector<int>> player(std::vector<std::vector<int>>& board) {
        try {
            int x;
            std::cout << "x: ";
            //
            if (!(std::cin >> x) || !is_int(x) || x < 1 || x > 8) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "EINGABE NICHT KORREKT2" << std::endl;
                return player(board);
            }

            x -= 1;

            if (board[0][x] == 0) {
                board[0][x] = this->token;
                fall(board, 0, x, this->token);
                return board;
            }
            else {
                std::cout << "EINGABE NICHT KORREKT2" << std::endl;
                return player(board);
            }
        } catch (...) {
            std::cout << "EINGABE NICHT KORREKT1" << std::endl;
            return player(board);
        }
    }

    std::vector<std::vector<int>> get_move(std::vector<std::vector<int>>& board) {
        return player(board);
    }

private:
    int token;
};

//

int minimax_counter=0;

class MinimaxNode {
public:
    MinimaxNode() : value(0), value_not_none(false), children(), board(), player_am_zug(0), token(0), depth(0), expanded(false) {}

    int value;
    bool value_not_none;
    std::vector<MinimaxNode> children;
    std::vector<std::vector<int>> board;
    int player_am_zug;
    int token;
    int depth;
    bool expanded;

    std::vector<MinimaxNode> expand_node() {
        std::vector<MinimaxNode> new_children;
        std::list<std::vector<std::vector<int>>> list_of_positions = generate_children(this->board, this->player_am_zug);
        for (const std::vector<std::vector<int>>& board_position : list_of_positions) {
            MinimaxNode child;
            child.board = board_position;
            child.player_am_zug = -this->player_am_zug;
            child.token = this->token;
            child.depth = this->depth + 1;
            child.value_not_none = false;
            child.value=0;
            child.children;
            new_children.push_back(child);

        }
        return new_children;
    }

    int minimax(int alpha, int beta, bool max_player, const int max_depth) {
        //
        minimax_counter+=1;
        //
        if (this->depth==max_depth || gewonnen(this->board,1) || gewonnen(this->board,-1) || game_over(this->board)) {
            this->value = evaluate_position(this->board, this->token);
            this->value_not_none=true;
            return this->value;
        }
        //
        if (!this->expanded) {this->children=this->expand_node(); this->expanded = true;}
        //
        if (max_player) {
            int max_value=-1000000;
            for (MinimaxNode& child : this->children) {
                int eval=child.minimax(alpha,beta,false,max_depth);
                if (eval>max_value) {max_value=eval;}
                //pruning
                if (eval>alpha) {alpha=eval;}
                if (beta<=alpha) {break;}
            }
            this->value=max_value;
            return max_value;
        }
        else if (!max_player) {
            int min_value=1000000;
            for (MinimaxNode& child : this->children) {
                int eval=child.minimax(alpha,beta,true,max_depth);
                if (eval<min_value) {min_value=eval;}
                //pruning
                if (eval<beta) {beta=eval;}
                if (beta<=alpha) {break;}
            }
            this->value=min_value;
            return min_value;
        }

    }

    void sort(bool max_player) {
        if (this->expanded) {
            //
            std::vector<MinimaxNode> not_none_children;
            std::list<MinimaxNode> none_children;
            std::vector<MinimaxNode> sorted_children;
            //
            for (const MinimaxNode& child : this->children) {
                if (!child.value_not_none) {none_children.push_back(child);}
                else {not_none_children.push_back(child);}
            }
            //
            if (max_player) {
                std::sort(not_none_children.begin(), not_none_children.end(),[](const MinimaxNode& a, const MinimaxNode& b) {return a.value > b.value;});
                //
                sorted_children.insert(sorted_children.end(), not_none_children.begin(), not_none_children.end());
                sorted_children.insert(sorted_children.end(), none_children.begin(), none_children.end());
                this->children=sorted_children;
                //
                for (MinimaxNode& child : not_none_children) {child.sort(false);}
            }
            else {
                std::sort(not_none_children.begin(), not_none_children.end(),[](const MinimaxNode& a, const MinimaxNode& b) {return a.value < b.value;});
                //
                sorted_children.insert(sorted_children.end(), not_none_children.begin(), not_none_children.end());
                sorted_children.insert(sorted_children.end(), none_children.begin(), none_children.end());
                this->children=sorted_children;
                //
                for (MinimaxNode& child : not_none_children) {child.sort(true);}
            }
        }
    }

};

class MinimaxPlayer {
public:
    MinimaxPlayer(int token, std::vector<std::vector<int>> board) : token(token), board(board) {
        root_node.board = board;
        root_node.player_am_zug = token;
        root_node.token=token;
        root_node.value_not_none = false;
        root_node.value = 0;
        root_node.depth = 0;
        root_node.children = root_node.expand_node();
        root_node.expanded=true;
        //
        minimax_counter=0;
    }

    MinimaxNode root_node;
    int token;
    std::vector<std::vector<int>> board;
    int max_time=7;
    int max_depth=10;
    int starting_depth=1;

    std::vector<std::vector<int>> minimaxer(const int depth, const std::chrono::duration<double> vergangene_zeit) {
        auto start = std::chrono::high_resolution_clock::now();
        //
        std::vector<int> values;
        std::vector<MinimaxNode> best_moves;
        MinimaxNode best_move;
        int best_value = -1000000;
        std::vector<MinimaxNode>& root_node_children=root_node.children;
        std::vector<std::vector<int>> return_board;
        //
        for (MinimaxNode& child : root_node_children){
            int eval;
            eval=child.minimax(-1000000,1000000,false, depth);
            child.value=eval;
            std::cout<<"a ";//child wurde fertig berechnet
            //
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> vergangene_zeit2 =(now+vergangene_zeit)-start;
            if (vergangene_zeit2.count() >= max_time) {std::cout<<" NICHT FERTIG"; break;}
        }
        //
        for (MinimaxNode& child : root_node_children) {values.push_back(child.value);}
        for (int v : values) {if (v > best_value) {best_value = v;}}
        for (MinimaxNode& child : root_node_children) {if (child.value==best_value) {best_moves.push_back(child);}}
        //
        //output---------
        std::cout << ""<< std::endl;
        std::cout << best_value << std::endl;
        std::cout<<"COUNTER: "; std::cout<<minimax_counter<<std::endl;
        //---------------
        best_move=best_moves[generate_random_int(0, best_moves.size()-1)];
        return_board=deepcopy(best_move.board);
        return return_board;
    }

    std::vector<std::vector<int>> minimaxerer(const std::vector<std::vector<int>> board_0) {
        auto start = std::chrono::high_resolution_clock::now();
        //
        int depth=this->starting_depth;
        std::vector<std::vector<int>> move;
        while (true) {
            //break1
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> vergangene_zeit = now - start;
            if (vergangene_zeit.count() >= max_time) {break;}
            else if (depth>max_depth) {break;}
            //calculate move
            std::cout<<"---DEPTH: ";
            std::cout<<depth<<std::endl;
            //
            std::vector<std::vector<int>> new_move=minimaxer(depth,vergangene_zeit);
            //sort+ depth
            //PRINT CHILD.VALUE FUER ALLE ROOTNODE CHILDREN
            for (MinimaxNode& child : root_node.children) {std::cout<<child.value; std::cout<<", ";}
            std::cout << std::endl;
            //MOVE SORTING:-------------------
            //this->root_node.sort(true);
            //--------------------------------
            //break2, damit nicht move=new_move wenn depth nicht fertig
            now = std::chrono::high_resolution_clock::now();
            vergangene_zeit = now - start;
            if (vergangene_zeit.count() >= max_time) {break;}
            move=new_move;
            //PRINT CHILD.VALUE FUER ALLE ROOTNODE CHILDREN NACH SORTIEREN
            for (MinimaxNode& child : root_node.children) {std::cout<<child.value; std::cout<<", ";}
            //
            std::cout<<std::endl;
            if (depth>max_depth) {break;}
            depth+=1;
        }
        return move;
    }

    std::vector<std::vector<int>> get_move(std::vector<std::vector<int>> board) {
        return minimaxerer(board);
    }
};

//

double c=10000;
int number_of_simulations=100;
int depth=4;

int mcts_counter=0;

class MCTSNode {
public:
    MCTSNode() : value(0), children(),parent(nullptr), visits(0), board(), player_am_zug(0), token(0), expanded(false) {}

    int value;
    std::vector<MCTSNode> children;
    MCTSNode* parent;
    int visits;
    std::vector<std::vector<int>> board;
    int player_am_zug;
    int token;
    bool expanded;

    double calculate_ucb() {
        MCTSNode* par = this->parent;
        if (this->visits == 0) {
            return std::numeric_limits<double>::infinity();
        }
        else {
            double ucb = (static_cast<double>(this->value) / this->visits) +c * (std::sqrt(std::log(static_cast<double>(par->visits)) / this->visits));
            return ucb;
        }
    }

    std::vector<MCTSNode> expand_node() {
        std::vector<MCTSNode> new_children;
        std::list<std::vector<std::vector<int>>> list_of_positions = generate_children(this->board, this->player_am_zug);
        for (const std::vector<std::vector<int>>& board_position : list_of_positions) {
            MCTSNode child;
            child.value=0;
            child.children;
            child.parent=this;
            child.visits=0;
            child.board=board_position;
            child.player_am_zug=-this->player_am_zug;
            child.token=this->token;
            child.expanded=false;
            //
            new_children.push_back(child);

        }
        return new_children;//vielleicht kein return sondern this->children=new_children?
    }

    MCTSNode* select_leaf_node() {
        double best_value = -std::numeric_limits<double>::infinity();
        MCTSNode* selected_node = nullptr;

        for (MCTSNode& child : this->children) {
            double ucb_of_child = child.calculate_ucb();
            if (ucb_of_child > best_value) {
                best_value = ucb_of_child;
                selected_node = &child;
            }
        }

        if (!selected_node->expanded) {return selected_node;}
        else {return selected_node->select_leaf_node();}
    }

    void backpropagate(double new_value, int number_of_simulations) {
        this->value += new_value;
        this->visits += number_of_simulations;
        //
        if (this->parent != nullptr) {
            parent->backpropagate(new_value, number_of_simulations);
        }
    }

    double simulate() {
        double value = 0.0;
        std::vector<double> values;
        //
        for (int j = 0; j < number_of_simulations; ++j) {
            std::vector<std::vector<int>> pos = this->board;
            int player = this->player_am_zug;
            for (int i = 0; i < depth; ++i) {
                std::vector<std::vector<int>> next_pos = generate_one_random_child(pos, player);
                if (next_pos.empty()) {break;}
                pos = next_pos;
                if (player== -6) {player= 6;}
                else if (player== 6) {player= -6;}
            }
            values.push_back(evaluate_position(pos, this->token));
        }
        for (double val : values) {value += val;}
        value /= values.size();
        //
        return value;
    }

};

class MCTSPlayer {
public:
    MCTSPlayer(int token, std::vector<std::vector<int>> board) : token(token), board(board) {
        root_node.value=0;
        root_node.children;
        root_node.parent=nullptr;
        root_node.visits=0;
        root_node.board=board;
        root_node.player_am_zug=token;
        root_node.token=token;
        root_node.expanded=false;
    }

    MCTSNode root_node;
    int token;
    std::vector<std::vector<int>> board;
    int max_time=3;

    bool mcts() {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        root_node.children=root_node.expand_node();
        //
        while (true) {
            mcts_counter += 1;
            MCTSNode* selected_node=root_node.select_leaf_node();
            selected_node->children=selected_node->expand_node();
            for (MCTSNode& child_node : selected_node->children) {
                double new_score = child_node.simulate();
                child_node.backpropagate(new_score, number_of_simulations);
            }
            //
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
            //
            if (elapsed_seconds > this->max_time) {break;}
        }
        return true;
    }

    std::vector<std::vector<int>> mctser() {
        mcts_counter = 0;
        //
        bool monte_carlo=mcts();
        //
        if (monte_carlo) {
            std::cout <<"COUNTER: ";
            std::cout << mcts_counter << std::endl;
            //
            std::vector<std::vector<int>> best_move;
            int highest_number_of_visits = -1;
            //
            for (MCTSNode& root_node_child : root_node.children) {
                if (root_node_child.visits > highest_number_of_visits) {
                    best_move = root_node_child.board;
                    highest_number_of_visits = root_node_child.visits;
                }
            }
            return best_move;
        }
        else {std::vector<std::vector<int>> empty_vector; return empty_vector;}
        
    }

    std::vector<std::vector<int>> get_move(std::vector<std::vector<int>> board) {
        return mctser();
    }

};

//

class VierGewinnt {
public:
    VierGewinnt() : board(), turn(1) {
            board={
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0}
        };
        }

    int play() {
        int current = 1;

        while (true) {
            //-----------------------------------------
            HumanPlayer player_1(1, this->board);
            MinimaxPlayer player_2(-1, this->board);
            //-----------------------------------------
            std::cout<<""<<std::endl;;
            std::cout<<this->turn<<std::endl;
            printboard(this->board);

            if (current==1) {
                std::cout <<"Gelb ist am Zug"<<std::endl;
                std::vector<std::vector<int>> board_copy = deepcopy(this->board);
                std::vector<std::vector<int>> new_board = player_1.get_move(board_copy);
                this->board=new_board;
            }
            else if (current==2) {
                std::cout<<"Rot ist am Zug"<<std::endl;
                std::vector<std::vector<int>> board_copy = deepcopy(this->board);
                std::vector<std::vector<int>> new_board = player_2.get_move(board_copy);
                this->board=new_board;
            }
            //
            if (game_over(board)) {printboard(this->board); std::cout<<"UNENTSCHIEDEN"<<std::endl; return 0;}
            else if (gewonnen(this->board, 1)) {printboard(this->board); std::cout<<"GELB HAT GEWONNEN"<<std::endl; return 1;}
            else if (gewonnen(this->board, -1)) {printboard(this->board); std::cout<<"ROT HAT GEWONNEN"<<std::endl; return -1;}
            //
            if (current==1) {current = 2;}
            else {current = 1;}
            this->turn += 1;
        }
    }

private:
    std::vector<std::vector<int>> board;
    int turn;
};

//

void spielen(int z) {
    int x_wins=0;
    int o_wins=0;
    int unentschieden=0;
    //
    for (int i=0; i<z; ++i) {
        std::cout<<"NEUES SPIEL"<<std::endl;
        VierGewinnt game;
        int r = game.play();
        if (r==1) {x_wins+=1;}
        else if (r== -1) {o_wins+=1;}
        else if (r==0) {unentschieden+= 1;}
        std::cout<<"GELB: "<<x_wins<<std::endl;
        std::cout<<"ROT: "<<o_wins<<std::endl;
        std::cout<<"-: "<<unentschieden<<std::endl;
    }
    std::cout<<"FERTIG"<<std::endl;
}

//

int main() {
    srand(time(0)); //seed
    spielen(30);
}


