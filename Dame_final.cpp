#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <sstream>
#include <string>

//

#define BROWN_TEXT "\033[38;2;139;69;19m"
#define BEIGE_TEXT "\033[38;2;227;212;173m"
#define RESET_TEXT "\033[0m"

//

bool is_int(int value) {
    std::string input = std::to_string(value);
    std::istringstream iss(input);
    int extracted_value;
    return (iss >> extracted_value) && (iss.eof());
}

int generate_random_int(int min, int max) {
    int random_number=min+rand()%(max-min+1);
    return random_number;
}

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

void print_board(const std::vector<std::vector<int>>& board) {
    std::vector zahlen= {8,7,6,5,4,3,2,1};
    std::cout<<"  a   b   c   d   e   f   g   h"<<std::endl;
    std::cout<<"+---+---+---+---+---+---+---+---+"<<std::endl;
    for (int i=0; i<8; ++i) {
        std::cout << "| ";
        for (int j = 0; j < 8; ++j) {
            if (board[i][j]==-1) {std::cout<<BROWN_TEXT<<"O"<<RESET_TEXT;}
            else if (board[i][j]==1) {std::cout<<BEIGE_TEXT<<"O"<<RESET_TEXT;}
            else if (board[i][j]==-2) {std::cout<<BROWN_TEXT<<"Q"<<RESET_TEXT;}
            else if (board[i][j]==2) {std::cout<<BEIGE_TEXT<<"Q"<<RESET_TEXT;}
            else {std::cout << " ";}
            std::cout << " | ";
        }
        std::cout<<zahlen[i] <<std::endl;
        std::cout<<"+---+---+---+---+---+---+---+---+"<<std::endl;
    }
}

//

std::list<std::vector<std::vector<int>>> children_schlagen_XO;
std::list<std::vector<std::vector<int>>> children_schlagen_WM;

void generate_children_schlagen_XO(int y, int x, const std::vector<std::vector<int>> board, int player, bool new_b) {
    if (new_b) {children_schlagen_XO.clear();}
    //
    std::vector<std::vector<int>> board_copy = board;
    //
    if (player==1) {
        if (y - 2 > -1 && x - 2 > -1 && board_copy[y - 2][x - 2]==0) {
            if (board_copy[y-1][x-1] < 0) {
                board_copy[y- 1][x-1] = 0;
                board_copy[y-2][x-2] = 1;
                board_copy[y][x] = 0;
                if (y - 2 ==0) {
                    board_copy[y-2][x-2] = 2;
                }
                children_schlagen_XO.push_back(board_copy);
                generate_children_schlagen_XO(y - 2, x - 2, board_copy, player, false);
                board_copy = board;
            }
        }
        if (y - 2 > -1 && x + 2 < 8 && board_copy[y-2][x+2]==0) {
            if (board_copy[y-1][x+1] < 0) {
                board_copy[y-1][x+1] = 0;
                board_copy[y-2][x+2] = 1;
                board_copy[y][x] = 0;
                if (y - 2 ==0) {
                    board_copy[y-2][x + 2] = 2;
                }
                children_schlagen_XO.push_back(board_copy);
                generate_children_schlagen_XO(y - 2, x + 2, board_copy, player, false);
                board_copy = board;
            }
        }
    }
    else if (player==-1) {
        if (y + 2 < 8 && x - 2 > -1 && board_copy[y+2][x-2]==0) {
            if (board_copy[y+1][x-1] > 0) {
                board_copy[y+1][x-1] = 0;
                board_copy[y+2][x-2] = -1;
                board_copy[y][x] = 0;
                if (y+2 ==7) {
                    board_copy[y+2][x - 2] = -2;
                }
                children_schlagen_XO.push_back(board_copy);
                generate_children_schlagen_XO(y + 2, x - 2, board_copy, player, false);
                board_copy = board;
            }
        }
        if (y + 2 < 8 && x + 2 < 8 && board_copy[y+2][x+2]==0) {
            if (board_copy[y+1][x+1] > 0) {
                board_copy[y+1][x+1] = 0;
                board_copy[y+2][x+2] = -1;
                board_copy[y][x] = 0;
                if (y + 2 ==7) {
                    board_copy[y + 2][x + 2] = -2;
                }
                children_schlagen_XO.push_back(board_copy);
                generate_children_schlagen_XO(y + 2, x + 2, board_copy, player, false);
                board_copy = board;
            }
        }
    }
    std::reverse(children_schlagen_XO.begin(), children_schlagen_XO.end());
}

void generate_children_schlagen_WM(int y, int x, std::vector<std::vector<int>> board, int player, bool new_b) {
    if (new_b) {children_schlagen_WM.clear();}
    std::vector<std::pair<int, int>> directions = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    std::vector<std::vector<int>> board_copy = board;
    //
    if (player==-2) {
        for (const auto& direction : directions) {
            int dy = direction.first;
            int dx = direction.second;
            for (int o=1; o<8; ++o) {
                if (y+(1+o)*dy>7 || x+(1+o)*dx>7 || y+(1+o)*dy<0 || x+(1+o)*dx<0) {break;}
                if (board_copy[y+o*dy][x+o*dx]<0) {break;}
                if (board_copy[y+o*dy][x+o*dx]>0) {
                    if (board_copy[y+(1+o)*dy][x+(1+o)*dx]==0) {
                        board_copy[y+(1+o)*dy][x+(1+o)*dx]=-2;
                        board_copy[y+o*dy][x+o*dx]=0;
                        board_copy[y][x]=0;
                        children_schlagen_WM.push_back(board_copy);
                        generate_children_schlagen_WM(y+(1+o)*dy, x+(1+o)*dx, board_copy, -2, false);
                        board_copy=board;
                        break;
                    }
                    else {break;}
                }
            }
        }
    }
    //
    else if (player==2) {
        for (const auto& direction : directions) {
            int dy = direction.first;
            int dx = direction.second;
            for (int o=1; o<8; ++o) {
                if (y+(1+o)*dy>7 || x+(1+o)*dx>7 || y+(1+o)*dy<0 || x+(1+o)*dx<0) {break;}
                if (board_copy[y+o*dy][x+o*dx]>0) {break;}
                if (board_copy[y+o*dy][x+o*dx]<0) {
                    if (board_copy[y+(1+o)*dy][x+(1+o)*dx]==0) {
                        board_copy[y+(1+o)*dy][x+(1+o)*dx]=2;
                        board_copy[y+o*dy][x+o*dx]=0;
                        board_copy[y][x]=0;
                        children_schlagen_WM.push_back(board_copy);
                        generate_children_schlagen_WM(y+(1+o)*dy, x+(1+o)*dx, board_copy, 2, false);
                        board_copy=board;
                        break;
                    }
                    else {break;}
                }
            }
        }
    }
    //
    children_schlagen_WM.reverse();
}

std::list<std::vector<std::vector<int>>> generate_children_WM(int y, int x, std::vector<std::vector<int>> board, int player) {
    std::vector<std::pair<int, int>> directions = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    std::list<std::vector<std::vector<int>>> childrenWM1;
    std::list<std::vector<std::vector<int>>> childrenWM2;
    std::vector<std::vector<int>> board_copy = board;
    bool schlagen = false;
    
    //
    if (player==-2) {
        for (const auto& direction : directions) {
            int dy = direction.first;
            int dx = direction.second;

            for (int o=1; o<8; ++o) {
                schlagen = false;
                if (y+o*dy>7 || x+o*dx>7 || y+o*dy<0 || x+o*dx<0) {break;}
                if (board_copy[y+o*dy][x+o*dx] < 0) {break;}
                if (board_copy[y+o*dy][x+o*dx] > 0) {
                    if (!(y+(o+1)*dy>7) && !(x+(o+1)*dx>7) && !(y+(o+1)*dy<0) && !(x+(o+1)*dx<0)) {
                        schlagen = true;}
                }
                if (board_copy[y+o*dy][x+o*dx] == 0) {
                    board_copy[y+o*dy][x+o*dx] = -2;
                    board_copy[y][x] = 0;
                    childrenWM2.push_back(board_copy);
                    board_copy = board;
                }
                if (schlagen) {
                    if (board_copy[y+(o+1)*dy][x+(o+1)*dx] == 0) {
                        board_copy[y+(o+1)*dy][x+(o+1)*dx] = -2;
                        board_copy[y][x] = 0;
                        board_copy[y+o*dy][x+o*dx] = 0;
                        childrenWM1.push_back(board_copy);
                        children_schlagen_WM.clear();
                        generate_children_schlagen_WM(y+(o+1)*dy, x+(o+1)*dx, board_copy, -2, true);
                        std::list<std::vector<std::vector<int>>> children_s = children_schlagen_WM;
                        childrenWM1.insert(childrenWM1.end(), children_s.begin(), children_s.end());
                        board_copy = board;
                        schlagen = false;
                        break;
                    }
                    else {break;}
                }
            }
        }
    }
    else if (player==2) {
        for (const auto& direction : directions) {
            int dy = direction.first;
            int dx = direction.second;
            for (int o=1; o<8; ++o) {
                schlagen = false;
                if (y+o*dy>7 || x+o*dx>7 || y+o*dy<0 || x+o*dx<0) {break;}
                if (board_copy[y+o*dy][x+o*dx] > 0) {break;}
                if (board_copy[y+o*dy][x+o*dx] < 0) {
                    if (!(y+(o+1)*dy>7) && !(x+(o+1)*dx>7) && !(y+(o+1)*dy<0) && !(x+(o+1)*dx<0)) {
                        schlagen = true;}
                }
                if (board_copy[y+o*dy][x+o*dx] == 0) {
                    board_copy[y+o*dy][x+o*dx] = 2;
                    board_copy[y][x] = 0;
                    childrenWM2.push_back(board_copy);
                    board_copy = board;
                }
                if (schlagen) {
                    if (board_copy[y+(o+1)*dy][x+(o+1)*dx] == 0) {
                        board_copy[y+(o+1)*dy][x+(o+1)*dx] = 2;
                        board_copy[y][x] = 0;
                        board_copy[y+o*dy][x+o*dx] = 0;
                        childrenWM1.push_back(board_copy);
                        children_schlagen_WM.clear();
                        generate_children_schlagen_WM(y+(o+1)*dy, x+(o+1)*dx, board_copy, -2, true);
                        std::list<std::vector<std::vector<int>>> children_s = children_schlagen_WM;
                        childrenWM1.insert(childrenWM1.end(), children_s.begin(), children_s.end());
                        board_copy = board;
                        schlagen = false;
                        break;
                    }
                    else {break;}
                }
            }
        }
    }
    childrenWM1.insert(childrenWM1.end(), childrenWM2.begin(), childrenWM2.end());
    return childrenWM1;
}

std::list<std::vector<std::vector<int>>> generate_children(const std::vector<std::vector<int>> board, int player) {
    std::list<std::vector<std::vector<int>>> children1;
    std::list<std::vector<std::vector<int>>> children2;
    std::vector<std::vector<int>> board_copy = board;
    
    for (int y=0; y<8; ++y) {
        for (int x=0; x <8; ++x) {
            if (player==1) {
                if (board_copy[y][x] == 1) {
                    if (y - 1 > -1 && x - 1 > -1 && board_copy[y - 1][x - 1] == 0) {
                        board_copy[y - 1][x - 1] = 1;
                        board_copy[y][x] = 0;
                        if (y - 1 == 0) {
                            board_copy[y - 1][x - 1] = 2;
                            children1.push_back(board_copy);
                        }
                        children2.push_back(board_copy);
                        board_copy = board;
                    }
                    if (y - 1 > -1 && x + 1 < 8 && board_copy[y - 1][x + 1] == 0) {
                        board_copy[y - 1][x + 1] = 1;
                        board_copy[y][x] = 0;
                        if (y - 1 == 0) {
                            board_copy[y - 1][x + 1] = 2;
                            children1.push_back(board_copy);
                        }
                        children2.push_back(board_copy);
                        board_copy = board;
                    }
                    //
                    children_schlagen_XO.clear();
                    generate_children_schlagen_XO(y, x, board_copy, 1, true);
                    std::list<std::vector<std::vector<int>>> children_s = children_schlagen_XO;
                    children1.insert(children1.end(), children_s.begin(), children_s.end());
                    //
                }
                else if (board_copy[y][x] == 2) {
                    //
                    std::list<std::vector<std::vector<int>>> children_wm = generate_children_WM(y, x, board_copy, 2);
                    children1.insert(children1.end(), children_wm.begin(), children_wm.end());
                    //
                }
            }
            else if (player==-1) {
                if (board_copy[y][x] == -1) {
                    if (y + 1 < 8 && x - 1 > -1 && board_copy[y + 1][x - 1] == 0) {
                        board_copy[y + 1][x - 1] = -1;
                        board_copy[y][x] = 0;
                        if (y + 1 == 7) {
                            board_copy[y + 1][x - 1] = -2;
                            children1.push_back(board_copy);
                        }
                        children2.push_back(board_copy);
                        board_copy = board;
                    }
                    if (y + 1 < 8 && x + 1 < 8 && board_copy[y + 1][x + 1] == 0) {
                        board_copy[y + 1][x + 1] = -1;
                        board_copy[y][x] = 0;
                        if (y + 1 == 7) {
                            board_copy[y + 1][x + 1] = -2;
                            children1.push_back(board_copy);
                        }
                        children2.push_back(board_copy);
                        board_copy = board;
                    }
                    //
                    children_schlagen_XO.clear();
                    generate_children_schlagen_XO(y, x, board_copy, -1, true);
                    std::list<std::vector<std::vector<int>>> children_s = children_schlagen_XO;
                    children1.insert(children1.end(), children_s.begin(), children_s.end());
                    //
                }
                else if (board_copy[y][x] == -2) {
                    //
                    std::list<std::vector<std::vector<int>>> children_wm = generate_children_WM(y, x, board_copy, -2);
                    children1.insert(children1.end(), children_wm.begin(), children_wm.end());
                    //
                }
            }
        }
    }
    std::list<std::vector<std::vector<int>>> children_list;
    children_list.insert(children_list.end(), children1.begin(), children1.end());
    children_list.insert(children_list.end(), children2.begin(), children2.end());
    return children_list;
}

//

int schlagen_XO_c=3;
int schlagen_WM_c=5;
int WM_c=5;

std::vector<int> gorc_XO_schlagen_children;
std::vector<std::vector<std::vector<int>>> gorc_XO_schlagen_children_delete;
std::vector<int> gorc_WM_schlagen_children;
std::vector<std::vector<std::vector<int>>> gorc_WM_schlagen_children_delete;


void gorc_XO_schlagen(int y, int x, std::vector<std::vector<int>>& boardc, int player, std::vector<std::vector<int>>& delete_list) {
    bool geschlagen = false;
    if (player == 1) {
        if (y-2>-1 && x-2>-1 && boardc[y-2][x-2] == 0 && boardc[y-1][x-1]<0) {
            geschlagen = true;
            std::vector<std::vector<int>> delete_list_1 = delete_list;
            delete_list_1.push_back({y-1, x-1});
            gorc_XO_schlagen(y-2, x-2, boardc, player, delete_list_1);
        }
        if (y-2>-1 && x+2<8 && boardc[y-2][x+2]==0 && boardc[y-1][x+1]<0) {
            geschlagen = true;
            std::vector<std::vector<int>> delete_list_2 = delete_list;
            delete_list_2.push_back({y-1, x+1});
            gorc_XO_schlagen(y-2, x+2, boardc, player, delete_list_2);
        }
        if (!geschlagen) {
            gorc_XO_schlagen_children.push_back((y+1)*10 + (x+1));
            gorc_XO_schlagen_children_delete.push_back(delete_list);
        }
    }
    else if (player == -1) {
        if (y+2<8 && x-2>-1 && boardc[y+2][x-2]==0 && boardc[y+1][x-1]>0) {
            geschlagen = true;
            std::vector<std::vector<int>> delete_list_1 = delete_list;
            delete_list_1.push_back({y+1, x-1});
            gorc_XO_schlagen(y+2, x-2, boardc, player, delete_list_1);
        }
        if (y+2<8 && x+2<8 && boardc[y+2][x+2]==0 && boardc[y+1][x+1]>0) {
            geschlagen = true;
            std::vector<std::vector<int>> delete_list_2 = delete_list;
            delete_list_2.push_back({y+1, x+1});
            gorc_XO_schlagen(y+2, x+2, boardc, player, delete_list_2);
        }
        if (!geschlagen) {
            gorc_XO_schlagen_children.push_back((y+1)*10 + (x+1));
            gorc_XO_schlagen_children_delete.push_back(delete_list);
        }
    }
}

std::vector<std::vector<int>> gorc_XO(int y, int x, std::vector<std::vector<int>>& boardc, int player) {
    std::vector<int> childrenXO;
    gorc_XO_schlagen_children.clear();
    gorc_XO_schlagen_children_delete.clear();
    //
    if (player==1) {
        if (y-1>-1 && x-1>-1 && boardc[y-1][x-1]==0) {childrenXO.push_back(1);}
        if (y-1>-1 && x+1<8 && boardc[y-1][x+1]==0) {childrenXO.push_back(2);}
        if (y-2>-1 && x-2>-1 && boardc[y-2][x-2]==0) {
            if (boardc[y-1][x-1]<0) {std::vector<std::vector<int>> dl; gorc_XO_schlagen(y, x, boardc, player, dl);}
        }
        if (y-2>-1 && x+2<8 && boardc[y-2][x+2]==0) {
            if (boardc[y-1][x+1]<0) {std::vector<std::vector<int>> dl; gorc_XO_schlagen(y, x, boardc, player, dl);}
        }
    }
    else if (player==-1) {
        if (y+1<8 && x-1>-1 && boardc[y+1][x-1]==0) {childrenXO.push_back(1);}
        if (y+1<8 && x+1<8 && boardc[y+1][x+1]==0) {childrenXO.push_back(2);}
        if (y+2<8 && x-2>-1 && boardc[y+2][x-2]==0) {
            if (boardc[y+1][x-1]>0) {std::vector<std::vector<int>> dl; gorc_XO_schlagen(y, x, boardc, player, dl);}
        }
        if (y+2<8 && x+2<8 && boardc[y+2][x+2]==0) {
            if (boardc[y+1][x+1]>0) {std::vector<std::vector<int>> dl; gorc_XO_schlagen(y, x, boardc, player, dl);}
        }
    }
    //
    for (int i=0; i<schlagen_XO_c; ++i) {
        childrenXO.insert(childrenXO.end(), gorc_XO_schlagen_children.begin(), gorc_XO_schlagen_children.end());
    }
    //
    if (childrenXO.empty()) {std::vector<std::vector<int>> empty_vector; return empty_vector;}
    else {
        //
        int r=generate_random_int(0, childrenXO.size()-1);
        int n=childrenXO[r];
        //
        if (player==1) {
            if (n==1) {
                boardc[y][x]= 0;
                if (y-1==0) {boardc[y-1][x-1]= 2;}
                else {boardc[y-1][x-1]= 1;}
                return boardc;
            }
            else if (n==2) {
                boardc[y][x]= 0;
                if (y-1==0) {boardc[y-1][x+1]= 2;}
                else {boardc[y-1][x+1] = 1;}
                return boardc;
            }
            else {// schlagen
                int n_y= (n/10)-1;
                int n_x= (n%10)-1;
                boardc[y][x]= 0;
                if (n_y==0) {boardc[n_y][n_x]= 2;}
                else {boardc[n_y][n_x]= 1;}
                std::vector<std::vector<int>> delete_list = gorc_XO_schlagen_children_delete[std::distance(gorc_XO_schlagen_children.begin(), std::find(gorc_XO_schlagen_children.begin(), gorc_XO_schlagen_children.end(), n))];
                for (const auto& feld : delete_list) {boardc[feld[0]][feld[1]]= 0;}
                return boardc;
            }
        }
        else if (player==-1) {
            if (n==1) {
                boardc[y][x]= 0;
                if (y+1==7) {boardc[y-1][x-1]= -2;}
                else {boardc[y+1][x-1]= -1;}
                return boardc;
            }
            else if (n==2) {
                boardc[y][x]= 0;
                if (y+1==7) {boardc[y-1][x+1]= -2;}
                else {boardc[y+1][x+1] = -1;}
                return boardc;
            }
            else {// schlagen
                int n_y= (n/10)-1;
                int n_x= (n%10)-1;
                boardc[y][x]= 0;
                if (n_y==7) {boardc[n_y][n_x] = -2;}
                else {boardc[n_y][n_x]= -1;}
                std::vector<std::vector<int>> delete_list = gorc_XO_schlagen_children_delete[std::distance(gorc_XO_schlagen_children.begin(), std::find(gorc_XO_schlagen_children.begin(), gorc_XO_schlagen_children.end(), n))];
                for (const auto& feld : delete_list) {boardc[feld[0]][feld[1]]= 0;}
                return boardc;
            }
        }
    }
}

void gorc_WM_schlagen(int y, int x, std::vector<std::vector<int>>& boardc, int player, std::vector<std::vector<int>>& delete_list) {
    bool geschlagen = false;
    std::vector<std::pair<int, int>> directions = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    //
    if (player == 2) {
        for (const auto& direction : directions) {
            int dy = direction.first;
            int dx = direction.second;
            for (int i= 1; i<8; ++i) {
                if (y+i*dy>7 || x+i*dx>7 || y+i*dy<0 || x+i*dx<0) {break;}
                if (boardc[y+i*dy][x+i*dx]>0) {break;}
                if (boardc[y+i*dy][x+i*dx]<0) {
                    bool r = false;
                    for (const auto& piece : delete_list) {
                        if (piece[0]==y+i*dy && piece[1]==x+i*dx) {r=true; break;}
                    }
                    if (r) {continue;}
                    if (!(y+(i+1)*dy>7) && !(x+(i+1)*dx>7) && !(y+(i+1)*dy<0) && !(x+(i+1)*dx<0) && boardc[y+(i+1)*dy][x+(i+1)*dx]==0) {
                        geschlagen = true;
                        //
                        std::vector<std::vector<int>> delete_list_1;
                        delete_list_1.insert(delete_list_1.end(), delete_list.begin(), delete_list.end());
                        delete_list_1.push_back({y+i*dy, x+i*dx});
                        //
                        gorc_WM_schlagen(y+(i+1)*dy, x+(i+1)*dx, boardc, player, delete_list_1);
                        break;
                    }
                    else {break;}
                }
            }
        }
    }
    //
    else if (player==-2) {
        for (const auto& direction : directions) {
            int dy = direction.first;
            int dx = direction.second;
            for (int i=1; i<8; ++i) {
                if (y+i*dy>7 || x+i*dx>7 || y+i*dy<0 || x+i*dx<0) {break;}
                if (boardc[y+i*dy][x+i*dx]<0) {break;}
                if (boardc[y+i*dy][x+i*dx]>0) {
                    bool r = false;
                    for (const auto& piece : delete_list) {
                        if (piece[0]==y+i*dy && piece[1]==x+i*dx) {r=true; break;}
                    }
                    if (r) {continue;}
                    if (!(y+(i+1)*dy>7) && !(x+(i+1)*dx>7) && !(y+(i+1)*dy<0) && !(x+(i+1)*dx<0) && boardc[y+(i+1)*dy][x+(i+1)*dx]==0) {
                        geschlagen = true;
                        //
                        std::vector<std::vector<int>> delete_list_1;
                        delete_list_1.insert(delete_list_1.end(), delete_list.begin(), delete_list.end());
                        delete_list_1.push_back({y+i*dy, x+i*dx});
                        //
                        gorc_WM_schlagen(y+(i+1)*dy, x+(i+1)*dx, boardc, player, delete_list_1);
                        break;
                    }
                    else {break;}
                }
            }
        }
    }
    //
    if (!geschlagen) {
        gorc_WM_schlagen_children.push_back(((y+1)*10) + ((x+1) +100));
        gorc_WM_schlagen_children_delete.push_back(delete_list);
    }
}

std::vector<std::vector<int>> gorc_WM(int y, int x, std::vector<std::vector<int>>& boardc, int player) {
    std::vector<int> children_WM;
    std::vector<std::vector<int>> directions = {{1, 1, 10}, {1, -1, 20}, {-1, 1, 30}, {-1, -1, 40}};
    gorc_WM_schlagen_children.clear();
    gorc_WM_schlagen_children_delete.clear();

    if (player == 2) {
        for (const auto& direction : directions) {
            int dy = direction[0];
            int dx = direction[1];
            int d = direction[2];
            for (int i=1; i<8; ++i) {
                if (y+i*dy>7 || x+i*dx>7 || y+i*dy<0 || x+i*dx<0) {break;}
                if (boardc[y+i*dy][x+i*dx]>0) {break;}
                if (boardc[y+i*dy][x+i*dx]==0) {children_WM.push_back(d + i);}
                if (boardc[y+i*dy][x+i*dx]<0) {
                    if (!(y+(i+1)*dy>7) && !(x+(i+1)*dx>7) && !(y+(i+1)*dy<0) && !(x+(i+1)*dx<0)) {
                        if (boardc[y+(i+1)*dy][x+(i+1)*dx]==0) {
                            std::vector<std::vector<int>> del_vector;
                            gorc_WM_schlagen(y, x, boardc, player, del_vector);
                            break;
                        }
                        else {break;}
                    }
                    else {break;}
                }
            }
        }
    }
    else if (player == -2) {
        for (const auto& direction : directions) {
            int dy = direction[0];
            int dx = direction[1];
            int d = direction[2];
            for (int i=1; i<8; ++i) {
                if (y+i*dy>7 || x+i*dx>7 || y+i*dy<0 || x+i*dx<0) {break;}
                if (boardc[y+i*dy][x+i*dx]<0) {break;}
                if (boardc[y+i*dy][x+i*dx]==0) {children_WM.push_back(d + i);}
                if (boardc[y+i*dy][x+i*dx]>0) {
                    if (!(y+(i+1)*dy>7) && !(x+(i+1)*dx>7) && !(y+(i+1)*dy<0) && !(x+(i+1)*dx<0)) {
                        if (boardc[y+(i+1)*dy][x+(i+1)*dx]==0) {
                            std::vector<std::vector<int>> del_vector;
                            gorc_WM_schlagen(y, x, boardc, player, del_vector);
                            break;
                        }
                        else {break;}
                    }
                    else {break;}
                }
            }
        }
    }
    //
    for (int i=0; i<schlagen_WM_c; ++i) {
    children_WM.insert(children_WM.end(), gorc_WM_schlagen_children.begin(), gorc_WM_schlagen_children.end());
    }
    //
    if (children_WM.empty()) {std::vector<std::vector<int>> empty_vector; return empty_vector;}
    else {
        //
        int r=generate_random_int(0, children_WM.size()-1);
        int n=children_WM[r];
        //
        if (n>10 && n<20) { //ur
            boardc[y][x]=0;
            boardc[y+(n-10)][x+(n-10)]=player;
            return boardc;
        }
        else if (n>20 && n<30) { //ul
            boardc[y][x]=0;
            boardc[y+(n-20)][x-(n-20)]=player;
            return boardc;
        }
        else if (n>30 && n<40) { //or
            boardc[y][x]=0;
            boardc[y-(n-30)][x+(n-30)]=player;
            return boardc;
        }
        else if (n>40 && n<50) { //ol
            boardc[y][x]=0;
            boardc[y-(n-40)][x-(n-40)]=player;
            return boardc;
        }
        else if (n>100) { //schlagen
            int n_y=((n-100)/10)-1;
            int n_x=((n-100)%10)-1;
            boardc[y][x]=0;
            boardc[n_y][n_x]=player;
            //
            std::vector<std::vector<int>> delete_list = gorc_WM_schlagen_children_delete[std::find(gorc_WM_schlagen_children.begin(), gorc_WM_schlagen_children.end(), n) - gorc_WM_schlagen_children.begin()];
            for (const auto& feld : delete_list) {boardc[feld[0]][feld[1]]= 0;}
            return boardc;
        }
    }
}

std::vector<std::vector<int>> generate_one_random_child(std::vector<std::vector<int>> boardcopy, int player) {
    //
    std::vector<int> pieces_y;
    std::vector<int> pieces_x;
    if (player==1) {
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (boardcopy[y][x]==1) {pieces_y.push_back(y); pieces_x.push_back(x);}
                else if (boardcopy[y][x]==2) {for (int i=0; i<WM_c; ++i) {pieces_y.push_back(y); pieces_x.push_back(x);}}
            }
        }
    } else if (player==-1) {
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (boardcopy[y][x]==-1) {pieces_y.push_back(y); pieces_x.push_back(x);}
                else if (boardcopy[y][x]==-2) {for (int i=0; i<WM_c; ++i) {pieces_y.push_back(y); pieces_x.push_back(x);}}
            }
        }
    }
    //
    if (pieces_x.empty()) {return {{0}};}
    //
    std::vector<std::vector<int>> child;
    for (int i=0; i<30; ++i) {
        int n=generate_random_int(0, pieces_y.size()-1);
        int y=pieces_y[n];
        int x=pieces_x[n];
        //
        if (player==1) {
            if (boardcopy[y][x]==1) {child=gorc_XO(y, x, boardcopy, 1);}
            else if (boardcopy[y][x]==2) {child=gorc_WM(y, x, boardcopy, 2);}
        }
        else if (player==-1) {
            if (boardcopy[y][x]==-1) {child=gorc_XO(y, x, boardcopy, -1);}
            else if (boardcopy[y][x]==-2) {child=gorc_WM(y, x, boardcopy, -2);}//problem hier
        }
        //
        if (!child.empty()) {break;}
    }
    //
    if (!child.empty()) {return child;}
    else {std::vector<std::vector<int>> empty_vector; return empty_vector;}
}

//

bool verloren1(const std::vector<std::vector<int>>& pos, int player) {
    int player2 =(player==1) ? 2 : -2;
    int eval=true;
    //
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (pos[y][x]==player || pos[y][x]==player2) {eval=false; break;}
        }
    }
    return eval;
}

bool keine_zugmoeglichkeiten(const std::vector<std::vector<int>>& pos, int player) {
    std::list<std::vector<std::vector<int>>> children = generate_children(pos, player);
    return children.empty() ? true : false;
}

bool verloren2(const std::vector<std::vector<int>>& pos, int player) {
    int player2 =(player==1) ? 2 : -2;
    int eval=0;
    //
    for (size_t sl=0; sl < pos.size(); ++sl) {
        eval+=std::count(pos[sl].begin(), pos[sl].end(), player);
        eval+=std::count(pos[sl].begin(), pos[sl].end(), player2);
    }
    if (eval==0 || keine_zugmoeglichkeiten(pos, player)) {return true;}
    else {return false;}
}

//

std::vector<int> X_matrix = {2, 0, 0, 1, 2, 3, 4, 5};
std::vector<int> O_matrix = {5, 4, 3, 2, 1, 0, 0, 2};

int evaluate_position(const std::vector<std::vector<int>>& pos, int player) {
    int eval = 0;
    if (player == 1) {
        int anz_X = 0;
        int anz_O = 0;
        int anz_W = 0;
        int anz_M = 0;
        for (size_t l = 0; l < pos.size(); ++l) {
            for (int o = 0; o < pos[l].size(); ++o) {
                if (pos[l][o] == 1) {
                    eval += 11;
                    anz_X += 1;
                    eval += X_matrix[l];
                }
                else if (pos[l][o] == -1) {
                    eval += -9;
                    anz_O += 1;
                    eval += -O_matrix[l];
                }
                else if (pos[l][o] == 2) {
                    eval += 49;
                    anz_W += 1;
                }
                else if (pos[l][o] == -2) {
                    eval += -51;
                    anz_M += 1;
                }
            }
        }
        //
        if (anz_X == 0 && anz_W == 0) {eval += -8888;}
        else if (anz_O == 0 && anz_M == 0) {eval += 8888;}
        return eval;
    }
    //
    else if (player == -1) {
        int anz_X = 0;
        int anz_O = 0;
        int anz_W = 0;
        int anz_M = 0;
        for (size_t l = 0; l < pos.size(); ++l) {
            for (int o = 0; o < pos[l].size(); ++o) {
                if (pos[l][o] == 1) {
                    eval += -9;
                    anz_X += 1;
                    eval += -X_matrix[l];
                }
                else if (pos[l][o] == -1) {
                    eval += 11;
                    anz_O += 1;
                    eval += O_matrix[l];
                }
                else if (pos[l][o] == 2) {
                    eval += -51;
                    anz_W += 1;
                }
                else if (pos[l][o] == -2) {
                    eval += 49;
                    anz_M += 1;
                }
            }
        }
        //
        if (anz_X == 0 && anz_W == 0) {eval += 8888;}
        else if (anz_O == 0 && anz_M == 0) {eval += 8888;}
        return eval;
    }
    return eval;
}

//

std::vector x_inputs={'a','b','c', 'd', 'e', 'f', 'g', 'h'};
std::vector y_inputs={8,7,6,5,4,3,2,1};

class HumanPlayer {
public:
    HumanPlayer(int token, std::vector<std::vector<int>> board) : token(token) {}
    //braucht board nicht, ist aber schoener, besser bei austausche zwischen Klassen MCTS und Minimax
    int token=token;

    std::vector<int> eingabe() {
        try {
            char vx_i, zx_i;
            int vy_i, zy_i;
            std::cout <<"von x: ";
            std::cin>> vx_i;
            std::cout <<"von y: ";
            std::cin>> vy_i;
            std::cout <<"zu x: ";
            std::cin>> zx_i;
            std::cout <<"zu y: ";
            std::cin >> zy_i;
            //
            auto it_vx = std::find(x_inputs.begin(), x_inputs.end(), vx_i);
            auto it_zx = std::find(x_inputs.begin(), x_inputs.end(), zx_i);
            auto it_vy = std::find(y_inputs.begin(), y_inputs.end(), vy_i);
            auto it_zy = std::find(y_inputs.begin(), y_inputs.end(), zy_i);
            int vx_index = std::distance(x_inputs.begin(), it_vx);
            int zx_index = std::distance(x_inputs.begin(), it_zx);
            int vy_index = std::distance(y_inputs.begin(), it_vy);
            int zy_index = std::distance(y_inputs.begin(), it_zy);
            int vy=vy_index;
            int zy=zy_index;
            int vx=vx_index;
            int zx=zx_index;
            //
            if (vy < 8 && vy > -1 && vx < 8 && vx > -1 && zy < 8 && zy > -1 && zx < 8 && zx > -1 && is_int(vx) && is_int(vy)&& is_int(zx)&& is_int(zy)) {
                return {vy, vx, zy, zx};
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "EINGABE NICHT KORREKT0" << std::endl;
                return eingabe();
            }
        }
        catch (...) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "EINGABE NICHT KORREKT1" << std::endl;
            return eingabe();
        }
    }

    std::vector<int> eingabe_schlagen_XO(int vy, int vx, const std::vector<std::vector<int>>& pos, int token) {
        std::vector<int> result;
        //
        bool korrekt = false;
        //
        try {
            char zx_i;
            int zy_i;
            std::cout << "zu x: ";
            std::cin >> zx_i;
            std::cout << "zu y: ";
            std::cin >> zy_i;
            auto it_zx = std::find(x_inputs.begin(), x_inputs.end(), zx_i);
            auto it_zy = std::find(y_inputs.begin(), y_inputs.end(), zy_i);
            int zy_index = std::distance(y_inputs.begin(), it_zy);
            int zx_index = std::distance(x_inputs.begin(), it_zx);
            int zy=zy_index;
            int zx=zx_index;
            //
            if (!is_int(zy) || !is_int(zx)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "EINGABE NICHT KORREKT3" << std::endl;
                return eingabe_schlagen_XO(vy, vx, pos, token);
            }
            //
            if (zx == vx && zy == vy) {korrekt = true;}
            if (zy < 8 && zy > -1 && zx < 8 && zx > -1) {
                if (token == 1) {
                    if (zy == vy - 2 && zx == vx - 2 && pos[vy][vx] == 1 && pos[zy][zx] == 0 && pos[vy - 1][vx - 1] < 0) {korrekt = true;}
                    else if (zy == vy - 2 && zx == vx + 2 && pos[vy][vx] == 1 && pos[zy][zx] == 0 && pos[vy - 1][vx + 1] < 0) {korrekt = true;}
                }
                else if (token ==-1) {
                    if (zy == vy + 2 && zx == vx - 2 && pos[vy][vx] == -1 && pos[zy][zx] == 0 && pos[vy + 1][vx - 1] > 0) {korrekt = true;}
                    else if (zy == vy + 2 && zx == vx + 2 && pos[vy][vx] == -1 && pos[zy][zx] == 0 && pos[vy + 1][vx + 1] > 0) {korrekt = true;}
                }
            }
            //
            if (korrekt) {result = {zy, zx}; return result;}
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "EINGABE NICHT KORREKT3" << std::endl;
                return eingabe_schlagen_XO(vy, vx, pos, token);}
            //
        }
        catch (...) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "EINGABE NICHT KORREKT3" << std::endl;
            return eingabe_schlagen_XO(vy, vx, pos, token);
        }
    }

    std::vector<int> eingabe_schlagen_WM(int vy, int vx, const std::vector<std::vector<int>>& pos, int token) {
        std::vector<int> result;
        //
        bool korrekt = false;
        //
        try {
            char zx_i;
            int zy_i;
            std::cout << "zu x: ";
            std::cin >> zx_i;
            std::cout << "zu y: ";
            std::cin >> zy_i;
            auto it_zx = std::find(x_inputs.begin(), x_inputs.end(), zx_i);
            auto it_zy = std::find(y_inputs.begin(), y_inputs.end(), zy_i);
            int zy_index = std::distance(y_inputs.begin(), it_zy);
            int zx_index = std::distance(x_inputs.begin(), it_zx);
            int zy=zy_index;
            int zx=zx_index;
            //
            if (!is_int(zy) || !is_int(zx)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "EINGABE NICHT KORREKT4" << std::endl;
                return eingabe_schlagen_WM(vy, vx, pos, token);
            }
            if (zx == vx && zy == vy) {korrekt = true;}
            if (zy<8 && zy>-1 && zx<8 && zx>-1) {
                if (token==1) {
                    //
                    for (int i = 0; i < 7; ++i) {
                        if (vy + 2 + i > 7 || vx + 2 + i > 7) {break;}
                        if (pos[vy + 1 + i][vx + 1 + i] > 0) {break;}
                        if (pos[vy + 1 + i][vx + 1 + i] < 0 && pos[vy + 2 + i][vx + 2 + i] == 0 && vy + 2 + i == zy && vx + 2 + i == zx) {
                            korrekt = true; break;}
                    }
                    //
                    for (int i = 0; i < 7; ++i) {
                        if (vy + 2 + i > 7 || vx - 2 - i < 0) {break;}
                        if (pos[vy + 1 + i][vx - 1 - i] > 0) {break;}
                        if (pos[vy + 1 + i][vx - 1 - i] < 0 && pos[vy + 2 + i][vx - 2 - i] == 0 && vy + 2 + i == zy && vx - 2 - i == zx) {korrekt = true; break; }
                    }
                    //
                    for (int i = 0; i < 7; ++i) {
                        if (vy - 2 - i < 0 || vx - 2 - i < 0) {break;}
                        if (pos[vy - 1 - i][vx - 1 - i] > 0) {break;}
                        if (pos[vy - 1 - i][vx - 1 - i] < 0 && pos[vy - 2 - i][vx - 2 - i] == 0 && vy - 2 - i == zy && vx - 2 - i == zx) {korrekt = true; break;}
                    }
                    //
                    for (int i = 0; i < 7; ++i) {
                        if (vy - 2 - i < 0 || vx + 2 + i > 7) {break;}
                        if (pos[vy - 1 - i][vx + 1 + i] > 0) {break;}
                        if (pos[vy - 1 - i][vx + 1 + i] < 0 && pos[vy - 2 - i][vx + 2 + i] == 0 && vy - 2 - i == zy && vx + 2 + i == zx) {korrekt = true; break;}
                    }
                }
                else if (token == -1) {
                    //
                    for (int i = 0; i < 7; ++i) {
                        if (vy + 2 + i > 7 || vx + 2 + i > 7) {break;}
                        if (pos[vy + 1 + i][vx + 1 + i] < 0) {break;}
                        if (pos[vy + 1 + i][vx + 1 + i] > 0 && pos[vy + 2 + i][vx + 2 + i] == 0 && vy + 2 + i == zy && vx + 2 + i == zx) {korrekt = true; break;}
                    }
                    //
                    for (int i = 0; i < 7; ++i) {
                        if (vy + 2 + i > 7 || vx - 2 - i < 0) {break;}
                        if (pos[vy + 1 + i][vx - 1 - i] < 0) {break;}
                        if (pos[vy + 1 + i][vx - 1 - i] > 0 && pos[vy + 2 + i][vx - 2 - i] == 0 && vy + 2 + i == zy && vx - 2 - i == zx) {korrekt = true; break;}
                    }
                    //
                    for (int i = 0; i < 7; ++i) {
                        if (vy - 2 - i < 0 || vx - 2 - i < 0) {break;}
                        if (pos[vy - 1 - i][vx - 1 - i] < 0) {break;}
                        if (pos[vy - 1 - i][vx - 1 - i] > 0 && pos[vy - 2 - i][vx - 2 - i] == 0 && vy - 2 - i == zy && vx - 2 - i == zx) {korrekt = true; break;}
                    }
                    //
                    for (int i = 0; i < 7; ++i) {
                        if (vy - 2 - i < 0 || vx + 2 + i > 7) {break;}
                        if (pos[vy - 1 - i][vx + 1 + i] < 0) {break;}
                        if (pos[vy - 1 - i][vx + 1 + i] > 0 && pos[vy - 2 - i][vx + 2 + i] == 0 && vy - 2 - i == zy && vx + 2 + i == zx) {korrekt = true; break;}
                    }
                }
            }
            //
            if (korrekt) {result = {zy, zx}; return result;}
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "EINGABE NICHT KORREKT4" << std::endl;
                return eingabe_schlagen_WM(vy, vx, pos, token);
            }
        }
        //
        catch (...) {
            std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "EINGABE NICHT KORREKT4" << std::endl;
                return eingabe_schlagen_WM(vy, vx, pos, token);
        }
    }

    //

    bool schlagen_moeglich_XO(int y, int x, const std::vector<std::vector<int>>& pos, int token) {
        bool moeglich = false;
        //
        if (token==1) {
            if (y-2>-1 && x-2>-1) {
                if (pos[y-2][x-2] == 0 && pos[y-1][x-1] <0) {moeglich = true;}
            if (y-2 >-1 && x+2<8) {
                if (pos[y - 2][x + 2] == 0 && pos[y - 1][x + 1] <0) {moeglich = true;}
            }
        }
        else if (token==-1) {
            if (y+2<8 && x-2>-1) {
                if (pos[y+2][x-2] == 0 && pos[y+1][x-1] >0) {moeglich = true;}
            }
            if (y+2<8 && x+2<8) {
                if (pos[y+2][x+2] == 0 && pos[y+1][x+1] >0) {moeglich = true;}
            }
        }
        //
        return moeglich;
        }
    }

    bool schlagen_moeglich_WM(int y, int x, const std::vector<std::vector<int>>& pos, int token) {
        bool moeglich=false;

        if (token==1) {
            for (int i = 0; i < 7; ++i) {
                if (y + 2 + i > 7 || x + 2 + i > 7) {break;}
                if (pos[y + 1 + i][x + 1 + i] > 0) {break;}
                if (pos[y + 1 + i][x + 1 + i] < 0) {
                    if (pos[y + 2 + i][x + 2 + i] == 0) {moeglich = true;break;}
                    else {break;}
                }
            }
            if (!moeglich) {
                for (int i = 0; i < 7; ++i) {
                    if (y - 2 - i < 0 || x + 2 + i > 7) {break;}
                    if (pos[y - 1 - i][x + 1 + i] > 0) {break;}
                    if (pos[y - 1 - i][x + 1 + i] < 0) {
                        if (pos[y - 2 - i][x + 2 + i] == 0) {moeglich = true; break;}
                        else {break;}
                    }
                }
            }
            if (!moeglich) {
                for (int i = 0; i < 7; ++i) {
                    if (y - 2 - i < 0 || x - 2 - i < 0) {break;}
                    if (pos[y - 1 - i][x - 1 - i] > 0) {break;}
                    if (pos[y - 1 - i][x - 1 - i] < 0) {
                        if (pos[y - 2 - i][x - 2 - i] == 0) {moeglich = true; break;}
                        else {break;}
                    }
                }
            }
            if (!moeglich) {
                for (int i = 0; i < 7; ++i) {
                    if (y + 2 + i > 7 || x - 2 - i < 0) {break;}
                    if (pos[y + 1 + i][x - 1 - i] > 0) {break;}
                    if (pos[y + 1 + i][x - 1 - i] < 0) {
                        if (pos[y + 2 + i][x - 2 - i] == 0) {moeglich = true; break;}
                        else {break;}
                    }
                }
            }
        }
        //
        else if (token == -1) {
            for (int i = 0; i < 7; ++i) {
                if (y + 2 + i > 7 || x + 2 + i > 7) {break;}
                if (pos[y + 1 + i][x + 1 + i] < 0) {
                    break;
                }
                if (pos[y + 1 + i][x + 1 + i] > 0) {
                    if (pos[y + 2 + i][x + 2 + i] == 0) {
                        moeglich = true;
                        break;
                    } else {
                        break;
                    }
                }
            }
            if (!moeglich) {
                for (int i = 0; i < 7; ++i) {
                    if (y - 2 - i < 0 || x + 2 + i > 7) {break;}
                    if (pos[y - 1 - i][x + 1 + i] < 0) {break;}
                    if (pos[y - 1 - i][x + 1 + i] > 0) {
                        if (pos[y - 2 - i][x + 2 + i] == 0) {moeglich = true; break;}
                        else {break;}
                    }
                }
            }
            if (!moeglich) {
                for (int i = 0; i < 7; ++i) {
                    if (y - 2 - i < 0 || x - 2 - i < 0) {break;}
                    if (pos[y - 1 - i][x - 1 - i] < 0) {break;}
                    if (pos[y - 1 - i][x - 1 - i] > 0) {
                        if (pos[y - 2 - i][x - 2 - i] == 0) {moeglich = true; break;}
                        else {break;}
                    }
                }
            }
            if (!moeglich) {
                for (int i = 0; i < 7; ++i) {
                    if (y + 2 + i > 7 || x - 2 - i < 0) {break;}
                    if (pos[y + 1 + i][x - 1 - i] < 0) {break;}
                    if (pos[y + 1 + i][x - 1 - i] > 0) {
                        if (pos[y + 2 + i][x - 2 - i] == 0) {moeglich = true; break;}
                        else {break;}
                    }
                }
            }
        }
        //
        return moeglich;
    }

    //

    std::vector<std::vector<int>> player_schlagen_chain_XO(int vy, int vx, std::vector<std::vector<int>>& pos, int token) {
        if (schlagen_moeglich_XO(vy, vx, pos, token)) {
            print_board(pos);
            //
            std::vector<int> input_move = eingabe_schlagen_XO(vy, vx, pos, token);
            int zy=input_move[0];
            int zx=input_move[1];
            //
            if (zx==vx && zy==vy) {}//um nicht mehr zu shlagen
            //
            if (token==1) {
                if (zy==vy-2 && zx==vx-2) {
                    pos[vy][vx]= 0;
                    pos[zy][zx]= 1;
                    if (zy==0) {pos[zy][zx]=2;}
                    pos[vy -1][vx -1] = 0;
                    print_board(pos);
                    //
                    vy=zy;
                    vx=zx;
                    return player_schlagen_chain_XO(vy, vx, pos, token);
                }
                else if (zy==vy-2 && zx == vx + 2) {
                    pos[vy][vx]= 0;
                    pos[zy][zx]= 1;
                    if (zy==0) {pos[zy][zx]= 2;}
                    pos[vy -1][vx +1]= 0;
                    print_board(pos);
                    //
                    vy=zy;
                    vx=zx;
                    return player_schlagen_chain_XO(vy, vx, pos, token);
                }
            }
            //
            else if (token==-1) {
                if (zy==vy+2 && zx==vx-2) {
                    pos[vy][vx]= 0;
                    pos[zy][zx]= -1;
                    if (zy==0) {pos[zy][zx]= -2;}
                    pos[vy +1][vx -1]= 0;
                    print_board(pos);
                    //
                    vy=zy;
                    vx=zx;
                    return player_schlagen_chain_XO(vy, vx, pos, token);
                }
                else if (zy==vy+2 && zx==vx+2) {
                    pos[vy][vx]= 0;
                    pos[zy][zx]= -1;
                    if (zy==0) {pos[zy][zx]= -2;}
                    pos[vy +1][vx +1] = 0;
                    print_board(pos);
                    //
                    vy=zy;
                    vx=zx;
                    return player_schlagen_chain_XO(vy, vx, pos, token);
                }
            }
        }
        //
        else {return pos;}
    }

    std::vector<std::vector<int>> player_schlagen_chain_WM(int vy, int vx, std::vector<std::vector<int>>& pos, int token) {
        if (schlagen_moeglich_WM(vy, vx, pos, token)) {
            print_board(pos);
            //
            std::vector<int> input_move = eingabe_schlagen_WM(vy, vx, pos, token);
            int zy=input_move[0];
            int zx=input_move[1];
            //
            if (zx==vx && zy==vy) {} // um nicht mehr zu schlagen
            //
            if (token==1) {
                if (zy<vy && zx<vx) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = 2;
                    pos[zy + 1][zx + 1] = 0;
                    print_board(pos);
                    //
                    vy = zy;
                    vx = zx;
                    return player_schlagen_chain_WM(vy, vx, pos, token);
                }
                else if (zy>vy && zx<vx) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = 2;
                    pos[zy - 1][zx + 1] = 0;
                    print_board(pos);
                    //
                    vy = zy;
                    vx = zx;
                    return player_schlagen_chain_WM(vy, vx, pos, token);
                }
                else if (zy<vy && zx>vx) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = 2;
                    pos[zy + 1][zx - 1] = 0;
                    print_board(pos);
                    //
                    vy=zy;
                    vx=zx;
                    return player_schlagen_chain_WM(vy, vx, pos, token);
                }
                else if (zy>vy && zx>vx) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = 2;
                    pos[zy - 1][zx + 1] = 0;
                    print_board(pos);
                    //
                    vy=zy;
                    vx=zx;
                    return player_schlagen_chain_WM(vy, vx, pos, token);
                }
            }
            else if (token==-1) {
                if (zy<vy && zx<vx) {
                    pos[vy][vx]= 0;
                    pos[zy][zx]= -2;
                    pos[zy + 1][zx + 1]= 0;
                    print_board(pos);
                    //
                    vy=zy;
                    vx=zx;
                    return player_schlagen_chain_WM(vy, vx, pos, token);
                }
                else if (zy>vy && zx<vx) {
                    pos[vy][vx]= 0;
                    pos[zy][zx]= -2;
                    pos[zy - 1][zx + 1]= 0;
                    print_board(pos);
                    //
                    vy=zy;
                    vx=zx;
                    return player_schlagen_chain_WM(vy, vx, pos, token);
                }
                else if (zy<vy && zx>vx) {
                    pos[vy][vx]= 0;
                    pos[zy][zx]= -2;
                    pos[zy + 1][zx - 1]= 0;
                    print_board(pos);
                    //
                    vy=zy;
                    vx=zx;
                    return player_schlagen_chain_WM(vy, vx, pos, token);
                }
                else if (zy>vy && zx>vx) {
                    pos[vy][vx]= 0;
                    pos[zy][zx]= -2;
                    pos[zy - 1][zx + 1]= 0;
                    print_board(pos);
                    //
                    vy=zy;
                    vx=zx;
                    return player_schlagen_chain_WM(vy, vx, pos, token);
                }
            }
        }
        else {return pos;}
    }

    //

    std::vector<std::vector<int>> player(std::vector<std::vector<int>>& pos) {
        while (true) {
            std::vector<int> input_move = eingabe();
            int vy=input_move[0];
            int vx=input_move[1];
            int zy=input_move[2];
            int zx=input_move[3];
            //
            if (token == 1) {
                //X normal
                if (zy == vy - 1 && zx == vx - 1 && pos[vy][vx] == 1 && pos[zy][zx] == 0) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = 1;
                    if (zy == 0) {pos[zy][zx] = 2;}
                    return pos;
                }
                else if (zy == vy - 1 && zx == vx + 1 && pos[vy][vx] == 1 && pos[zy][zx] == 0) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = 1;
                    if (zy == 0) {pos[zy][zx] = 2;}
                    return pos;
                }
                //schlagen
                else if (zy == vy - 2 && zx == vx - 2 && pos[vy][vx] == 1 && pos[zy][zx] == 0 && pos[vy - 1][vx - 1] < 0) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = 1;
                    if (zy == 0) {pos[zy][zx] = 2;}
                    pos[vy - 1][vx - 1] = 0;
                    return player_schlagen_chain_XO(zy, zx, pos, this->token);
                }
                else if (zy == vy - 2 && zx == vx + 2 && pos[vy][vx] == 1 && pos[zy][zx] == 0 && pos[vy - 1][vx + 1] < 0) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = 1;
                    if (zy == 0) {pos[zy][zx] = 2;}
                    pos[vy - 1][vx + 1] = 0;
                    return player_schlagen_chain_XO(zy, zx, pos, this->token);
                }
                // M normal + erstes Mal schlagen
                else if (pos[vy][vx] == 2 && pos[zy][zx] == 0 && zy < vy && zx < vx) {
                    for (int i = 0; i < 7; ++i) {
                        if (vy - i - 1 < 0 || vx - i - 1 < 0) {break;}
                        if (vy - i - 1 == zy && vx - i - 1 == zx) {
                            pos[vy][vx] = 0;
                            pos[zy][zx] = 2;
                            return pos;
                        }
                        if (pos[vy - i - 1][vx - i - 1] > 0) {break;}
                        if (pos[vy - i - 1][vx - i - 1] < 0) {
                            if (!(vy - i - 2 < 0 || vx - i - 2 < 0)) {
                                if (vy - i - 2 == zy && vx - i - 2 == zx) {
                                    pos[vy][vx] = 0;
                                    pos[zy][zx] = 2;
                                    pos[vy - i - 1][vx - i - 1] = 0;
                                    return player_schlagen_chain_WM(zy, zx, pos, this->token);
                                }
                                else {break;}
                            }
                            else {break;}
                        }
                    }
                }
                else if (pos[vy][vx] == 2 && pos[zy][zx] == 0 && zy > vy && zx < vx) {
                    for (int i = 0; i < 7; ++i) {
                        if (vy + i + 1 > 7 || vx - i - 1 < 0) {break;}
                        if (vy + i + 1 == zy && vx - i - 1 == zx) {
                            pos[vy][vx] = 0;
                            pos[zy][zx] = 2;
                            return pos;
                        }
                        if (pos[vy + i + 1][vx - i - 1] > 0) {break;}
                        if (pos[vy + i + 1][vx - i - 1] < 0) {
                            if (!(vy + i + 2 > 7 || vx - i - 2 < 0)) {
                                if (vy + i + 2 == zy && vx - i - 2 == zx) {
                                    pos[vy][vx] = 0;
                                    pos[zy][zx] = 2;
                                    pos[vy + i + 1][vx - i - 1] = 0;
                                    return player_schlagen_chain_WM(zy, zx, pos, this->token);
                                }
                                else {break;}
                            }
                            else {break;}
                        }
                    }
                }
                else if (pos[vy][vx] == 2 && pos[zy][zx] == 0 && zy < vy && zx > vx) {
                    for (int i = 0; i < 7; ++i) {
                        if (vy - i - 1 < 0 || vx + i + 1 > 7) {break;}
                        if (vy - i - 1 == zy && vx + i + 1 == zx) {
                            pos[vy][vx] = 0;
                            pos[zy][zx] = 2;
                            return pos;
                        }
                        if (pos[vy - i - 1][vx + i + 1] > 0) {break;}
                        if (pos[vy - i - 1][vx + i + 1] < 0) {
                            if (!(vy - i - 2 < 0 || vx + i + 2 > 7)) {
                                if (vy - i - 2 == zy && vx + i + 2 == zx) {
                                    pos[vy][vx] = 0;
                                    pos[zy][zx] = 2;
                                    pos[vy - i - 1][vx + i + 1] = 0;
                                    return player_schlagen_chain_WM(zy, zx, pos, this->token);
                                }
                                else {break;}
                            }
                            else {break;}
                        }
                    }
                }
                else if (pos[vy][vx] == 2 && pos[zy][zx] == 0 && zy > vy && zx > vx) {
                    for (int i = 0; i < 7; ++i) {
                        if (vy + i + 1 > 7 || vx + i + 1 > 7) {break;}
                        if (vy + i + 1 == zy && vx + i + 1 == zx) {
                            pos[vy][vx] = 0;
                            pos[zy][zx] = 2;
                            return pos;
                        }
                        if (pos[vy + i + 1][vx + i + 1] > 0) {break;}
                        if (pos[vy + i + 1][vx + i + 1] < 0) {
                            if (!(vy + i + 2 > 7 || vx + i + 2 > 7)) {
                                if (vy + i + 2 == zy && vx + i + 2 == zx) {
                                    pos[vy][vx] = 0;
                                    pos[zy][zx] = 2;
                                    pos[vy + i + 1][vx + i + 1] = 0;
                                    return player_schlagen_chain_WM(zy, zx, pos, this->token);
                                }
                                else {break;}
                            }
                            else {break;}
                        }
                    }
                }
            }
            //
            else if (token == -1) {
                if (zy == vy + 1 && zx == vx - 1 && pos[vy][vx] == -1 && pos[zy][zx] == 0) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = -1;
                    if (zy == 7) {pos[zy][zx] = -2;}
                    return pos;
                }
                else if (zy == vy + 1 && zx == vx + 1 && pos[vy][vx] == -1 && pos[zy][zx] == 0) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = -1;
                    if (zy == 7) {pos[zy][zx] = -2;}
                    return pos;
                }
                else if (zy == vy + 2 && zx == vx - 2 && pos[vy][vx] == -1 && pos[zy][zx] == 0 && pos[vy + 1][vx - 1] > 0) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = -1;
                    if (zy == 0) {pos[zy][zx] = -2;}
                    pos[vy + 1][vx - 1] = 0;
                    return player_schlagen_chain_XO(zy, zx, pos, this->token);
                }
                else if (zy == vy + 2 && zx == vx + 2 && pos[vy][vx] == -1 && pos[zy][zx] == 0 && pos[vy + 1][vx + 1] > 0) {
                    pos[vy][vx] = 0;
                    pos[zy][zx] = -1;
                    if (zy == 0) {pos[zy][zx] = -2;}
                    pos[vy + 1][vx + 1] = 0;
                    return player_schlagen_chain_XO(zy, zx, pos, this->token);
                }
                else if (pos[vy][vx] == -2 && pos[zy][zx] == 0 && zy < vy && zx < vx) {
                    for (int i = 0; i < 7; ++i) {
                        if (vy - i - 1 < 0 || vx - i - 1 < 0) {break;}
                        if (vy - i - 1 == zy && vx - i - 1 == zx) {
                            pos[vy][vx] = 0;
                            pos[zy][zx] = -2;
                            return pos;
                        }
                        if (pos[vy - i - 1][vx - i - 1] < 0) {break;}
                        if (pos[vy - i - 1][vx - i - 1] > 0) {
                            if (!(vy - i - 2 < 0 || vx - i - 2 < 0)) {
                                if (vy - i - 2 == zy && vx - i - 2 == zx) {
                                    pos[vy][vx] = 0;
                                    pos[zy][zx] = -2;
                                    pos[vy - i - 1][vx - i - 1] = 0;
                                    return player_schlagen_chain_WM(zy, zx, pos, this->token);
                                }
                                else {break;}
                            }
                            else {break;}
                        }
                    }
                }
                else if (pos[vy][vx] == -2 && pos[zy][zx] == 0 && zy > vy && zx < vx) {
                    for (int i = 0; i < 7; ++i) {
                        if (vy + i + 1 > 7 || vx - i - 1 < 0) {break;}
                        if (vy + i + 1 == zy && vx - i - 1 == zx) {
                            pos[vy][vx] = 0;
                            pos[zy][zx] = -2;
                            return pos;
                        }
                        if (pos[vy + i + 1][vx - i - 1] < 0) {break;}
                        if (pos[vy + i + 1][vx - i - 1] > 0) {
                            if (!(vy + i + 2 > 7 || vx - i - 2 < 0)) {
                                if (vy + i + 2 == zy && vx - i - 2 == zx) {
                                    pos[vy][vx] = 0;
                                    pos[zy][zx] = -2;
                                    pos[vy + i + 1][vx - i - 1] = 0;
                                    return player_schlagen_chain_WM(zy, zx, pos, this->token);
                                }
                                else {break;}
                            }
                            else {break;}
                        }
                    }
                }
                else if (pos[vy][vx] == -2 && pos[zy][zx] == 0 && zy < vy && zx > vx) {
                    for (int i = 0; i < 7; ++i) {
                        if (vy - i - 1 < 0 || vx + i + 1 > 7) {break;}
                        if (vy - i - 1 == zy && vx + i + 1 == zx) {
                            pos[vy][vx] = 0;
                            pos[zy][zx] = -2;
                            return pos;
                        }
                        if (pos[vy - i - 1][vx + i + 1] < 0) {break;}
                        if (pos[vy - i - 1][vx + i + 1] > 0) {
                            if (!(vy - i - 2 < 0 || vx + i + 2 > 7)) {
                                if (vy - i - 2 == zy && vx + i + 2 == zx) {
                                    pos[vy][vx] = 0;
                                    pos[zy][zx] = -2;
                                    pos[vy - i - 1][vx + i + 1] = 0;
                                    return player_schlagen_chain_WM(zy, zx, pos, this->token);
                                }
                                else {break;}
                            }
                            else {break;}
                        }
                    }
                }
                else if (pos[vy][vx] == -2 && pos[zy][zx] == 0 && zy > vy && zx > vx) {
                    for (int i = 0; i < 7; ++i) {
                        if (vy + i + 1 > 7 || vx + i + 1 > 7) {break;}
                        if (vy + i + 1 == zy && vx + i + 1 == zx) {
                            pos[vy][vx] = 0;
                            pos[zy][zx] = -2;
                            return pos;
                        }
                        if (pos[vy + i + 1][vx + i + 1] < 0) {break;}
                        if (pos[vy + i + 1][vx + i + 1] > 0) {
                            if (!(vy + i + 2 > 7 || vx + i + 2 > 7)) {
                                if (vy + i + 2 == zy && vx + i + 2 == zx) {
                                    pos[vy][vx] = 0;
                                    pos[zy][zx] = -2;
                                    pos[vy + i + 1][vx + i + 1] = 0;
                                    return player_schlagen_chain_WM(zy, zx, pos, this->token);
                                }
                                else {break;}
                            }
                            else {break;}
                        }
                    }
                }
            }
            std::cout << "EINGABE NICHT KORREKT2" << std::endl;
        }
    }

    std::vector<std::vector<int>> get_move(std::vector<std::vector<int>>& board) {
        return player(board);
    }
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
        if (this->depth==max_depth || verloren1(this->board,1) || verloren1(this->board,-1)) {
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
    int max_time=3;
    int max_depth=12;
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
        std::cout << std::endl;
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
            //break
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> vergangene_zeit = now - start;
            if (vergangene_zeit.count() >= max_time) {break;}
            else if (depth>max_depth) {break;}
            //calculate move
            std::cout<<"---DEPTH: ";
            std::cout<<depth<<std::endl;
            //
            std::vector<std::vector<int>> new_move=minimaxer(depth,vergangene_zeit);
            //
            for (MinimaxNode& child : root_node.children) {std::cout<<child.value;  std::cout<<", ";}
            std::cout<<std::endl;
            //sort+depth
            //this->root_node.sort(true);
            move=new_move;
            //
            for (MinimaxNode& child : root_node.children) {std::cout<<child.value;  std::cout<<", ";}
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

double c=std::sqrt(2);
int number_of_simulations=20;
int depth=2;

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
    int max_time=1;

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

int max_turns=70;

class Dame {
public:
    Dame() : board(), turn(1) {
            board={
            {0,-1,0,-1,0,-1,0,-1},
            {-1,0,-1,0,-1,0,-1,0},
            {0,-1,0,-1,0,-1,0,-1},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {1,0,1,0,1,0,1,0},
            {0,1,0,1,0,1,0,1},
            {1,0,1,0,1,0,1,0},

        };
    }

    int play() {
        int current = 1;

        while (true) {
            //-----------------------------------------
            HumanPlayer player_1(1, this->board);
            MinimaxPlayer player_2(-1, this->board);
            //-----------------------------------------
            std::cout<<this->turn<<std::endl;
            print_board(this->board);

            if (current==1) {
                std::cout <<"Weiss ist am Zug"<<std::endl;
                std::vector<std::vector<int>> board_copy = deepcopy(this->board);
                std::vector<std::vector<int>> new_board = player_1.get_move(board_copy);
                this->board=new_board;
            }
            else if (current==2) {
                std::cout<<"Schwarz ist am Zug"<<std::endl;
                std::vector<std::vector<int>> board_copy = deepcopy(this->board);
                std::vector<std::vector<int>> new_board = player_2.get_move(board_copy);
                this->board=new_board;
            }
            //
            if (verloren2(this->board, -1)) {print_board(this->board); std::cout<<"WEISS HAT GEWONNEN"<<std::endl; return 1;}
            else if (verloren2(this->board, 1)) {print_board(this->board); std::cout<<"SCHWARZ HAT GEWONNEN"<<std::endl; return -1;}
            else if (this->turn==max_turns) {print_board(this->board); std::cout<<"UNENTSCHIEDEN"<<std::endl; return 0;}
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
        Dame game;
        int r = game.play();
        if (r==1) {x_wins+=1;}
        else if (r== -1) {o_wins+=1;}
        else if (r==0) {unentschieden+= 1;}
        std::cout<<"Weiss: "<<x_wins<<std::endl;
        std::cout<<"Schwarz: "<<o_wins<<std::endl;
        std::cout<<"-: "<<unentschieden<<std::endl;
    }
    std::cout<<"FERTIG"<<std::endl;
}

//

int main() {
    srand(time(0)); //seed
    spielen(3);
}
