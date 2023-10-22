#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <chrono>
#include <cmath>
#include <sstream>
#include <string>

//-1:B
//-2:L
//-3:X
//-4:T
//-5:Q
//-6:K
//-7:Z not moved towerT
//-8:Y not moved kingK
//-9:F en passant B (just moved 2 forw)

//1:b
//2:l
//3:x
//4:t
//5:q
//6:k
//7:z not moved towert
//8:y not moved kingk
//9:f en passant b (just moved 2 forw)


void print_bitboard(const uint64_t bitboard) {
    std::vector<int> bits;
    uint64_t m = 1ULL;
    //
    for (int i=0; i<64; ++i) {
        if (bitboard & m) {bits.push_back(1);}
        else {bits.push_back(0);}
        m <<= 1;
    }
    //
    std::reverse(bits.begin(), bits.end());
    //
    for (int i=0; i<bits.size(); ++i) {
        std::cout<<bits[i]<<" ";
        if ((i+1) % 8 == 0) {std::cout << std::endl;}
    }
}

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

//

bool is_one_at_this_index(const uint64_t bitboard, int index) {
    uint64_t m = 1ULL << index; //m=00000....001 1 wird um index rightshifted
    return (((bitboard & m) != 0)); 
}

int yx_zu_index(int y, int x) {
    return (y*8+x); //fangt unten rechts an!
}

bool is_in_board(int y, int x) {
    return ((x>-1 && x<8) && (y>-1 && y<8));
}

uint64_t shift_bit(uint64_t bitboard, int vy, int vx, int zy, int zx) {//wenn (vy, vx)==1
    //clear bit at (vy, vx)
    bitboard &= ~(1ULL << (vy * 8 + vx));
    //set bit at (zy, zx) to 1
    bitboard |= (1ULL << (zy * 8 + zx));
    return bitboard;
}

uint64_t clear_bit(uint64_t bitboard, int index) {
    //m with a 0 bit at index and 1s everywhere else
    //if already 0: does nothing
    uint64_t m = ~(1ULL << index);
    return bitboard & m;
}

uint64_t set_bit_to_one(uint64_t bitboard, int index) {
    uint64_t mask = 1ULL << index;
    return bitboard | mask;
}

uint64_t remove_common_bits(uint64_t from_this_bitboard, uint64_t by_comparing_with_this_board) {
    //find common bits by AND operation
    uint64_t common_bits = from_this_bitboard & by_comparing_with_this_board;
    //remove common bits from bitboard1
    uint64_t result = from_this_bitboard & (~common_bits);
    return result;
}

int count_bits_in_bitboard(uint64_t bitboard) {
    int counter = 0;
    uint64_t m = 1ULL;
    for (int i = 0; i < 64; i++) {
        if ((bitboard & m)!=0) {
            counter+=1;
        }
        m <<= 1;
    }
    return counter;
}

//

//Tt: {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}
//Xx: {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}}
//Qq: {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}}

std::vector<uint64_t> gcTtXxQq(const uint64_t bitboard, const uint64_t this_players_pieces, const uint64_t other_players_pieces, std::vector<std::pair<int,int>> directions) {
    std::vector<uint64_t> children_Tt_Xx_Qq;
    //get current positions as (x, y) coord.
    std::vector<std::pair<int,int>> positions;
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (is_one_at_this_index(bitboard, yx_zu_index(y, x))) {positions.push_back({y, x});}
        }
    }
    //
    //modify board
    for (const auto& position : positions) {
        int y_current = position.first;
        int x_current = position.second;
        //
        for (const auto& direction : directions) {
            int direction_y=direction.first;
            int direction_x=direction.second;
            //
            for (int i=1; i<8; ++i) {
                int y_new = y_current + i*direction_y;
                int x_new = x_current + i*direction_x;
                //
                if (is_in_board(y_new, x_new)) {
                    if (is_one_at_this_index(other_players_pieces, yx_zu_index(y_new, x_new))) {
                        uint64_t child=shift_bit(bitboard, y_current, x_current, y_new, x_new);
                        children_Tt_Xx_Qq.push_back(child);
                        break;
                    }
                    else if (is_one_at_this_index(this_players_pieces, yx_zu_index(y_new, x_new))) {break;}
                    else {
                        uint64_t child=shift_bit(bitboard, y_current, x_current, y_new, x_new);
                        children_Tt_Xx_Qq.push_back(child);
                    }
                }
                else {break;}
            }
        }
    }
    //
    return children_Tt_Xx_Qq;
}

std::vector<std::pair<uint64_t, uint64_t>> gcZz(const uint64_t Zz_bitboard, const uint64_t Tt_bitboard, const uint64_t this_players_pieces, const uint64_t other_players_pieces) {
    std::vector<std::pair<int,int>> directions={{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    std::vector<std::pair<uint64_t, uint64_t>> children_Tt_Zz;
    //
    //position von Zz mit yx finden
    std::vector<std::pair<int,int>> positions;
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (is_one_at_this_index(Zz_bitboard, yx_zu_index(y, x))) {positions.push_back({y, x});}
        }
    }
    //
    //modify board
    for (const auto& position : positions) {
        int y_current = position.first;
        int x_current = position.second;
        //
        for (const auto& direction : directions) {
            int direction_y=direction.first;
            int direction_x=direction.second;
            //
            for (int i=1; i<8; ++i) {
                int y_new = y_current + i*direction_y;
                int x_new = x_current + i*direction_x;
                //
                if (is_in_board(y_new, x_new)) {
                    if (is_one_at_this_index(other_players_pieces, yx_zu_index(y_new, x_new))) {
                        uint64_t child_Tt=set_bit_to_one(Tt_bitboard, yx_zu_index(y_new, x_new));
                        uint64_t child_Zz=clear_bit(Zz_bitboard, yx_zu_index(y_current, x_current));
                        children_Tt_Zz.push_back({child_Tt, child_Zz});
                        break;
                    }
                    else if (is_one_at_this_index(this_players_pieces, yx_zu_index(y_new, x_new))) {break;}
                    else {
                        uint64_t child_Tt=set_bit_to_one(Tt_bitboard, yx_zu_index(y_new, x_new));
                        uint64_t child_Zz=clear_bit(Zz_bitboard, yx_zu_index(y_current, x_current));
                        children_Tt_Zz.push_back({child_Tt, child_Zz});
                    }
                }
                else {break;}
            }
        }
    }
    //
    return children_Tt_Zz;//{child_Tt, child_Zz}
}

std::vector<uint64_t> gcKk(const uint64_t king_bitboard, const uint64_t this_players_pieces) {
    std::vector<uint64_t> childrenKk;
    //
    int dx[] = {0, 0, 1, -1, 1, -1, 1, -1};
    int dy[] = {1, -1, 0, 0, 1, 1, -1, -1};
    
    //get kings current position as (x, y) coord.
    std::vector<std::pair<int,int>> king_positions;
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (is_one_at_this_index(king_bitboard, yx_zu_index(y, x))) {king_positions.push_back({y, x});}
        }
    }
    //modify boards
    for (const auto& position : king_positions) {
        int y_current = position.first;
        int x_current = position.second;
        //
        for (int i=0; i<8; ++i) {
            int y_new = y_current + dy[i];
            int x_new = x_current + dx[i];
            //
            if (is_in_board(y_new, x_new)) {
                if (!(is_one_at_this_index(this_players_pieces, yx_zu_index(y_new, x_new)))) {
                    uint64_t child=shift_bit(king_bitboard, y_current, x_current, y_new, x_new);
                    childrenKk.push_back(child);
                }
            }
        }
    }
    //
    return childrenKk;
}

std::vector<uint64_t> gcLl(const uint64_t knight_bitboard, const uint64_t this_players_pieces) {
    std::vector<uint64_t> childrenLl;
    //
    int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
    int dy[] = {2, 1, -1, -2, -2, -1, 1, 2};
    
    //get knights current position as (x, y) coord.
    std::vector<std::pair<int,int>> knight_positions;
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (is_one_at_this_index(knight_bitboard, yx_zu_index(y, x))) {knight_positions.push_back({y, x});}
        }
    }
    //modify boards
    for (const auto& position : knight_positions) {
        int y_current = position.first;
        int x_current = position.second;
        //
        for (int i=0; i<8; ++i) {
            int y_new = y_current + dy[i];
            int x_new = x_current + dx[i];
            //
            if (is_in_board(y_new, x_new)) {
                if (!(is_one_at_this_index(this_players_pieces, yx_zu_index(y_new, x_new)))) {
                    uint64_t child=shift_bit(knight_bitboard, y_current, x_current, y_new, x_new);
                    childrenLl.push_back(child);
                }
            }
        }
    }
    //
    return childrenLl;
}

std::vector<std::vector<uint64_t>> gcBb(int player, const uint64_t pawn_bitboard, const uint64_t this_players_pieces, const uint64_t this_players_Ff, const uint64_t other_players_pieces, const uint64_t other_players_Ff) {
    std::vector<std::vector<uint64_t>> childrenBb;
    //get pawns current positions as (x, y) coord.
    std::vector<std::pair<int,int>> pawn_positions;
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (is_one_at_this_index(pawn_bitboard, yx_zu_index(y, x))) {pawn_positions.push_back({y, x});}
        }
    }
    //
    uint64_t all_pieces=this_players_pieces|other_players_pieces;
    //modify boards
    for (const auto& position : pawn_positions) {
        int vy = position.first;
        int vx = position.second;
        //
        if (player==1) {
            //2 nach vorne
            if (vy==1 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy+1, vx))) && !(is_one_at_this_index(all_pieces, yx_zu_index(vy+2, vx)))) {
                uint64_t child=clear_bit(pawn_bitboard, yx_zu_index(vy,vx));
                uint64_t child_Ff=set_bit_to_one(this_players_Ff, yx_zu_index(vy+2,vx));
                childrenBb.push_back({child, child_Ff, other_players_Ff});
            }
            //normal 1 nach vorne
            if (vy+1<8 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy+1, vx)))) {
                uint64_t child=shift_bit(pawn_bitboard, vy, vx, vy+1, vx);
                childrenBb.push_back({child, this_players_Ff, other_players_Ff});
            }
            //schlagen
            if (vy+1<8 && vx-1>-1 && is_one_at_this_index(other_players_pieces, yx_zu_index(vy+1, vx-1))) {
                uint64_t child=shift_bit(pawn_bitboard, vy, vx, vy+1, vx-1);
                childrenBb.push_back({child, this_players_Ff, other_players_Ff});
            }
            if (vy+1<8 && vx+1<8 && is_one_at_this_index(other_players_pieces, yx_zu_index(vy+1, vx+1))) {
                uint64_t child=shift_bit(pawn_bitboard, vy, vx, vy+1, vx+1);
                childrenBb.push_back({child, this_players_Ff, other_players_Ff});
            }
            //en passant
            if (vy+1<8 && vx-1>-1 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy+1, vx-1))) && is_one_at_this_index(other_players_Ff, yx_zu_index(vy, vx-1))) {
                uint64_t child=shift_bit(pawn_bitboard, vy, vx, vy+1, vx-1);
                uint64_t new_o_p_p_en=clear_bit(other_players_Ff, yx_zu_index(vy, vx-1));
                childrenBb.push_back({child, this_players_Ff, new_o_p_p_en});
            }
            if (vy+1<8 && vx+1<8 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy+1, vx+1))) && is_one_at_this_index(other_players_Ff, yx_zu_index(vy, vx+1))) {
                uint64_t child=shift_bit(pawn_bitboard, vy, vx, vy+1, vx+1);
                uint64_t new_o_p_p_en=clear_bit(other_players_Ff, yx_zu_index(vy, vx+1));
                childrenBb.push_back({child, this_players_Ff, new_o_p_p_en});
            }
        }
        //
        else {
            //2 nach vorne
            if (vy==6 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy-1, vx))) && !(is_one_at_this_index(all_pieces, yx_zu_index(vy-2, vx)))) {
                uint64_t child=clear_bit(pawn_bitboard, yx_zu_index(vy,vx));
                uint64_t child_Ff=set_bit_to_one(this_players_Ff, yx_zu_index(vy-2,vx));
                childrenBb.push_back({child, child_Ff, other_players_Ff});
            }
            //normal 1 nach vorne
            if (vy+1<8 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy-1, vx)))) {
                uint64_t child=shift_bit(pawn_bitboard, vy, vx, vy-1, vx);
                childrenBb.push_back({child, this_players_Ff, other_players_Ff});
            }
            //schlagen
            if (vy-1>-1 && vx-1>-1 && is_one_at_this_index(other_players_pieces, yx_zu_index(vy-1, vx-1))) {
                uint64_t child=shift_bit(pawn_bitboard, vy, vx, vy-1, vx-1);
                childrenBb.push_back({child, this_players_Ff, other_players_Ff});
            }
            if (vy-1>-1 && vx+1<8 && is_one_at_this_index(other_players_pieces, yx_zu_index(vy-1, vx+1))) {
                uint64_t child=shift_bit(pawn_bitboard, vy, vx, vy-1, vx+1);
                childrenBb.push_back({child, this_players_Ff, other_players_Ff});
            }
            //en passant
            if (vy-1>-1 && vx-1>-1 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy-1, vx-1))) && is_one_at_this_index(other_players_Ff, yx_zu_index(vy, vx-1))) {
                uint64_t child=shift_bit(pawn_bitboard, vy, vx, vy-1, vx-1);
                uint64_t new_o_p_p_en=clear_bit(other_players_Ff, yx_zu_index(vy, vx-1));
                childrenBb.push_back({child, this_players_Ff, new_o_p_p_en});
            }
            if (vy-1>.1 && vx+1<8 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy-1, vx+1))) && is_one_at_this_index(other_players_Ff, yx_zu_index(vy, vx+1))) {
                uint64_t child=shift_bit(pawn_bitboard, vy, vx, vy-1, vx+1);
                uint64_t new_o_p_p_en=clear_bit(other_players_Ff, yx_zu_index(vy, vx+1));
                childrenBb.push_back({child, this_players_Ff, new_o_p_p_en});
            }
        }
    }
    //
    return childrenBb; //return a vector with child_pawn_this_player Ff_this_player and new_o_p_p_en_other_player_of_this_child
}

//

std::vector<uint64_t> gorcLl(const uint64_t knight_bitboard, const uint64_t this_players_pieces) {
    std::vector<std::vector<int>> childrenLl;
    //
    int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
    int dy[] = {2, 1, -1, -2, -2, -1, 1, 2};
    
    //get knights current position as (x, y) coord.
    std::vector<std::pair<int,int>> knight_positions;
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (is_one_at_this_index(knight_bitboard, yx_zu_index(y, x))) {knight_positions.push_back({y, x});}
        }
    }
    if (knight_positions.empty()) {std::vector<uint64_t> empty_vector; return empty_vector;}
    //collect coordinates of moves
    for (const auto& position : knight_positions) {
        int y_current = position.first;
        int x_current = position.second;
        //
        for (int i=0; i<8; ++i) {
            int y_new = y_current + dy[i];
            int x_new = x_current + dx[i];
            //
            if (is_in_board(y_new, x_new)) {
                if (!(is_one_at_this_index(this_players_pieces, yx_zu_index(y_new, x_new)))) {
                    childrenLl.push_back({y_current, x_current, y_new, x_new});
                }
            }
        }
    }
    if (childrenLl.empty()) {std::vector<uint64_t> empty_vector; return empty_vector;}
    int chosen_index=generate_random_int(0, childrenLl.size()-1);
    std::vector<int> chosen_move=childrenLl[chosen_index];
    int vy=chosen_move[0];
    int vx=chosen_move[1];
    int zy=chosen_move[2];
    int zx=chosen_move[3];
    uint64_t child=shift_bit(knight_bitboard, vy, vx, zy, zx);
    std::vector<uint64_t> return_vector={child};
    return return_vector;
}

std::vector<uint64_t> gorcTtXxQq(const uint64_t bitboard, const uint64_t this_players_pieces, const uint64_t other_players_pieces, std::vector<std::pair<int,int>> directions) {
    std::vector<std::vector<int>> children_Tt_Xx_Qq;
    //get current positions as (x, y) coord.
    std::vector<std::pair<int,int>> positions;
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (is_one_at_this_index(bitboard, yx_zu_index(y, x))) {positions.push_back({y, x});}
        }
    }
    if (positions.empty()) {std::vector<uint64_t> empty_vector; return empty_vector;}
    //collect coorinates of moves
    for (const auto& position : positions) {
        int y_current = position.first;
        int x_current = position.second;
        //
        for (const auto& direction : directions) {
            int direction_y=direction.first;
            int direction_x=direction.second;
            //
            for (int i=1; i<8; ++i) {
                int y_new = y_current + i*direction_y;
                int x_new = x_current + i*direction_x;
                //
                if (is_in_board(y_new, x_new)) {
                    if (is_one_at_this_index(other_players_pieces, yx_zu_index(y_new, x_new))) {
                        children_Tt_Xx_Qq.push_back({y_current, x_current, y_new, x_new});
                        break;
                    }
                    else if (is_one_at_this_index(this_players_pieces, yx_zu_index(y_new, x_new))) {break;}
                    else {
                        children_Tt_Xx_Qq.push_back({y_current, x_current, y_new, x_new});
                    }
                }
                else {break;}
            }
        }
    }
    if (children_Tt_Xx_Qq.empty()) {std::vector<uint64_t> empty_vector; return empty_vector;}
    //
    int chosen_index=generate_random_int(0, children_Tt_Xx_Qq.size()-1);
    std::vector<int> chosen_move=children_Tt_Xx_Qq[chosen_index];
    int vy=chosen_move[0];
    int vx=chosen_move[1];
    int zy=chosen_move[2];
    int zx=chosen_move[3];
    uint64_t child=shift_bit(bitboard, vy, vx, zy, zx);
    std::vector<uint64_t> return_vector={child};
    return return_vector;
}

std::vector<std::pair<uint64_t, uint64_t>> gorcZz(const uint64_t Zz_bitboard, const uint64_t Tt_bitboard, const uint64_t this_players_pieces, const uint64_t other_players_pieces) {
    std::vector<std::pair<int,int>> directions={{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    std::vector<std::vector<int>> children_Tt_Zz;
    //position von Zz mit yx finden
    std::vector<std::pair<int,int>> positions;
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (is_one_at_this_index(Zz_bitboard, yx_zu_index(y, x))) {positions.push_back({y, x});}
        }
    }
    if (positions.empty()) {std::vector<std::pair<uint64_t, uint64_t>> empty_vector; return empty_vector;}
    //collect coordinates of moves
    for (const auto& position : positions) {
        int y_current = position.first;
        int x_current = position.second;
        //
        for (const auto& direction : directions) {
            int direction_y=direction.first;
            int direction_x=direction.second;
            //
            for (int i=1; i<8; ++i) {
                int y_new = y_current + i*direction_y;
                int x_new = x_current + i*direction_x;
                //
                if (is_in_board(y_new, x_new)) {
                    if (is_one_at_this_index(other_players_pieces, yx_zu_index(y_new, x_new))) {
                        children_Tt_Zz.push_back({y_current, x_current, y_new, x_new});
                        break;
                    }
                    else if (is_one_at_this_index(this_players_pieces, yx_zu_index(y_new, x_new))) {break;}
                    else {
                        children_Tt_Zz.push_back({y_current, x_current, y_new, x_new});
                    }
                }
                else {break;}
            }
        }
    }
    if (children_Tt_Zz.empty()) {std::vector<std::pair<uint64_t, uint64_t>> empty_vector; return empty_vector;}
    //
    int chosen_index=generate_random_int(0, children_Tt_Zz.size()-1);
    std::vector<int> chosen_move=children_Tt_Zz[chosen_index];
    int vy=chosen_move[0];
    int vx=chosen_move[1];
    int zy=chosen_move[2];
    int zx=chosen_move[3];
    uint64_t child_Tt=set_bit_to_one(Tt_bitboard, yx_zu_index(zy, zx));
    uint64_t child_Zz=clear_bit(Zz_bitboard, yx_zu_index(vy, vx));
    std::vector<std::pair<uint64_t, uint64_t>> return_vector={{child_Tt, child_Zz}};
    return return_vector;
    //return {child_Tt, child_Zz}
}

std::vector<std::vector<uint64_t>> gorcBb(int player, const uint64_t pawn_bitboard, const uint64_t this_players_pieces, const uint64_t this_players_Ff, const uint64_t other_players_pieces, const uint64_t other_players_Ff) {
    std::vector<std::vector<int>> childrenBb;
    //get pawns current positions as (x, y) coord.
    std::vector<std::pair<int,int>> pawn_positions;
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (is_one_at_this_index(pawn_bitboard, yx_zu_index(y, x))) {pawn_positions.push_back({y, x});}
        }
    }
    if (pawn_positions.empty()) {std::vector<std::vector<uint64_t>> empty_vector; return empty_vector;}
    //
    uint64_t all_pieces=this_players_pieces|other_players_pieces;
    //modify boards
    for (const auto& position : pawn_positions) {
        int vy = position.first;
        int vx = position.second;
        //
        if (player==1) {
            //2 nach vorne
            if (vy==1 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy+1, vx))) && !(is_one_at_this_index(all_pieces, yx_zu_index(vy+2, vx)))) {
                childrenBb.push_back({1, vy, vx});
            }
            //normal 1 nach vorne
            if (vy+1<8 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy+1, vx)))) {
                childrenBb.push_back({2, vy, vx});
            }
            //schlagen
            if (vy+1<8 && vx-1>-1 && is_one_at_this_index(other_players_pieces, yx_zu_index(vy+1, vx-1))) {
                childrenBb.push_back({3, vy, vx});
            }
            if (vy+1<8 && vx+1<8 && is_one_at_this_index(other_players_pieces, yx_zu_index(vy+1, vx+1))) {
                childrenBb.push_back({4, vy, vx});
            }
            //en passant
            if (vy+1<8 && vx-1>-1 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy+1, vx-1))) && is_one_at_this_index(other_players_Ff, yx_zu_index(vy, vx-1))) {
                childrenBb.push_back({5, vy, vx});
            }
            if (vy+1<8 && vx+1<8 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy+1, vx+1))) && is_one_at_this_index(other_players_Ff, yx_zu_index(vy, vx+1))) {
                childrenBb.push_back({6, vy, vx});
            }
        }
        //
        else {
            //2 nach vorne
            if (vy==6 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy-1, vx))) && !(is_one_at_this_index(all_pieces, yx_zu_index(vy-2, vx)))) {
                childrenBb.push_back({7, vy, vx});
            }
            //normal 1 nach vorne
            if (vy+1<8 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy-1, vx)))) {
                childrenBb.push_back({8, vy, vx});
            }
            //schlagen
            if (vy-1>-1 && vx-1>-1 && is_one_at_this_index(other_players_pieces, yx_zu_index(vy-1, vx-1))) {
                childrenBb.push_back({9, vy, vx});
            }
            if (vy-1>-1 && vx+1<8 && is_one_at_this_index(other_players_pieces, yx_zu_index(vy-1, vx+1))) {
                childrenBb.push_back({10, vy, vx});
            }
            //en passant
            if (vy-1>-1 && vx-1>-1 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy-1, vx-1))) && is_one_at_this_index(other_players_Ff, yx_zu_index(vy, vx-1))) {
                childrenBb.push_back({11, vy, vx});
            }
            if (vy-1>.1 && vx+1<8 && !(is_one_at_this_index(all_pieces, yx_zu_index(vy-1, vx+1))) && is_one_at_this_index(other_players_Ff, yx_zu_index(vy, vx+1))) {
                childrenBb.push_back({12, vy, vx});
            }
        }
    }
    if (childrenBb.empty()) {std::vector<std::vector<uint64_t>> empty_vector; return empty_vector;}
    //
    int chosen_index=generate_random_int(0, childrenBb.size()-1);
    std::vector<int> chosen_move=childrenBb[chosen_index];
    int type_of_move=chosen_move[0];
    int vy=chosen_move[1];
    int vx=chosen_move[2];
    uint64_t child;
    uint64_t child_Ff;
    uint64_t new_o_p_p_en;
    if (type_of_move==1) {
        child=clear_bit(pawn_bitboard, yx_zu_index(vy,vx));
        child_Ff=set_bit_to_one(this_players_Ff, yx_zu_index(vy+2,vx));
        new_o_p_p_en=other_players_Ff;
    }
    if (type_of_move==2) {
        child=shift_bit(pawn_bitboard, vy, vx, vy+1, vx);
        child_Ff=this_players_Ff;
        new_o_p_p_en=other_players_Ff;
    }
    if (type_of_move==3) {
        child=shift_bit(pawn_bitboard, vy, vx, vy+1, vx-1);
        child_Ff=this_players_Ff;
        new_o_p_p_en=other_players_Ff;
    }
    if (type_of_move==4) {
        child=shift_bit(pawn_bitboard, vy, vx, vy+1, vx+1);
        child_Ff=this_players_Ff;
        new_o_p_p_en=other_players_Ff;
    }
    if (type_of_move==5) {
        child=shift_bit(pawn_bitboard, vy, vx, vy+1, vx-1);
        child_Ff=this_players_Ff;
        new_o_p_p_en=clear_bit(other_players_Ff, yx_zu_index(vy, vx-1));
    }
    if (type_of_move==6) {
        child=shift_bit(pawn_bitboard, vy, vx, vy+1, vx+1);
        child_Ff=this_players_Ff;
        new_o_p_p_en=clear_bit(other_players_Ff, yx_zu_index(vy, vx+1));
    }
    //player2
    if (type_of_move==7) {
        child=clear_bit(pawn_bitboard, yx_zu_index(vy,vx));
        child_Ff=set_bit_to_one(this_players_Ff, yx_zu_index(vy-2,vx));
        new_o_p_p_en=other_players_Ff;
    }
    if (type_of_move==8) {
        child=shift_bit(pawn_bitboard, vy, vx, vy-1, vx);
        child_Ff=this_players_Ff;
        new_o_p_p_en=other_players_Ff;
    }
    if (type_of_move==9) {
        child=shift_bit(pawn_bitboard, vy, vx, vy-1, vx-1);
        child_Ff=this_players_Ff;
        new_o_p_p_en=other_players_Ff;
    }
    if (type_of_move==10) {
        child=shift_bit(pawn_bitboard, vy, vx, vy-1, vx+1);
        child_Ff=this_players_Ff;
        new_o_p_p_en=other_players_Ff;
    }
    if (type_of_move==11) {
        child=shift_bit(pawn_bitboard, vy, vx, vy-1, vx-1);
        child_Ff=this_players_Ff;
        new_o_p_p_en=clear_bit(other_players_Ff, yx_zu_index(vy, vx-1));
    }
    if (type_of_move==12) {
        child=shift_bit(pawn_bitboard, vy, vx, vy-1, vx+1);
        child_Ff=this_players_Ff;
        new_o_p_p_en=clear_bit(other_players_Ff, yx_zu_index(vy, vx+1));
    }
    std::vector<std::vector<uint64_t>> return_vector={{child, child_Ff, new_o_p_p_en}};
    return return_vector;
    //return a pair with child_pawn_this_player, child_Ff and new_o_p_p_en_other_player
}

std::vector<uint64_t> gorcKk(const uint64_t king_bitboard, const uint64_t this_players_pieces) {
    std::vector<std::vector<int>> childrenKk;
    //
    int dx[] = {0, 0, 1, -1, 1, -1, 1, -1};
    int dy[] = {1, -1, 0, 0, 1, 1, -1, -1};
    
    //get kings current position as (x, y) coord.
    std::vector<std::pair<int,int>> king_positions;
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            if (is_one_at_this_index(king_bitboard, yx_zu_index(y, x))) {king_positions.push_back({y, x});}
        }
    }
    if (king_positions.empty()) {std::vector<uint64_t> empty_vector; return empty_vector;}
    //collect coordinates of moves
    for (const auto& position : king_positions) {
        int y_current = position.first;
        int x_current = position.second;
        //
        for (int i=0; i<8; ++i) {
            int y_new = y_current + dy[i];
            int x_new = x_current + dx[i];
            //
            if (is_in_board(y_new, x_new)) {
                if (!(is_one_at_this_index(this_players_pieces, yx_zu_index(y_new, x_new)))) {
                    childrenKk.push_back({y_current, x_current, y_new, x_new});
                }
            }
        }
    }
    //
    if (childrenKk.empty()) {std::vector<uint64_t> empty_vector; return empty_vector;}
    int chosen_index=generate_random_int(0, childrenKk.size()-1);
    std::vector<int> chosen_move=childrenKk[chosen_index];
    int vy=chosen_move[0];
    int vx=chosen_move[1];
    int zy=chosen_move[2];
    int zx=chosen_move[3];
    uint64_t child=shift_bit(king_bitboard, vy, vx, zy, zx);
    std::vector<uint64_t> return_vector={child};
    return return_vector;
    //return childrenKk;
}

//

std::vector<std::vector<int>> invert_matrix(std::vector<std::vector<int>> matrix) {
    std::vector<std::vector<int>> inverted_matrix = matrix;
    std::reverse(inverted_matrix.begin(), inverted_matrix.end());
    return inverted_matrix;
}

std::vector<std::vector<int>> matrix_minus(std::vector<std::vector<int>> matrix) {
    for (int i=0; i<matrix.size(); ++i) {
        for (int j=0; j<matrix[i].size(); ++j) {
            matrix[i][j]=-matrix[i][j];
        }
    }
    return matrix;
}

std::vector<std::vector<int>> reverse_matrix(std::vector<std::vector<int>> matrix) {
    std::reverse(matrix.begin(), matrix.end());
    for (int i = 0; i < 8; i++) {std::reverse(matrix[i].begin(), matrix[i].end());}
    return matrix;
}


std::vector<std::vector<int>> b = {
    {  0,   0,   0,   0,   0,   0,   0,   0},
    { 50,  50,  50,  50,  50, 50,  50,  50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    { 0,  5, 10, 40, 40, 10,  5,  0},
    { 0,  0,  0, 60, 60,  0,  0,  0},
    { 0, 0, 10,  10,  10, 10, 0,  0},
    { 5, 10, 10,-20,-20, 10, 10,  5},
    { 0, 0,  0,  0,  0,  0,   0,  0}
};

std::vector<std::vector<int>> l = {
    {-50, -40, -30, -30, -30, -30, -40, -50},
    {-40, -20,   0,   0,    0,  0, -20, -40},
    {-30, 0, 20, 15, 15, 20, 0, -30},
    {-30, 5, 15, 20, 20, 15, 5, -30},
    {-30, 0, 15, 20, 20, 15, 0, -30},
    {-30, 5, 10, 15, 15, 10, 5, -30},
    {-40, -20, 0, 5, 5, 0, -20, -40},
    {-50, -40, -30, -30, -30, -30, -40, -50}
};

std::vector<std::vector<int>> x = {
    {-20, -10, -10, -10, -10, -10, -10, -20},
    {-10, 0, 0, 0, 0, 0, 0, -10},
    {-10, 0, 5, 10, 10, 5, 0, -10},
    {-10, 5, 5, 10, 10, 5, 5, -10},
    {-10, 0, 10, 10, 10, 10, 0, -10},
    {-10, 10, 10, 10, 10, 10, 10, -10},
    {-10, 5, 0, 0, 0, 0, 5, -10},
    {-20, -10, -10, -10, -10, -10, -10, -20}
};

std::vector<std::vector<int>> t = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {5, 20, 20, 20, 20, 20, 20, 5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {0, 0, 0, 10, 10, 0, 0, 0}
};

std::vector<std::vector<int>> q = {
    {-20, -10, -10, -5, -5, -10, -10, -20},
    {-10, 0, 0, 0, 0, 0, 0, -10},
    {-10, 0, 5, 5, 5, 5, 0, -10},
    {-5, 0, 5, 5, 5, 5, 0, -5},
    {0, 0, 5, 5, 5, 5, 0, -5},
    {-10, 5, 5, 5, 5, 5, 0, -10},
    {-10, 0, 5, 0, 0, 0, 0, -10},
    {-20, -10, -10, -5, -5, -10, -10, -20}
};

std::vector<std::vector<int>> k_normal = {
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-20, -30, -30, -40, -40, -30, -30, -20},
    {-10, -20, -20, -20, -20, -20, -20, -10},
    {20, 20, 0, 0, 0, 0, 20, 20},
    {20, 30, 10, 0, 0, 10, 30, 20}
};

std::vector<std::vector<int>> k_endgame = {
    {-50, -40, -30, -20, -20, -30, -40, -50},
    {-30, -20, -10, 0, 0, -10, -20, -30},
    {-30, -10, 20, 30, 30, 20, -10, -30},
    {-30, -10, 30, 40, 40, 30, -10, -30},
    {-30, -10, 30, 40, 40, 30, -10, -30},
    {-30, -10, 20, 30, 30, 20, -10, -30},
    {-30, -30, 0, 0, 0, 0, -30, -30},
    {-50, -30, -30, -30, -30, -30, -30, -50}
};

std::vector<std::vector<int>> b_matrix=reverse_matrix(b);
std::vector<std::vector<int>> l_matrix=reverse_matrix(l);
std::vector<std::vector<int>> x_matrix=reverse_matrix(x);
std::vector<std::vector<int>> t_matrix=reverse_matrix(t);
std::vector<std::vector<int>> q_matrix=reverse_matrix(q);
std::vector<std::vector<int>> k_normal_matrix=reverse_matrix(k_normal);
std::vector<std::vector<int>> k_endgame_matrix=reverse_matrix(k_endgame);

std::vector<std::vector<int>> B_matrix =invert_matrix(b_matrix);
std::vector<std::vector<int>> L_matrix =invert_matrix(l_matrix);
std::vector<std::vector<int>> X_matrix =invert_matrix(x_matrix);
std::vector<std::vector<int>> T_matrix =invert_matrix(t_matrix);
std::vector<std::vector<int>> Q_matrix =invert_matrix(q_matrix);
std::vector<std::vector<int>> K_normal_matrix =invert_matrix(k_normal_matrix);
std::vector<std::vector<int>> K_endgame_matrix =invert_matrix(k_endgame_matrix);

std::vector<std::vector<int>> other_b_matrix = matrix_minus(b_matrix);
std::vector<std::vector<int>> other_l_matrix = matrix_minus(l_matrix);
std::vector<std::vector<int>> other_x_matrix = matrix_minus(x_matrix);
std::vector<std::vector<int>> other_t_matrix = matrix_minus(t_matrix);
std::vector<std::vector<int>> other_q_matrix = matrix_minus(q_matrix);
std::vector<std::vector<int>> other_k_normal_matrix = matrix_minus(k_normal_matrix);
std::vector<std::vector<int>> other_k_endgame_matrix = matrix_minus(k_endgame_matrix);
std::vector<std::vector<int>> other_B_matrix = matrix_minus(B_matrix);
std::vector<std::vector<int>> other_L_matrix = matrix_minus(L_matrix);
std::vector<std::vector<int>> other_X_matrix = matrix_minus(X_matrix);
std::vector<std::vector<int>> other_T_matrix = matrix_minus(T_matrix);
std::vector<std::vector<int>> other_Q_matrix = matrix_minus(Q_matrix);
std::vector<std::vector<int>> other_K_normal_matrix = matrix_minus(K_normal_matrix);
std::vector<std::vector<int>> other_K_endgame_matrix = matrix_minus(K_endgame_matrix);

//

//forward declarartions
class Board;
std::vector<Board> generate_children(int playerk);

class Board {
public:

    bool schlagen;

    uint64_t b;
    uint64_t l;
    uint64_t x;
    uint64_t t;
    uint64_t q;
    uint64_t k;
    uint64_t y;
    uint64_t z;
    uint64_t f;
    
    uint64_t B;
    uint64_t L;
    uint64_t X;
    uint64_t T;
    uint64_t Q;
    uint64_t K;
    uint64_t Y;
    uint64_t Z;
    uint64_t F;

    Board() {}

    void print_board() const {
        std::vector zahlen= {8,7,6,5,4,3,2,1};
        std::vector<std::vector<std::string>> board;
        //initialize board with spaces
        for (int i = 0; i < 8; i++) {
            std::vector<std::string> empty_string_vector(8, " ");
            board.push_back(empty_string_vector);
        }
        //place symbols on board based on bitboards
        for (int i=0; i<8; i++) {
            for (int j=0; j<8; j++) {
                uint64_t mask = 1ULL << (i*8+j);
                if (this->b & mask) board[i][j] = 'b';
                if (this->B & mask) board[i][j] = 'B';
                if (this->f & mask) board[i][j] = 'f';
                if (this->F & mask) board[i][j] = 'F';
                if (this->l & mask) board[i][j] = 'l';
                if (this->L & mask) board[i][j] = 'L';
                if (this->x & mask) board[i][j] = 'x';
                if (this->X & mask) board[i][j] = 'X';
                if (this->t & mask) board[i][j] = 't';
                if (this->T & mask) board[i][j] = 'T';
                if (this->z & mask) board[i][j] = 'z';
                if (this->Z & mask) board[i][j] = 'Z';
                if (this->q & mask) board[i][j] = 'q';
                if (this->Q & mask) board[i][j] = 'Q';
                if (this->k & mask) board[i][j] = 'k';
                if (this->K & mask) board[i][j] = 'K';
                if (this->y & mask) board[i][j] = 'y';
                if (this->Y & mask) board[i][j] = 'Y';
                
            }
        }
        //
        std::reverse(board.begin(), board.end());
        for (int i=0; i<8; i++) {std::reverse(board[i].begin(), board[i].end());}
        //
        //print the board
        std::cout <<"    a   b   c   d   e   f   g   h\n";
        std::cout <<"  ---------------------------------\n";
        for (int i=0; i<8; i++) {
            std::cout << zahlen[i] <<" I";
            for (int j=0; j<8; j++) {
                std::cout << " ";
                std::cout << board[i][j];
                std::cout << " ";
                std::cout << "I";
            }
            std::cout <<'\n';
            std::cout <<"  ---------------------------------\n";
        }
    }

    bool verloren(int playerk) const {
        if (playerk==6) {
            if (this->k==0 && this->y==0) {return true;}
            else {return false;}
        }
        else {
            if (this->K==0 && this->Y==0) {return true;}
            else {return false;}
        }
    }

    int evaluate_position(int playerk) const {
        int val = 0;
        int y_this_king=0;
        int x_this_king=0;
        int y_other_king=0;
        int x_other_king=0;
        int anz_Qq=0;
        int anz_minor_pieces=0;
        if (playerk==6) {
            for (int p=0; p<8; ++p) {
                for (int o=0; o<8; ++o) {
                    if (is_one_at_this_index(this->B, yx_zu_index(p, o)) || is_one_at_this_index(this->F, yx_zu_index(p, o))) {
                        val += -1000;
                        val += other_B_matrix[p][o];
                        // Pawn structure
                        if (p+1<=7 && o+1<=7) {
                            if (is_one_at_this_index(this->B, yx_zu_index(p+1, o+1)) || is_one_at_this_index(this->F, yx_zu_index(p+1, o+1))) {val += -10;}
                        }
                        if (p-1>=0 && o-1>=0) {
                            if (is_one_at_this_index(this->B, yx_zu_index(p-1, o-1)) || is_one_at_this_index(this->F, yx_zu_index(p-1, o-1))) {val += -10;}
                        }
                        if (p+1<=7 && o-1>=0) {
                            if (is_one_at_this_index(this->B, yx_zu_index(p+1, o-1)) || is_one_at_this_index(this->F, yx_zu_index(p+1, o-1))) {val += -10;}
                        }
                        if (p-1>=0 && o+1<=7) {
                            if (is_one_at_this_index(this->B, yx_zu_index(p-1, o+1)) || is_one_at_this_index(this->F, yx_zu_index(p-1, o+1))) {val += -10;}
                        }
                    }//
                    else if (is_one_at_this_index(this->b, yx_zu_index(p, o)) || is_one_at_this_index(this->f, yx_zu_index(p, o))) {
                        val += 1000;
                        val += b_matrix[p][o];
                        // Pawn structure
                        if (p+1<=7 && o+1<=7) {
                            if (is_one_at_this_index(this->b, yx_zu_index(p+1, o+1)) || is_one_at_this_index(this->f, yx_zu_index(p+1, o+1))) {val += 10;}
                        }
                        if (p-1>=0 && o-1>=0) {
                            if (is_one_at_this_index(this->b, yx_zu_index(p-1, o-1)) || is_one_at_this_index(this->f, yx_zu_index(p-1, o-1))) {val += 10;}
                        }
                        if (p+1<=7 && o-1>=0) {
                            if (is_one_at_this_index(this->b, yx_zu_index(p+1, o-1)) || is_one_at_this_index(this->f, yx_zu_index(p+1, o-1))) {val += 10;}
                        }
                        if (p-1>=0 && o+1<=7) {
                            if (is_one_at_this_index(this->b, yx_zu_index(p-1, o+1)) || is_one_at_this_index(this->f, yx_zu_index(p-1, o+1))) {val += 10;}
                        }
                    }
                    else if (is_one_at_this_index(this->L, yx_zu_index(p, o))) {
                        val += -3000;
                        val += other_L_matrix[p][o];
                        anz_minor_pieces+=1;
                    }
                    else if (is_one_at_this_index(this->X, yx_zu_index(p, o))) {
                        val += -3000;
                        val += other_X_matrix[o][p];
                        anz_minor_pieces+=1;
                    }
                    else if (is_one_at_this_index(this->T, yx_zu_index(p, o)) || is_one_at_this_index(this->Z, yx_zu_index(p, o))) {
                        val += -5000;
                        val += other_T_matrix[p][o];
                    }
                    else if (is_one_at_this_index(this->l, yx_zu_index(p, o))) {
                        val += 3000;
                        val += l_matrix[p][o];
                        anz_minor_pieces+=1;
                    }
                    else if (is_one_at_this_index(this->x, yx_zu_index(p, o))) {
                        val += 3000;
                        val += x_matrix[o][p];
                        anz_minor_pieces+=1;
                    }
                    else if (is_one_at_this_index(this->t, yx_zu_index(p, o)) || is_one_at_this_index(this->z, yx_zu_index(p, o))) {
                        val += 5000;
                        val += t_matrix[p][o];
                    }
                    else if (is_one_at_this_index(this->Q, yx_zu_index(p, o))) {
                        val += -9000;
                        val += other_Q_matrix[p][o];
                        anz_Qq+=1;
                    }
                    else if (is_one_at_this_index(this->K, yx_zu_index(p, o)) || is_one_at_this_index(this->Y, yx_zu_index(p, o))) {
                        val += -10000000;
                        y_other_king=p;
                        x_other_king=o;
                    }
                    else if (is_one_at_this_index(this->q, yx_zu_index(p, o))) {
                        val += 9000;
                        val += q_matrix[p][o];
                        anz_Qq+=1;
                    }
                    else if (is_one_at_this_index(this->k, yx_zu_index(p, o)) || is_one_at_this_index(this->y, yx_zu_index(p, o))) {
                        val += 10000000;
                        y_this_king=p;
                        x_this_king=o;
                    }
                }
            }
            if (anz_Qq==0) {
                val+=other_K_endgame_matrix[y_other_king][x_other_king];
                val+=k_endgame_matrix[y_this_king][x_this_king];
            }
            else if (anz_Qq==1) {
                if (anz_minor_pieces<3) {
                    val+=other_K_endgame_matrix[y_other_king][x_other_king];
                    val+=k_endgame_matrix[y_this_king][x_this_king];
                }
            }
            else {
                val+=other_K_normal_matrix[y_other_king][x_other_king];
                val+=k_normal_matrix[y_this_king][x_this_king];
            }
        }
        else if (playerk==-6) {
            for (int p=0; p<8; ++p) {
                for (int o=0; o<8; ++o) {
                    if (is_one_at_this_index(this->B, yx_zu_index(p, o)) || is_one_at_this_index(this->F, yx_zu_index(p, o))) {
                        val += 1000;
                        val += B_matrix[p][o];
                        // Pawn structure
                        if (p+1<=7 && o+1<=7) {
                            if (is_one_at_this_index(this->B, yx_zu_index(p+1, o+1)) || is_one_at_this_index(this->F, yx_zu_index(p+1, o+1))) {val += 10;}
                        }
                        if (p-1>=0 && o-1>=0) {
                            if (is_one_at_this_index(this->B, yx_zu_index(p-1, o-1)) || is_one_at_this_index(this->F, yx_zu_index(p-1, o-1))) {val += 10;}
                        }
                        if (p+1<=7 && o-1>=0) {
                            if (is_one_at_this_index(this->B, yx_zu_index(p+1, o-1)) || is_one_at_this_index(this->F, yx_zu_index(p+1, o-1))) {val += 10;}
                        }
                        if (p-1>=0 && o+1<=7) {
                            if (is_one_at_this_index(this->B, yx_zu_index(p-1, o+1)) || is_one_at_this_index(this->F, yx_zu_index(p-1, o+1))) {val += 10;}
                        }
                    }
                    else if (is_one_at_this_index(this->b, yx_zu_index(p, o)) || is_one_at_this_index(this->f, yx_zu_index(p, o))) {
                        val += -1000;
                        val += other_b_matrix[p][o];
                        // Pawn structure
                        if (p+1<=7 && o+1<=7) {
                            if (is_one_at_this_index(this->b, yx_zu_index(p+1, o+1)) || is_one_at_this_index(this->f, yx_zu_index(p+1, o+1))) {val += -10;}
                        }
                        if (p-1>=0 && o-1>=0) {
                            if (is_one_at_this_index(this->b, yx_zu_index(p-1, o-1)) || is_one_at_this_index(this->f, yx_zu_index(p-1, o-1))) {val += -10;}
                        }
                        if (p+1<=7 && o-1>=0) {
                            if (is_one_at_this_index(this->b, yx_zu_index(p+1, o-1)) || is_one_at_this_index(this->f, yx_zu_index(p+1, o-1))) {val += -10;}
                        }
                        if (p-1>=0 && o+1<=7) {
                            if (is_one_at_this_index(this->b, yx_zu_index(p-1, o+1)) || is_one_at_this_index(this->f, yx_zu_index(p-1, o+1))) {val += -10;}
                        }
                    }
                    else if (is_one_at_this_index(this->L, yx_zu_index(p, o))) {
                        val += 3000;
                        val += L_matrix[p][o];
                        anz_minor_pieces+=1;
                    }
                    else if (is_one_at_this_index(this->X, yx_zu_index(p, o))) {
                        val += 3000;
                        val += X_matrix[p][o];
                        anz_minor_pieces+=1;
                    }
                    else if (is_one_at_this_index(this->T, yx_zu_index(p, o)) || is_one_at_this_index(this->Z, yx_zu_index(p, o))) {
                        val += 5000;
                        val += T_matrix[p][o];
                    }
                    else if (is_one_at_this_index(this->l, yx_zu_index(p, o))) {
                        val += -3000;
                        val += other_l_matrix[p][o];
                        anz_minor_pieces+=1;
                    }
                    else if (is_one_at_this_index(this->x, yx_zu_index(p, o))) {
                        val += -3000;
                        val += other_x_matrix[p][o];
                        anz_minor_pieces+=1;
                    }
                    else if (is_one_at_this_index(this->t, yx_zu_index(p, o)) || is_one_at_this_index(this->z, yx_zu_index(p, o))) {
                        val += -5000;
                        val += other_t_matrix[p][o];
                    }
                    else if (is_one_at_this_index(this->Q, yx_zu_index(p, o))) {
                        val += 9000;
                        val += Q_matrix[p][o];
                        anz_Qq+=1;
                    }
                    else if (is_one_at_this_index(this->K, yx_zu_index(p, o)) || is_one_at_this_index(this->Y, yx_zu_index(p, o))) {
                        val += 10000000;
                        y_this_king=p;
                        x_this_king=o;
                    }
                    else if (is_one_at_this_index(this->q, yx_zu_index(p, o))) {
                        val += -9000;
                        val += other_q_matrix[p][o];
                        anz_Qq+=1;
                    }
                    else if (is_one_at_this_index(this->k, yx_zu_index(p, o)) || is_one_at_this_index(this->y, yx_zu_index(p, o))) {
                        val += -10000000;
                        y_other_king=p;
                        x_other_king=o;
                    }
                }
            }
            if (anz_Qq==0) {
                val+=other_k_endgame_matrix[y_other_king][x_other_king];
                val+=K_endgame_matrix[y_this_king][x_this_king];
            }
            else if (anz_Qq==1) {
                if (anz_minor_pieces<3) {
                    val+=other_k_endgame_matrix[y_other_king][x_other_king];
                    val+=K_endgame_matrix[y_this_king][x_this_king];
                }
            }
            else {
                val+=other_k_normal_matrix[y_other_king][x_other_king];
                val+=K_normal_matrix[y_this_king][x_this_king];
            }
        }
        return val;
    }

    std::vector<Board> gcYy(int player, const uint64_t this_players_pieces, const uint64_t all_pieces) {
        std::vector<Board> children;
        //
        uint64_t Yy_bitboard;
        uint64_t Kk_bitboard;
        uint64_t Zz_bitboard;
        uint64_t Tt_bitboard;
        if (player==6) {
            Yy_bitboard=this->y;
            Kk_bitboard=this->k;
            Zz_bitboard=this->z;
            Tt_bitboard=this->t;
        }
        else {
            Yy_bitboard=this->Y;
            Kk_bitboard=this->K;
            Zz_bitboard=this->Z;
            Tt_bitboard=this->T;
        }
        //
        //get Yy current positions as (x, y) coord.
        std::vector<std::pair<int,int>> Yy_positions;
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (is_one_at_this_index(Yy_bitboard, yx_zu_index(y, x))) {Yy_positions.push_back({y, x});}
            }
        }
        //
        int dx[] = {0, 0, 1, -1, 1, -1, 1, -1};
        int dy[] = {1, -1, 0, 0, 1, 1, -1, -1};
        //
        //normal
        for (const auto& position : Yy_positions) {
            int y_current = position.first;
            int x_current = position.second;
            //
            for (int i=0; i<8; ++i) {
                int y_new = y_current + dy[i];
                int x_new = x_current + dx[i];
                //
                if (is_in_board(y_new, x_new)) {
                    if (!(is_one_at_this_index(this_players_pieces, yx_zu_index(y_new, x_new)))) {
                        uint64_t new_Kk=set_bit_to_one(Kk_bitboard, yx_zu_index(y_new, x_new));
                        uint64_t new_Yy=clear_bit(Yy_bitboard, yx_zu_index(y_current, x_current));
                        Board child;
                        if (player==6) {
                            child.b=this->b;
                            child.l=this->l;
                            child.x=this->x;
                            child.t=this->t;
                            child.q=this->q;
                            child.k=new_Kk;
                            child.y=new_Yy;
                            child.z=this->z;
                            child.f=this->f;
                            //
                            child.B=remove_common_bits(this->B, new_Kk);
                            child.L=remove_common_bits(this->L, new_Kk);
                            child.X=remove_common_bits(this->X, new_Kk);
                            child.T=remove_common_bits(this->T, new_Kk);
                            child.Q=remove_common_bits(this->Q, new_Kk);
                            child.K=remove_common_bits(this->K, new_Kk);
                            child.Y=remove_common_bits(this->Y, new_Kk);
                            child.Z=remove_common_bits(this->Z, new_Kk);
                            child.F=remove_common_bits(this->F, new_Kk);
                            //
                            child.schlagen=true;
                        }
                        else {
                            child.B=this->B;
                            child.L=this->L;
                            child.X=this->X;
                            child.T=this->T;
                            child.Q=this->Q;
                            child.K=new_Kk;
                            child.Y=new_Yy;
                            child.Z=this->Z;
                            child.F=this->F;
                            //
                            child.b=remove_common_bits(this->b, new_Kk);
                            child.l=remove_common_bits(this->l, new_Kk);
                            child.x=remove_common_bits(this->x, new_Kk);
                            child.t=remove_common_bits(this->t, new_Kk);
                            child.q=remove_common_bits(this->q, new_Kk);
                            child.k=remove_common_bits(this->k, new_Kk);
                            child.y=remove_common_bits(this->y, new_Kk);
                            child.z=remove_common_bits(this->z, new_Kk);
                            child.f=remove_common_bits(this->f, new_Kk);
                            //
                            child.schlagen=true;
                        }
                        //
                        children.push_back(child);
                    }
                }
            }
        }
        //
        //rochade
        if (player==6) {
            if (is_one_at_this_index(Yy_bitboard, 3) && is_one_at_this_index(Zz_bitboard, 0) && !(is_one_at_this_index(all_pieces, 1)) && !(is_one_at_this_index(all_pieces, 2))) {
                uint64_t new_Kk=set_bit_to_one(Kk_bitboard, 1);
                uint64_t new_Yy=clear_bit(Yy_bitboard, 3);
                uint64_t new_Tt=set_bit_to_one(Tt_bitboard, 2);
                uint64_t new_Zz=clear_bit(Zz_bitboard, 0);
                //
                Board simulation;
                simulation.b=this->b;
                simulation.l=this->l;
                simulation.x=this->x;
                simulation.t=new_Tt;
                simulation.q=this->q;
                simulation.k=new_Kk;
                simulation.y=new_Yy;
                simulation.z=new_Zz;
                simulation.f=this->f;
                //
                simulation.B=remove_common_bits(this->B, new_Kk);
                simulation.L=remove_common_bits(this->L, new_Kk);
                simulation.X=remove_common_bits(this->X, new_Kk);
                simulation.T=remove_common_bits(this->T, new_Kk);
                simulation.Q=remove_common_bits(this->Q, new_Kk);
                simulation.K=remove_common_bits(this->K, new_Kk);
                simulation.Y=remove_common_bits(this->Y, new_Kk);
                simulation.Z=remove_common_bits(this->Z, new_Kk);
                simulation.F=remove_common_bits(this->F, new_Kk);
                //
                simulation.schlagen=true;
                //
                bool legal=true;
                for (Board child : simulation.generate_children(-6)) {
                    uint64_t other_players_pieces= child.B|child.L|child.X|child.T|child.Q|child.K|child.Y|child.Z|child.F;
                    if (is_one_at_this_index(other_players_pieces, 1) || is_one_at_this_index(other_players_pieces, 2) || is_one_at_this_index(other_players_pieces, 3)) {
                        legal=false;
                        break;
                    }
                }
                if (legal) {children.push_back(simulation);}
            }
            //
            if (is_one_at_this_index(Yy_bitboard, 3) && is_one_at_this_index(Zz_bitboard, 7) && !(is_one_at_this_index(all_pieces, 6)) && !(is_one_at_this_index(all_pieces, 5)) && !(is_one_at_this_index(all_pieces, 4))) {
                uint64_t new_Kk=set_bit_to_one(Kk_bitboard, 5);
                uint64_t new_Yy=clear_bit(Yy_bitboard, 3);
                uint64_t new_Tt=set_bit_to_one(Tt_bitboard, 4);
                uint64_t new_Zz=clear_bit(Zz_bitboard, 7);
                //
                Board simulation;
                simulation.b=this->b;
                simulation.l=this->l;
                simulation.x=this->x;
                simulation.t=new_Tt;
                simulation.q=this->q;
                simulation.k=new_Kk;
                simulation.y=new_Yy;
                simulation.z=new_Zz;
                simulation.f=this->f;
                //
                simulation.B=remove_common_bits(this->B, new_Kk);
                simulation.L=remove_common_bits(this->L, new_Kk);
                simulation.X=remove_common_bits(this->X, new_Kk);
                simulation.T=remove_common_bits(this->T, new_Kk);
                simulation.Q=remove_common_bits(this->Q, new_Kk);
                simulation.K=remove_common_bits(this->K, new_Kk);
                simulation.Y=remove_common_bits(this->Y, new_Kk);
                simulation.Z=remove_common_bits(this->Z, new_Kk);
                simulation.F=remove_common_bits(this->F, new_Kk);
                //
                simulation.schlagen=true;
                //
                bool legal=true;
                for (Board child : simulation.generate_children(-6)) {
                    uint64_t other_players_pieces= child.B|child.L|child.X|child.T|child.Q|child.K|child.Y|child.Z|child.F;
                    if (is_one_at_this_index(other_players_pieces, 3) || is_one_at_this_index(other_players_pieces, 4) || is_one_at_this_index(other_players_pieces, 5)) {
                        legal=false;
                        break;
                    }
                }
                if (legal) {children.push_back(simulation);}
            }
        }
        else {
            if (is_one_at_this_index(Yy_bitboard, 59) && is_one_at_this_index(Zz_bitboard, 56) && !(is_one_at_this_index(all_pieces, 57)) && !(is_one_at_this_index(all_pieces, 58))) {
                uint64_t new_Kk=set_bit_to_one(Kk_bitboard, 57);
                uint64_t new_Yy=clear_bit(Yy_bitboard, 59);
                uint64_t new_Tt=set_bit_to_one(Tt_bitboard, 58);
                uint64_t new_Zz=clear_bit(Zz_bitboard, 56);
                //
                Board simulation;
                simulation.B=this->B;
                simulation.L=this->L;
                simulation.X=this->X;
                simulation.T=new_Tt;
                simulation.Q=this->Q;
                simulation.K=new_Kk;
                simulation.Y=new_Yy;
                simulation.Z=new_Zz;
                simulation.F=this->F;
                //
                simulation.b=remove_common_bits(this->b, new_Kk);
                simulation.l=remove_common_bits(this->l, new_Kk);
                simulation.x=remove_common_bits(this->x, new_Kk);
                simulation.t=remove_common_bits(this->t, new_Kk);
                simulation.q=remove_common_bits(this->q, new_Kk);
                simulation.k=remove_common_bits(this->k, new_Kk);
                simulation.y=remove_common_bits(this->y, new_Kk);
                simulation.z=remove_common_bits(this->z, new_Kk);
                simulation.f=remove_common_bits(this->f, new_Kk);
                //
                simulation.schlagen=true;
                //
                bool legal=true;
                for (Board child : simulation.generate_children(6)) {
                    uint64_t other_players_pieces= child.b|child.l|child.x|child.t|child.q|child.k|child.y|child.z|child.f;
                    if (is_one_at_this_index(other_players_pieces, 59) || is_one_at_this_index(other_players_pieces, 58) || is_one_at_this_index(other_players_pieces, 57)) {
                        legal=false;
                        break;
                    }
                }
                if (legal) {children.push_back(simulation);}
            }
            //
            if (is_one_at_this_index(Yy_bitboard, 59) && is_one_at_this_index(Zz_bitboard, 63) && !(is_one_at_this_index(all_pieces, 62)) && !(is_one_at_this_index(all_pieces, 61)) && !(is_one_at_this_index(all_pieces, 60))) {
                uint64_t new_Kk=set_bit_to_one(Kk_bitboard, 61);
                uint64_t new_Yy=clear_bit(Yy_bitboard, 59);
                uint64_t new_Tt=set_bit_to_one(Tt_bitboard, 60);
                uint64_t new_Zz=clear_bit(Zz_bitboard, 63);
                //
                Board simulation;
                simulation.B=this->B;
                simulation.L=this->l;
                simulation.X=this->X;
                simulation.T=new_Tt;
                simulation.Q=this->Q;
                simulation.K=new_Kk;
                simulation.Y=new_Yy;
                simulation.Z=new_Zz;
                simulation.F=this->F;
                //
                simulation.b=remove_common_bits(this->b, new_Kk);
                simulation.l=remove_common_bits(this->l, new_Kk);
                simulation.x=remove_common_bits(this->x, new_Kk);
                simulation.t=remove_common_bits(this->t, new_Kk);
                simulation.q=remove_common_bits(this->q, new_Kk);
                simulation.k=remove_common_bits(this->k, new_Kk);
                simulation.y=remove_common_bits(this->y, new_Kk);
                simulation.z=remove_common_bits(this->z, new_Kk);
                simulation.f=remove_common_bits(this->f, new_Kk);
                //
                simulation.schlagen=true;
                //
                bool legal=true;
                for (Board child : simulation.generate_children(6)) {
                    uint64_t other_players_pieces= child.b|child.l|child.x|child.t|child.q|child.k|child.y|child.z|child.f;
                    if (is_one_at_this_index(other_players_pieces, 59) || is_one_at_this_index(other_players_pieces, 60) || is_one_at_this_index(other_players_pieces, 61)) {
                        legal=false;
                        break;
                    }
                }
                if (legal) {children.push_back(simulation);}
            }
        }
        //
        return children;
    }

    std::vector<Board> generate_children(int playerk) {
        std::vector<Board> children;
        //
        //Ff zu Bb
        std::vector<std::pair<int,int>> positions_Ff;
        if (playerk==6) {
            for (int y=0; y<8; ++y) {
                for (int x=0; x<8; ++x) {
                    if (is_one_at_this_index(this->f, yx_zu_index(y, x))) {positions_Ff.push_back({y, x});}
                }
            }
            for (const auto& position_Ff : positions_Ff) {
                int y=position_Ff.first;
                int x=position_Ff.second;
                uint64_t new_f=clear_bit(this->f, yx_zu_index(y, x));
                uint64_t new_b=set_bit_to_one(this->b, yx_zu_index(y, x));
                this->f=new_f;
                this->b=new_b;
            }
        }
        else {
            for (int y=0; y<8; ++y) {
                for (int x=0; x<8; ++x) {
                    if (is_one_at_this_index(this->F, yx_zu_index(y, x))) {positions_Ff.push_back({y, x});}
                }
            }
            for (const auto& position_Ff : positions_Ff) {
                int y=position_Ff.first;
                int x=position_Ff.second;
                uint64_t new_F=clear_bit(this->F, yx_zu_index(y, x));
                uint64_t new_B=set_bit_to_one(this->B, yx_zu_index(y, x));
                this->F=new_F;
                this->B=new_B;
            }
        }
        //
        uint64_t white_pieces = this->b|this->l|this->x|this->t|this->q|this->k|this->y|this->z|this->f;
        uint64_t black_pieces = this->B|this->L|this->X|this->T|this->Q|this->K|this->Y|this->Z|this->F;
        uint64_t all_pieces = white_pieces|black_pieces;
        //
        if (playerk==6) {
            if (this->b!=0) {
                for (const auto& board : gcBb(1, this->b, white_pieces, this->f, black_pieces, this->F)) {
                    Board child;
                    child.b=board[0];
                    child.l=this->l;
                    child.x=this->x;
                    child.t=this->t;
                    child.q=this->q;
                    child.k=this->k;
                    child.y=this->y;
                    child.z=this->z;
                    child.f=board[1];
                    //
                    if (board[1]==0) {
                        child.B=remove_common_bits(this->B, board[0]);
                        child.L=remove_common_bits(this->L, board[0]);
                        child.X=remove_common_bits(this->X, board[0]);
                        child.T=remove_common_bits(this->T, board[0]);
                        child.Q=remove_common_bits(this->Q, board[0]);
                        child.K=remove_common_bits(this->K, board[0]);
                        child.Y=remove_common_bits(this->Y, board[0]);
                        child.Z=remove_common_bits(this->Z, board[0]);
                        child.F=remove_common_bits(board[2], board[0]);
                    }
                    else {
                        child.B=this->B;
                        child.L=this->L;
                        child.X=this->X;
                        child.T=this->T;
                        child.Q=this->Q;
                        child.K=this->K;
                        child.Y=this->Y;
                        child.Z=this->Z;
                        child.F=this->F;
                    }
                    //
                    uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                    if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->l!=0) {
                for (const auto& board : gcLl(this->l, white_pieces)) {
                    Board child;
                    child.b=this->b;
                    child.l=board;
                    child.x=this->x;
                    child.t=this->t;
                    child.q=this->q;
                    child.k=this->k;
                    child.y=this->y;
                    child.z=this->z;
                    child.f=this->f;
                    //
                    child.B=remove_common_bits(this->B, board);
                    child.L=remove_common_bits(this->L, board);
                    child.X=remove_common_bits(this->X, board);
                    child.T=remove_common_bits(this->T, board);
                    child.Q=remove_common_bits(this->Q, board);
                    child.K=remove_common_bits(this->K, board);
                    child.Y=remove_common_bits(this->Y, board);
                    child.Z=remove_common_bits(this->Z, board);
                    child.F=remove_common_bits(this->F, board);
                    //
                    uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                    if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->t!=0) {
                for (const auto& board : gcTtXxQq(this->t, white_pieces, black_pieces, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}})) {
                    Board child;
                    child.b=this->b;
                    child.l=this->l;
                    child.x=this->x;
                    child.t=board;
                    child.q=this->q;
                    child.k=this->k;
                    child.y=this->y;
                    child.z=this->z;
                    child.f=this->f;
                    //
                    child.B=remove_common_bits(this->B, board);
                    child.L=remove_common_bits(this->L, board);
                    child.X=remove_common_bits(this->X, board);
                    child.T=remove_common_bits(this->T, board);
                    child.Q=remove_common_bits(this->Q, board);
                    child.K=remove_common_bits(this->K, board);
                    child.Y=remove_common_bits(this->Y, board);
                    child.Z=remove_common_bits(this->Z, board);
                    child.F=remove_common_bits(this->F, board);
                    //
                    uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                    if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->x!=0) {
                for (const auto& board : gcTtXxQq(this->x, white_pieces, black_pieces, {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}})) {
                    Board child;
                    child.b=this->b;
                    child.l=this->l;
                    child.x=board;
                    child.t=this->t;
                    child.q=this->q;
                    child.k=this->k;
                    child.y=this->y;
                    child.z=this->z;
                    child.f=this->f;
                    //
                    child.B=remove_common_bits(this->B, board);
                    child.L=remove_common_bits(this->L, board);
                    child.X=remove_common_bits(this->X, board);
                    child.T=remove_common_bits(this->T, board);
                    child.Q=remove_common_bits(this->Q, board);
                    child.K=remove_common_bits(this->K, board);
                    child.Y=remove_common_bits(this->Y, board);
                    child.Z=remove_common_bits(this->Z, board);
                    child.F=remove_common_bits(this->F, board);
                    //
                    uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                    if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->q!=0) {
                for (const auto& board : gcTtXxQq(this->q, white_pieces, black_pieces, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}})) {
                    Board child;
                    child.b=this->b;
                    child.l=this->l;
                    child.x=this->x;
                    child.t=this->t;
                    child.q=board;
                    child.k=this->k;
                    child.y=this->y;
                    child.z=this->z;
                    child.f=this->f;
                    //
                    child.B=remove_common_bits(this->B, board);
                    child.L=remove_common_bits(this->L, board);
                    child.X=remove_common_bits(this->X, board);
                    child.T=remove_common_bits(this->T, board);
                    child.Q=remove_common_bits(this->Q, board);
                    child.K=remove_common_bits(this->K, board);
                    child.Y=remove_common_bits(this->Y, board);
                    child.Z=remove_common_bits(this->Z, board);
                    child.F=remove_common_bits(this->F, board);
                    //
                    uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                    if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->z!=0) {
                for (const auto& board : gcZz(this->z, this->t, white_pieces, black_pieces)) {
                    Board child;
                    child.b=this->b;
                    child.l=this->l;
                    child.x=this->x;
                    child.t=board.first;
                    child.q=this->q;
                    child.k=this->k;
                    child.y=this->y;
                    child.z=board.second;
                    child.f=this->f;
                    //
                    child.B=remove_common_bits(this->B, board.first);
                    child.L=remove_common_bits(this->L, board.first);
                    child.X=remove_common_bits(this->X, board.first);
                    child.T=remove_common_bits(this->T, board.first);
                    child.Q=remove_common_bits(this->Q, board.first);
                    child.K=remove_common_bits(this->K, board.first);
                    child.Y=remove_common_bits(this->Y, board.first);
                    child.Z=remove_common_bits(this->Z, board.first);
                    child.F=remove_common_bits(this->F, board.first);
                    //
                    uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                    if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->k!=0) {
                for (const auto& board : gcKk(this->k, white_pieces)) {
                    Board child;
                    child.b=this->b;
                    child.l=this->l;
                    child.x=this->x;
                    child.t=this->t;
                    child.q=this->q;
                    child.k=board;
                    child.y=this->y;
                    child.z=this->z;
                    child.f=this->f;
                    //
                    child.B=remove_common_bits(this->B, board);
                    child.L=remove_common_bits(this->L, board);
                    child.X=remove_common_bits(this->X, board);
                    child.T=remove_common_bits(this->T, board);
                    child.Q=remove_common_bits(this->Q, board);
                    child.K=remove_common_bits(this->K, board);
                    child.Y=remove_common_bits(this->Y, board);
                    child.Z=remove_common_bits(this->Z, board);
                    child.F=remove_common_bits(this->F, board);
                    //
                    uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                    if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->y!=0) {
                for (Board child_Yy : gcYy(6, white_pieces, all_pieces)) {children.push_back(child_Yy);}
            }
        }
        else {
            if (this->B!=0) {
                for (const auto& board : gcBb(-1, this->B, black_pieces, this->F, white_pieces, this->f)) {
                    Board child;
                    child.B=board[0];
                    child.L=this->L;
                    child.X=this->X;
                    child.T=this->T;
                    child.Q=this->Q;
                    child.K=this->K;
                    child.Y=this->Y;
                    child.Z=this->Z;
                    child.F=board[1];
                    //
                    if (board[1]==0) {
                        child.b=remove_common_bits(this->b, board[0]);
                        child.l=remove_common_bits(this->l, board[0]);
                        child.x=remove_common_bits(this->x, board[0]);
                        child.t=remove_common_bits(this->t, board[0]);
                        child.q=remove_common_bits(this->q, board[0]);
                        child.k=remove_common_bits(this->k, board[0]);
                        child.y=remove_common_bits(this->y, board[0]);
                        child.z=remove_common_bits(this->z, board[0]);
                        child.f=remove_common_bits(board[2], board[0]);
                    }
                    else {
                        child.b=this->b;
                        child.l=this->l;
                        child.x=this->x;
                        child.t=this->t;
                        child.q=this->q;
                        child.k=this->k;
                        child.y=this->y;
                        child.z=this->z;
                        child.f=this->f;
                    }
                    //
                    uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                    if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->L!=0) {
                for (const auto& board : gcLl(this->L, black_pieces)) {
                    Board child;
                    child.B=this->B;
                    child.L=board;
                    child.X=this->X;
                    child.T=this->T;
                    child.Q=this->Q;
                    child.K=this->K;
                    child.Y=this->Y;
                    child.Z=this->Z;
                    child.F=this->F;
                    //
                    child.b=remove_common_bits(this->b, board);
                    child.l=remove_common_bits(this->l, board);
                    child.x=remove_common_bits(this->x, board);
                    child.t=remove_common_bits(this->t, board);
                    child.q=remove_common_bits(this->q, board);
                    child.k=remove_common_bits(this->k, board);
                    child.y=remove_common_bits(this->y, board);
                    child.z=remove_common_bits(this->z, board);
                    child.f=remove_common_bits(this->f, board);
                    //
                    uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                    if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->T!=0) {
                for (const auto& board : gcTtXxQq(this->T, black_pieces, white_pieces, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}})) {
                    Board child;
                    child.B=this->B;
                    child.L=this->L;
                    child.X=this->X;
                    child.T=board;
                    child.Q=this->Q;
                    child.K=this->K;
                    child.Y=this->Y;
                    child.Z=this->Z;
                    child.F=this->F;
                    //
                    child.b=remove_common_bits(this->b, board);
                    child.l=remove_common_bits(this->l, board);
                    child.x=remove_common_bits(this->x, board);
                    child.t=remove_common_bits(this->t, board);
                    child.q=remove_common_bits(this->q, board);
                    child.k=remove_common_bits(this->k, board);
                    child.y=remove_common_bits(this->y, board);
                    child.z=remove_common_bits(this->z, board);
                    child.f=remove_common_bits(this->f, board);
                    //
                    uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                    if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->X!=0) {
                for (const auto& board : gcTtXxQq(this->X, black_pieces, white_pieces, {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}})) {
                    Board child;
                    child.B=this->B;
                    child.L=this->L;
                    child.X=board;
                    child.T=this->T;
                    child.Q=this->Q;
                    child.K=this->K;
                    child.Y=this->Y;
                    child.Z=this->Z;
                    child.F=this->F;
                    //
                    child.b=remove_common_bits(this->b, board);
                    child.l=remove_common_bits(this->l, board);
                    child.x=remove_common_bits(this->x, board);
                    child.t=remove_common_bits(this->t, board);
                    child.q=remove_common_bits(this->q, board);
                    child.k=remove_common_bits(this->k, board);
                    child.y=remove_common_bits(this->y, board);
                    child.z=remove_common_bits(this->z, board);
                    child.f=remove_common_bits(this->f, board);
                    //
                    uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                    if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->Q!=0) {
                for (const auto& board : gcTtXxQq(this->Q, black_pieces, white_pieces, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}})) {
                    Board child;
                    child.B=this->B;
                    child.L=this->L;
                    child.X=this->X;
                    child.T=this->T;
                    child.Q=board;
                    child.K=this->K;
                    child.Y=this->Y;
                    child.Z=this->Z;
                    child.F=this->F;
                    //
                    child.b=remove_common_bits(this->b, board);
                    child.l=remove_common_bits(this->l, board);
                    child.x=remove_common_bits(this->x, board);
                    child.t=remove_common_bits(this->t, board);
                    child.q=remove_common_bits(this->q, board);
                    child.k=remove_common_bits(this->k, board);
                    child.y=remove_common_bits(this->y, board);
                    child.z=remove_common_bits(this->z, board);
                    child.f=remove_common_bits(this->f, board);
                    //
                    uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                    if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->Z!=0) {
                for (const auto& board : gcZz(this->Z, this->T, black_pieces, white_pieces)) {
                    Board child;
                    child.B=this->B;
                    child.L=this->L;
                    child.X=this->X;
                    child.T=board.first;
                    child.Q=this->Q;
                    child.K=this->K;
                    child.Y=this->Y;
                    child.Z=board.second;
                    child.F=this->F;
                    //
                    child.b=remove_common_bits(this->b, board.first);
                    child.l=remove_common_bits(this->l, board.first);
                    child.x=remove_common_bits(this->x, board.first);
                    child.t=remove_common_bits(this->t, board.first);
                    child.q=remove_common_bits(this->q, board.first);
                    child.k=remove_common_bits(this->k, board.first);
                    child.y=remove_common_bits(this->y, board.first);
                    child.z=remove_common_bits(this->z, board.first);
                    child.f=remove_common_bits(this->f, board.first);
                    //
                    uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                    if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->K!=0) {
                for (const auto& board : gcKk(this->K, black_pieces)) {
                    Board child;
                    child.B=this->B;
                    child.L=this->L;
                    child.X=this->X;
                    child.T=this->T;
                    child.Q=this->Q;
                    child.K=board;
                    child.Y=this->Y;
                    child.Z=this->Z;
                    child.F=this->F;
                    //
                    child.b=remove_common_bits(this->b, board);
                    child.l=remove_common_bits(this->l, board);
                    child.x=remove_common_bits(this->x, board);
                    child.t=remove_common_bits(this->t, board);
                    child.q=remove_common_bits(this->q, board);
                    child.k=remove_common_bits(this->k, board);
                    child.y=remove_common_bits(this->y, board);
                    child.z=remove_common_bits(this->z, board);
                    child.f=remove_common_bits(this->f, board);
                    //
                    uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                    if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                    else {child.schlagen=false;}
                    //
                    children.push_back(child);
                }
            }
            //
            if (this->Y!=0) {
                for (Board child_Yy : gcYy(-6, black_pieces, all_pieces)) {children.push_back(child_Yy);}
            }
        }
        return children;
    }

    std::vector<Board> generate_one_random_child(int playerk) {
        std::vector<Board> children; //will always contain 1 child
        //Ff zu Bb
        std::vector<std::pair<int,int>> positions_Ff;
        if (playerk==6) {
            for (int y=0; y<8; ++y) {
                for (int x=0; x<8; ++x) {
                    if (is_one_at_this_index(this->f, yx_zu_index(y, x))) {positions_Ff.push_back({y, x});}
                }
            }
            for (const auto& position_Ff : positions_Ff) {
                int y=position_Ff.first;
                int x=position_Ff.second;
                uint64_t new_f=clear_bit(this->f, yx_zu_index(y, x));
                uint64_t new_b=set_bit_to_one(this->b, yx_zu_index(y, x));
                this->f=new_f;
                this->b=new_b;
            }
        }
        else {
            for (int y=0; y<8; ++y) {
                for (int x=0; x<8; ++x) {
                    if (is_one_at_this_index(this->F, yx_zu_index(y, x))) {positions_Ff.push_back({y, x});}
                }
            }
            for (const auto& position_Ff : positions_Ff) {
                int y=position_Ff.first;
                int x=position_Ff.second;
                uint64_t new_F=clear_bit(this->F, yx_zu_index(y, x));
                uint64_t new_B=set_bit_to_one(this->B, yx_zu_index(y, x));
                this->F=new_F;
                this->B=new_B;
            }
        }
        //
        uint64_t white_pieces = this->b|this->l|this->x|this->t|this->q|this->k|this->y|this->z|this->f;
        uint64_t black_pieces = this->B|this->L|this->X|this->T|this->Q|this->K|this->Y|this->Z|this->F;
        uint64_t all_pieces = white_pieces|black_pieces;
        //
        int anz_Bb=0;
        int anz_Ll=0;
        int anz_Xx=0;
        int anz_Tt=0;
        int anz_Qq=0;
        int anz_Kk=0; 
        int anz_Zz=0;
        int anz_Yy=0;
        int Bb_c=1;
        int Ll_c=2;
        int Xx_c=2;
        int Tt_c=2;
        int Qq_c=3;
        int Kk_c=1;
        int Zz_c=2;
        int Yy_c=1;
        if (playerk==6) {
            anz_Bb=count_bits_in_bitboard(this->b);
            anz_Ll=count_bits_in_bitboard(this->l);
            anz_Xx=count_bits_in_bitboard(this->x);
            anz_Tt=count_bits_in_bitboard(this->t);
            anz_Qq=count_bits_in_bitboard(this->q);
            anz_Kk=count_bits_in_bitboard(this->k);
            anz_Zz=count_bits_in_bitboard(this->z);
            anz_Yy=count_bits_in_bitboard(this->y);
        }
        else {
            anz_Bb=count_bits_in_bitboard(this->B);
            anz_Ll=count_bits_in_bitboard(this->L);
            anz_Xx=count_bits_in_bitboard(this->X);
            anz_Tt=count_bits_in_bitboard(this->T);
            anz_Qq=count_bits_in_bitboard(this->Q);
            anz_Kk=count_bits_in_bitboard(this->K);
            anz_Zz=count_bits_in_bitboard(this->Z);
            anz_Yy=count_bits_in_bitboard(this->Y);
        }
        std::vector<int> pieces;
        for (int i=0; i<anz_Bb; ++i) {for (int j=0; j<Bb_c; ++j) {pieces.push_back(1);}}
        for (int i=0; i<anz_Ll; ++i) {for (int j=0; j<Ll_c; ++j) {pieces.push_back(2);}}
        for (int i=0; i<anz_Xx; ++i) {for (int j=0; j<Xx_c; ++j) {pieces.push_back(3);}}
        for (int i=0; i<anz_Tt; ++i) {for (int j=0; j<Tt_c; ++j) {pieces.push_back(4);}}
        for (int i=0; i<anz_Qq; ++i) {for (int j=0; j<Qq_c; ++j) {pieces.push_back(5);}}
        for (int i=0; i<anz_Kk; ++i) {for (int j=0; j<Kk_c; ++j) {pieces.push_back(6);}}
        for (int i=0; i<anz_Zz; ++i) {for (int j=0; j<Zz_c; ++j) {pieces.push_back(7);}}
        for (int i=0; i<anz_Yy; ++i) {for (int j=0; j<Yy_c; ++j) {pieces.push_back(8);}}
        //
        for (int anzahl_max_versuche=0; anzahl_max_versuche<100; ++anzahl_max_versuche) {
            int chosen_index=generate_random_int(0, pieces.size()-1);
            int chosen_piece=pieces[chosen_index];
            //
            if (playerk==6) {
                if (chosen_piece==1) {
                    for (const auto& board : gorcBb(1, this->b, white_pieces, this->f, black_pieces, this->F)) {
                        Board child;
                        child.b=board[0];
                        child.l=this->l;
                        child.x=this->x;
                        child.t=this->t;
                        child.q=this->q;
                        child.k=this->k;
                        child.y=this->y;
                        child.z=this->z;
                        child.f=board[1];
                        //
                        if (board[1]==0) {
                            child.B=remove_common_bits(this->B, board[0]);
                            child.L=remove_common_bits(this->L, board[0]);
                            child.X=remove_common_bits(this->X, board[0]);
                            child.T=remove_common_bits(this->T, board[0]);
                            child.Q=remove_common_bits(this->Q, board[0]);
                            child.K=remove_common_bits(this->K, board[0]);
                            child.Y=remove_common_bits(this->Y, board[0]);
                            child.Z=remove_common_bits(this->Z, board[0]);
                            child.F=remove_common_bits(board[2], board[0]);
                        }
                        else { //Zug war 2 nach vorne, kein Schlagen
                            child.B=this->B;
                            child.L=this->L;
                            child.X=this->X;
                            child.T=this->T;
                            child.Q=this->Q;
                            child.K=this->K;
                            child.Y=this->Y;
                            child.Z=this->Z;
                            child.F=this->F;
                        }
                        //
                        uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                        if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==2) {
                    for (const auto& board : gorcLl(this->l, white_pieces)) {
                        Board child;
                        child.b=this->b;
                        child.l=board;
                        child.x=this->x;
                        child.t=this->t;
                        child.q=this->q;
                        child.k=this->k;
                        child.y=this->y;
                        child.z=this->z;
                        child.f=this->f;
                        //
                        child.B=remove_common_bits(this->B, board);
                        child.L=remove_common_bits(this->L, board);
                        child.X=remove_common_bits(this->X, board);
                        child.T=remove_common_bits(this->T, board);
                        child.Q=remove_common_bits(this->Q, board);
                        child.K=remove_common_bits(this->K, board);
                        child.Y=remove_common_bits(this->Y, board);
                        child.Z=remove_common_bits(this->Z, board);
                        child.F=remove_common_bits(this->F, board);
                        //
                        uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                        if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==4) {
                    for (const auto& board : gorcTtXxQq(this->t, white_pieces, black_pieces, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}})) {
                        Board child;
                        child.b=this->b;
                        child.l=this->l;
                        child.x=this->x;
                        child.t=board;
                        child.q=this->q;
                        child.k=this->k;
                        child.y=this->y;
                        child.z=this->z;
                        child.f=this->f;
                        //
                        child.B=remove_common_bits(this->B, board);
                        child.L=remove_common_bits(this->L, board);
                        child.X=remove_common_bits(this->X, board);
                        child.T=remove_common_bits(this->T, board);
                        child.Q=remove_common_bits(this->Q, board);
                        child.K=remove_common_bits(this->K, board);
                        child.Y=remove_common_bits(this->Y, board);
                        child.Z=remove_common_bits(this->Z, board);
                        child.F=remove_common_bits(this->F, board);
                        //
                        uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                        if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==3) {
                    for (const auto& board : gorcTtXxQq(this->x, white_pieces, black_pieces, {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}})) {
                        Board child;
                        child.b=this->b;
                        child.l=this->l;
                        child.x=board;
                        child.t=this->t;
                        child.q=this->q;
                        child.k=this->k;
                        child.y=this->y;
                        child.z=this->z;
                        child.f=this->f;
                        //
                        child.B=remove_common_bits(this->B, board);
                        child.L=remove_common_bits(this->L, board);
                        child.X=remove_common_bits(this->X, board);
                        child.T=remove_common_bits(this->T, board);
                        child.Q=remove_common_bits(this->Q, board);
                        child.K=remove_common_bits(this->K, board);
                        child.Y=remove_common_bits(this->Y, board);
                        child.Z=remove_common_bits(this->Z, board);
                        child.F=remove_common_bits(this->F, board);
                        //
                        uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                        if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==5) {
                    for (const auto& board : gorcTtXxQq(this->q, white_pieces, black_pieces, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}})) {
                        Board child;
                        child.b=this->b;
                        child.l=this->l;
                        child.x=this->x;
                        child.t=this->t;
                        child.q=board;
                        child.k=this->k;
                        child.y=this->y;
                        child.z=this->z;
                        child.f=this->f;
                        //
                        child.B=remove_common_bits(this->B, board);
                        child.L=remove_common_bits(this->L, board);
                        child.X=remove_common_bits(this->X, board);
                        child.T=remove_common_bits(this->T, board);
                        child.Q=remove_common_bits(this->Q, board);
                        child.K=remove_common_bits(this->K, board);
                        child.Y=remove_common_bits(this->Y, board);
                        child.Z=remove_common_bits(this->Z, board);
                        child.F=remove_common_bits(this->F, board);
                        //
                        uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                        if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==7) {
                    for (const auto& board : gorcZz(this->z, this->t, white_pieces, black_pieces)) {
                        Board child;
                        child.b=this->b;
                        child.l=this->l;
                        child.x=this->x;
                        child.t=board.first;
                        child.q=this->q;
                        child.k=this->k;
                        child.y=this->y;
                        child.z=board.second;
                        child.f=this->f;
                        //
                        child.B=remove_common_bits(this->B, board.first);
                        child.L=remove_common_bits(this->L, board.first);
                        child.X=remove_common_bits(this->X, board.first);
                        child.T=remove_common_bits(this->T, board.first);
                        child.Q=remove_common_bits(this->Q, board.first);
                        child.K=remove_common_bits(this->K, board.first);
                        child.Y=remove_common_bits(this->Y, board.first);
                        child.Z=remove_common_bits(this->Z, board.first);
                        child.F=remove_common_bits(this->F, board.first);
                        //
                        uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                        if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==6) {
                    for (const auto& board : gorcKk(this->k, white_pieces)) {
                        Board child;
                        child.b=this->b;
                        child.l=this->l;
                        child.x=this->x;
                        child.t=this->t;
                        child.q=this->q;
                        child.k=board;
                        child.y=this->y;
                        child.z=this->z;
                        child.f=this->f;
                        //
                        child.B=remove_common_bits(this->B, board);
                        child.L=remove_common_bits(this->L, board);
                        child.X=remove_common_bits(this->X, board);
                        child.T=remove_common_bits(this->T, board);
                        child.Q=remove_common_bits(this->Q, board);
                        child.K=remove_common_bits(this->K, board);
                        child.Y=remove_common_bits(this->Y, board);
                        child.Z=remove_common_bits(this->Z, board);
                        child.F=remove_common_bits(this->F, board);
                        //
                        uint64_t child_black_pieces=child.B|child.L|child.X|child.T|child.Q|child.K|child.Z|child.Y|child.F;
                        if (child_black_pieces!=black_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==8) {
                    std::vector<Board> children_Yy= gcYy(6, white_pieces, all_pieces);
                    if (children_Yy.size()!=0) {
                        int chosen_index=generate_random_int(0, children_Yy.size()-1);
                        Board child_Yy=children_Yy[chosen_index];
                        children.push_back(child_Yy);
                    }
                }
            }
            else {
                if (chosen_piece==1) {
                    for (const auto& board : gorcBb(-1, this->B, black_pieces, this->F, white_pieces, this->f)) {
                        Board child;
                        child.B=board[0];
                        child.L=this->L;
                        child.X=this->X;
                        child.T=this->T;
                        child.Q=this->Q;
                        child.K=this->K;
                        child.Y=this->Y;
                        child.Z=this->Z;
                        child.F=board[1];
                        //
                        if (board[1]==0) {
                            child.b=remove_common_bits(this->b, board[0]);
                            child.l=remove_common_bits(this->l, board[0]);
                            child.x=remove_common_bits(this->x, board[0]);
                            child.t=remove_common_bits(this->t, board[0]);
                            child.q=remove_common_bits(this->q, board[0]);
                            child.k=remove_common_bits(this->k, board[0]);
                            child.y=remove_common_bits(this->y, board[0]);
                            child.z=remove_common_bits(this->z, board[0]);
                            child.f=remove_common_bits(board[2], board[0]);
                        }
                        else {
                            child.b=this->b;
                            child.l=this->l;
                            child.x=this->x;
                            child.t=this->t;
                            child.q=this->q;
                            child.k=this->k;
                            child.y=this->y;
                            child.z=this->z;
                            child.f=this->f;
                        }
                        //
                        uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                        if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==2) {
                    for (const auto& board : gorcLl(this->L, black_pieces)) {
                        Board child;
                        child.B=this->B;
                        child.L=board;
                        child.X=this->X;
                        child.T=this->T;
                        child.Q=this->Q;
                        child.K=this->K;
                        child.Y=this->Y;
                        child.Z=this->Z;
                        child.F=this->F;
                        //
                        child.b=remove_common_bits(this->b, board);
                        child.l=remove_common_bits(this->l, board);
                        child.x=remove_common_bits(this->x, board);
                        child.t=remove_common_bits(this->t, board);
                        child.q=remove_common_bits(this->q, board);
                        child.k=remove_common_bits(this->k, board);
                        child.y=remove_common_bits(this->y, board);
                        child.z=remove_common_bits(this->z, board);
                        child.f=remove_common_bits(this->f, board);
                        //
                        uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                        if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==4) {
                    for (const auto& board : gorcTtXxQq(this->T, black_pieces, white_pieces, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}})) {
                        Board child;
                        child.B=this->B;
                        child.L=this->L;
                        child.X=this->X;
                        child.T=board;
                        child.Q=this->Q;
                        child.K=this->K;
                        child.Y=this->Y;
                        child.Z=this->Z;
                        child.F=this->F;
                        //
                        child.b=remove_common_bits(this->b, board);
                        child.l=remove_common_bits(this->l, board);
                        child.x=remove_common_bits(this->x, board);
                        child.t=remove_common_bits(this->t, board);
                        child.q=remove_common_bits(this->q, board);
                        child.k=remove_common_bits(this->k, board);
                        child.y=remove_common_bits(this->y, board);
                        child.z=remove_common_bits(this->z, board);
                        child.f=remove_common_bits(this->f, board);
                        //
                        uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                        if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==3) {
                    for (const auto& board : gorcTtXxQq(this->X, black_pieces, white_pieces, {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}})) {
                        Board child;
                        child.B=this->B;
                        child.L=this->L;
                        child.X=board;
                        child.T=this->T;
                        child.Q=this->Q;
                        child.K=this->K;
                        child.Y=this->Y;
                        child.Z=this->Z;
                        child.F=this->F;
                        //
                        child.b=remove_common_bits(this->b, board);
                        child.l=remove_common_bits(this->l, board);
                        child.x=remove_common_bits(this->x, board);
                        child.t=remove_common_bits(this->t, board);
                        child.q=remove_common_bits(this->q, board);
                        child.k=remove_common_bits(this->k, board);
                        child.y=remove_common_bits(this->y, board);
                        child.z=remove_common_bits(this->z, board);
                        child.f=remove_common_bits(this->f, board);
                        //
                        uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                        if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==5) {
                    for (const auto& board : gorcTtXxQq(this->Q, black_pieces, white_pieces, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}})) {
                        Board child;
                        child.B=this->B;
                        child.L=this->L;
                        child.X=this->X;
                        child.T=this->T;
                        child.Q=board;
                        child.K=this->K;
                        child.Y=this->Y;
                        child.Z=this->Z;
                        child.F=this->F;
                        //
                        child.b=remove_common_bits(this->b, board);
                        child.l=remove_common_bits(this->l, board);
                        child.x=remove_common_bits(this->x, board);
                        child.t=remove_common_bits(this->t, board);
                        child.q=remove_common_bits(this->q, board);
                        child.k=remove_common_bits(this->k, board);
                        child.y=remove_common_bits(this->y, board);
                        child.z=remove_common_bits(this->z, board);
                        child.f=remove_common_bits(this->f, board);
                        //
                        uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                        if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==7) {
                    for (const auto& board : gorcZz(this->Z, this->T, black_pieces, white_pieces)) {
                        Board child;
                        child.B=this->B;
                        child.L=this->L;
                        child.X=this->X;
                        child.T=board.first;
                        child.Q=this->Q;
                        child.K=this->K;
                        child.Y=this->Y;
                        child.Z=board.second;
                        child.F=this->F;
                        //
                        child.b=remove_common_bits(this->b, board.first);
                        child.l=remove_common_bits(this->l, board.first);
                        child.x=remove_common_bits(this->x, board.first);
                        child.t=remove_common_bits(this->t, board.first);
                        child.q=remove_common_bits(this->q, board.first);
                        child.k=remove_common_bits(this->k, board.first);
                        child.y=remove_common_bits(this->y, board.first);
                        child.z=remove_common_bits(this->z, board.first);
                        child.f=remove_common_bits(this->f, board.first);
                        //
                        uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                        if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==6) {
                    for (const auto& board : gorcKk(this->K, black_pieces)) {
                        Board child;
                        child.B=this->B;
                        child.L=this->L;
                        child.X=this->X;
                        child.T=this->T;
                        child.Q=this->Q;
                        child.K=board;
                        child.Y=this->Y;
                        child.Z=this->Z;
                        child.F=this->F;
                        //
                        child.b=remove_common_bits(this->b, board);
                        child.l=remove_common_bits(this->l, board);
                        child.x=remove_common_bits(this->x, board);
                        child.t=remove_common_bits(this->t, board);
                        child.q=remove_common_bits(this->q, board);
                        child.k=remove_common_bits(this->k, board);
                        child.y=remove_common_bits(this->y, board);
                        child.z=remove_common_bits(this->z, board);
                        child.f=remove_common_bits(this->f, board);
                        //
                        uint64_t child_white_pieces=child.b|child.l|child.x|child.t|child.q|child.k|child.z|child.y|child.f;
                        if (child_white_pieces!=white_pieces) {child.schlagen=true;}
                        else {child.schlagen=false;}
                        //
                        children.push_back(child);
                    }
                }
                //
                else if (chosen_piece==8) {
                    std::vector<Board> children_Yy= gcYy(-6, black_pieces, all_pieces);
                    if (children_Yy.size()!=0) {
                        int chosen_index=generate_random_int(0, children_Yy.size()-1);
                        Board child_Yy=children_Yy[chosen_index];
                        children.push_back(child_Yy);
                    }
                }
            }
            if (children.size()==1) {return children;}
        }
    }

};

//

std::vector<std::vector<std::vector<int>>> generate_childrenv(std::vector<std::vector<int>> position, int playerk);//declaration weil gcKk gc in def hat und gc gcKk in def hat.

std::vector<std::vector<std::vector<int>>> gcKkv(int y, int x, const std::vector<std::vector<int>> pos, int player) {
    std::vector<std::vector<int>> boardc = pos;
    std::vector<std::vector<std::vector<int>>> childrenK;

    int direction_y[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int direction_x[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i=0; i<8; ++i) {
        int new_y = y + direction_y[i];
        int new_x = x + direction_x[i];

        if (new_y>-1 && new_y<8 && new_x>-1 && new_x<8) {
            if ((player==-6 || player==-8) && boardc[new_y][new_x]>=0) {
                boardc[y][x]=0;
                boardc[new_y][new_x]=-6;
                childrenK.push_back(boardc);
                boardc=pos;
            }
            else if ((player==6 || player==8) && boardc[new_y][new_x]<=0) {
                boardc[y][x]=0;
                boardc[new_y][new_x]=6;
                childrenK.push_back(boardc);
                boardc=pos;
            }
        }
    }
    //Rochade
    if (player==-8) {
        if (boardc[0][0]==-7 && boardc[0][1]==0 && boardc[0][2]==0 && boardc[0][3]==0) {
            boardc[0][2]=-6;
            boardc[0][3]=-4;
            boardc[0][0]=0;
            boardc[0][4]=0;
            bool legal = true;
            for (auto child : generate_childrenv(boardc, 6)) {
                if (child[0][2]>0 || child[0][3]>0 || child[0][4]>0) {legal = false; break;}
            }
            if (legal) {childrenK.push_back(boardc);}
            boardc = pos;
        }
        if (boardc[0][7]==-7 && boardc[0][6]==0 && boardc[0][5]==0) {
            boardc[0][6]=-6;
            boardc[0][5]=-4;
            boardc[0][7]=0;
            boardc[0][4]=0;
            bool legal = true;
            for (auto child : generate_childrenv(boardc, 6)) {
                if (child[0][4]>0 || child[0][5]>0 || child[0][6]>0) {legal = false; break;}
            }
            if (legal) {childrenK.push_back(boardc);}
            boardc = pos;
        }
    }
    else if (player==8) {
        if (boardc[7][0]==7 && boardc[7][1]==0 && boardc[7][2]==0 && boardc[7][3]==0) {
            boardc[7][2]=6;
            boardc[7][3]=4;
            boardc[7][0]=0;
            boardc[7][4]=0;
            bool legal = true;
            for (auto child : generate_childrenv(boardc, -6)) {
                if (child[7][2]<0 || child[7][3]<0 || child[7][4]<0) {legal = false; break;}
            }
            if (legal) {childrenK.push_back(boardc);}
            boardc = pos;
        }
        if (boardc[7][7]==7 && boardc[7][6]==0 && boardc[7][5]==0) {
            boardc[7][6]=6;
            boardc[7][5]=4;
            boardc[7][7]=0;
            boardc[7][4]=0;
            bool legal = true;
            for (auto child : generate_childrenv(boardc, -6)) {
                if (child[7][4]<0 || child[7][5]<0 || child[7][6]<0) {legal = false; break;}
            }
            if (legal) {childrenK.push_back(boardc);}
            boardc = pos;
        }
    }
    //
    return childrenK;
}

std::vector<std::vector<std::vector<int>>> gcLlv(int y, int x, const std::vector<std::vector<int>> pos, int player) {
    std::vector<std::vector<int>> boardc = pos;
    std::vector<std::vector<std::vector<int>>> childrenL;
    //
    int direction_y[] = {-2, -2, 2, 2, 1, -1, 1, -1};
    int direction_x[] = {1, -1, 1, -1, 2, 2, -2, -2};
    //
    if (player==-2) {
        for (int i=0; i<8; ++i) {
            int new_y= y+direction_y[i];
            int new_x= x+direction_x[i];
            if (new_y>-1 && new_y<8 && new_x>-1 && new_x<8 && pos[new_y][new_x]>=0) {
                boardc=pos;
                boardc[y][x]=0;
                boardc[new_y][new_x]=-2;
                childrenL.push_back(boardc);
            }
        }
    }
    else if (player==2) {
        for (int i=0; i<8; ++i) {
            int new_y= y+direction_y[i];
            int new_x= x+direction_x[i];
            if (new_y>-1 && new_y<8 && new_x>-1 && new_x<8 && pos[new_y][new_x]<=0) {
                boardc=pos;
                boardc[y][x]=0;
                boardc[new_y][new_x]=2;
                childrenL.push_back(boardc);
            }
        }
    }
    //
    return childrenL;
}

std::vector<std::vector<std::vector<int>>> gcBbv(int y, int x, const std::vector<std::vector<int>> pos, int player) {
    std::vector<std::vector<int>> boardc = pos;
    std::vector<std::vector<std::vector<int>>> childrenB;
    //
    if (player==-1) {
        if (y==1 && boardc[y+2][x]==0 && boardc[y+1][x]==0) {
            boardc[y][x]=0;
            boardc[y+2][x]=-9;
            childrenB.push_back(boardc);
            boardc=pos;
        }
        if (y+1<8) {
            if (boardc[y+1][x]==0) {
                boardc[y][x]=0;
                boardc[y+1][x]=-1;
                if (y+1==7) {boardc[y+1][x]=-5;}
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        if (x-1>-1 && y+1<8) {
            if (boardc[y+1][x-1]>0) {
                boardc[y][x]=0;
                boardc[y+1][x-1]=-1;
                if (y+1==7) {boardc[y+1][x-1]=-5;}
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        if (x+1<8 && y+1<8) {
            if (boardc[y+1][x+1]>0) {
                boardc[y][x]=0;
                boardc[y+1][x+1]=-1;
                if (y+1==7) {boardc[y+1][x+1]=-5;}
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        //en passant
        if (x-1>-1 && y+1<8) {
            if (boardc[y][x-1]==9 && boardc[y+1][x-1]==0) {
                boardc[y][x]=0;
                boardc[y+1][x-1]=-1;
                boardc[y][x-1]=0;
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        if (x+1<8 && y+1<8) {
            if (boardc[y][x+1]==9 && boardc[y+1][x+1]==0) {
                boardc[y][x]=0;
                boardc[y+1][x+1]=-1;
                boardc[y][x+1]=0;
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
    }
    else if (player==1) {
        if (y==6 && boardc[y-2][x]==0 && boardc[y-1][x]==0) {
            boardc[y][x]=0;
            boardc[y-2][x]=9;
            childrenB.push_back(boardc);
            boardc=pos;
        }
        if (y-1>-1) {
            if (boardc[y-1][x]==0) {
                boardc[y][x]=0;
                boardc[y-1][x]=1;
                if (y-1==0) {boardc[y-1][x]=5;}
                childrenB.push_back(boardc);
                boardc = pos;
            }
        }
        if (x-1>-1 && y-1>-1) {
            if (boardc[y-1][x-1]<0) {
                boardc[y][x]=0;
                boardc[y-1][x-1]=1;
                if (y-1==0) {boardc[y-1][x-1]=5;}
                childrenB.push_back(boardc);
                boardc = pos;
            }
        }
        if (x+1<8 && y-1>-1) {
            if (boardc[y-1][x+1]<0) {
                boardc[y][x]=0;
                boardc[y-1][x+1]=1;
                if (y-1==0) {boardc[y-1][x+1]=5;}
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        //en passant
        if (x-1>-1 && y-1>-1) {
            if (boardc[y][x-1]==-9 && boardc[y-1][x-1]==0) {
                boardc[y][x]=0;
                boardc[y-1][x-1]=1;
                boardc[y][x-1]=0;
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        if (x+1<8 && y-1>-1) {
            if (boardc[y][x+1]==-9 && boardc[y-1][x+1]==0) {
                boardc[y][x]=0;
                boardc[y-1][x+1]=1;
                boardc[y][x+1]=0;
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
    }
    return childrenB;
}

std::vector<std::vector<std::vector<int>>> gcTtXxQqv(int y, int x, const std::vector<std::vector<int>> pos, int player, std::vector<int> direction_y, std::vector<int> direction_x) {
    std::vector<std::vector<std::vector<int>>> children;
    std::vector<std::vector<int>> boardc;
    //
    if (player<0) {
        for (int dir=0; dir<direction_y.size(); ++dir) {
            for (int step=1; step<8; ++step) {
                int new_y = y+step * direction_y[dir];
                int new_x = x+step * direction_x[dir];
                //
                if (new_y>-1 && new_y<8 && new_x>-1 && new_x<8) {
                    if (pos[new_y][new_x]>=0) {
                        boardc=pos;
                        boardc[y][x]=0;
                        boardc[new_y][new_x]=player;
                        children.push_back(boardc);
                        if (pos[new_y][new_x]!=0) {break;}
                    }
                    else {break;}
                }
                else {break;}
            }
        }
    }
    else if (player>0) {
        for (int dir=0; dir<direction_y.size(); ++dir) {
            for (int step = 1; step <8; ++step) {
                int new_y = y + step * direction_y[dir];
                int new_x = x + step * direction_x[dir];
                //
                if (new_y>-1 && new_y<8 && new_x>-1 && new_x<8) {
                    if (pos[new_y][new_x]<=0) {
                        boardc=pos;
                        boardc[y][x]=0;
                        boardc[new_y][new_x]=player;
                        children.push_back(boardc);
                        if (pos[new_y][new_x]!=0) {break;}
                    }
                    else {break;}
                }
                else {break;}
            }
        }
    }
    //
    return children;
}

std::vector<std::vector<std::vector<int>>> generate_childrenv(std::vector<std::vector<int>> position, int playerk) {
    std::vector<std::vector<std::vector<int>>> children;
    // 9&-9 zu 1&-1
    if (playerk==6) {
        for (int y=0; y<position.size(); ++y) {
            for (int x=0; x<position[y].size(); ++x) {
                if (position[y][x]==9) {position[y][x]=1;}
            }
        }
    }
    else if (playerk==-6) {
        for (int y=0; y<position.size(); ++y) {
            for (int x=0; x<position[y].size(); ++x) {
                if (position[y][x]==-9) {position[y][x]=-1;}
            }
        }
    }
    //
    if (playerk==6) {
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (position[y][x]==1) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcBbv(y, x, position, 1);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==6) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcKkv(y, x, position, 6);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==4 || position[y][x]==7) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQqv(y, x, position, 4, {0, 0, 1, -1}, {1, -1, 0, 0});
                    children.insert(children.end(), new_children.begin(), new_children.end());                
                }
                else if (position[y][x]==3) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQqv(y, x, position, 3, {1, -1, -1, 1}, {1, 1, -1, -1});
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==5) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQqv(y, x, position, 5, {1, -1, -1, 1, 0, 0, 1, -1}, {1, 1, -1, -1, 1, -1, 0, 0});
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==2) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcLlv(y, x, position, 2);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==8) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcKkv(y, x, position, 8);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
            }
        }
    }
    else if (playerk==-6) {
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (position[y][x]==-1) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcBbv(y, x, position, -1);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==-6) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcKkv(y, x, position, -6);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==-4 || position[y][x]==-7) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQqv(y, x, position, -4, {0, 0, 1, -1}, {1, -1, 0, 0});
                    children.insert(children.end(), new_children.begin(), new_children.end());                
                }
                else if (position[y][x]==-3) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQqv(y, x, position, -3, {1, -1, -1, 1}, {1, 1, -1, -1});
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==-5) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQqv(y, x, position, -5, {1, -1, -1, 1, 0, 0, 1, -1}, {1, 1, -1, -1, 1, -1, 0, 0});
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==-2) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcLlv(y, x, position, -2);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==-8) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcKkv(y, x, position, -8);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
            }
        }
    }
    return children;
}

std::vector<std::vector<int>> convert_board_to_vector(const Board& board) {
    std::vector<std::vector<int>> result(8, std::vector<int>(8, 0));
    for (int i=0; i<64; i++) {
        int row = i/8;
        int col = i%8;

        if (board.B & (1ULL << i)) result[row][col]= -1;
        else if (board.L & (1ULL<<i)) result[row][col]= -2;
        else if (board.X & (1ULL<<i)) result[row][col]= -3;
        else if (board.T & (1ULL<<i)) result[row][col]= -4;
        else if (board.Q & (1ULL<<i)) result[row][col]= -5;
        else if (board.K & (1ULL<<i)) result[row][col]= -6;
        else if (board.Z & (1ULL<<i)) result[row][col]= -7;
        else if (board.Y & (1ULL<<i)) result[row][col]= -8;
        else if (board.F & (1ULL<<i)) result[row][col]= -9;

        else if (board.b & (1ULL<<i)) result[row][col]= 1;
        else if (board.l & (1ULL<<i)) result[row][col]= 2;
        else if (board.x & (1ULL<<i)) result[row][col]= 3;
        else if (board.t & (1ULL<<i)) result[row][col]= 4;
        else if (board.q & (1ULL<<i)) result[row][col]= 5;
        else if (board.k & (1ULL<<i)) result[row][col]= 6;
        else if (board.z & (1ULL<<i)) result[row][col]= 7;
        else if (board.y & (1ULL<<i)) result[row][col]= 8;
        else if (board.f & (1ULL<<i)) result[row][col]= 9;
    }
    std::reverse(result.begin(), result.end());
    for (int i=0; i<8; i++) {std::reverse(result[i].begin(), result[i].end());}
    return result;
}

Board* convert_vector_to_board(std::vector<std::vector<int>> vector) {
    Board* board = new Board;
    //
    std::reverse(vector.begin(), vector.end());
    for (int i=0; i<8; i++) {std::reverse(vector[i].begin(), vector[i].end());}
    //
    board->b = 0ULL;
    board->l = 0ULL;
    board->x = 0ULL;
    board->t = 0ULL;
    board->q = 0ULL;
    board->k = 0ULL;
    board->z = 0ULL;
    board->y = 0ULL;
    board->f = 0ULL;
    board->B = 0ULL;
    board->L = 0ULL;
    board->X = 0ULL;
    board->T = 0ULL;
    board->Q = 0ULL;
    board->K = 0ULL;
    board->Y = 0ULL;
    board->Z = 0ULL;
    board->F = 0ULL;
    //
    for (int row=0; row<8; row++) {
        for (int col=0; col<8; col++) {
            int piece = vector[row][col];
            if (piece==-1) board->B |= (1ULL << (row*8+col));
            else if (piece==-2) board->L |= (1ULL << (row*8+col));
            else if (piece==-3) board->X |= (1ULL << (row*8+col));
            else if (piece==-4) board->T |= (1ULL << (row*8+col));
            else if (piece==-5) board->Q |= (1ULL << (row*8+col));
            else if (piece==-6) board->K |= (1ULL << (row*8+col));
            else if (piece==-7) board->Z |= (1ULL << (row*8+col));
            else if (piece==-8) board->Y |= (1ULL << (row*8+col));
            else if (piece==-9) board->F |= (1ULL << (row*8+col));

            else if (piece==1) board->b |= (1ULL << (row*8+col));
            else if (piece==2) board->l |= (1ULL << (row*8+col));
            else if (piece==3) board->x |= (1ULL << (row*8+col));
            else if (piece==4) board->t |= (1ULL << (row*8+col));
            else if (piece==5) board->q |= (1ULL << (row*8+col));
            else if (piece==6) board->k |= (1ULL << (row*8+col));
            else if (piece==7) board->z |= (1ULL << (row*8+col));
            else if (piece==8) board->y |= (1ULL << (row*8+col));
            else if (piece==9) board->f |= (1ULL << (row*8+col));
        }
    }
    board->print_board();
    return board;
}

void print_vector_board(const std::vector<std::vector<int>>& board) {
    std::vector zahlen= {8,7,6,5,4,3,2,1};
    std::cout <<"    a   b   c   d   e   f   g   h\n";
    std::cout <<"  ---------------------------------\n";
    for (int i=0; i<8; ++i) {
        std::cout << zahlen[i] <<" I";
        for (int j=0; j<8; ++j) {
            std::cout << " ";
            if (board[i][j]==1) {std::cout << 'b';}
            else if (board[i][j]==9) {std::cout <<'f';}
            else if (board[i][j]==2) {std::cout <<'l';}
            else if (board[i][j]==3) {std::cout <<'x';}
            else if (board[i][j]==4) {std::cout <<'t';}
            else if (board[i][j]==7) {std::cout <<'z';}
            else if (board[i][j]==5) {std::cout <<'q';}
            else if (board[i][j]==6) {std::cout <<'k';}
            else if (board[i][j]==8) {std::cout <<'y';}
            else if (board[i][j]==-1) {std::cout <<'B';}
            else if (board[i][j]==-9) {std::cout <<'F';}
            else if (board[i][j]==-2) {std::cout <<'L';}
            else if (board[i][j]==-3) {std::cout <<'X';}
            else if (board[i][j]==-4) {std::cout <<'T';}
            else if (board[i][j]==-7) {std::cout <<'Z';}
            else if (board[i][j]==-5) {std::cout <<'Q';}
            else if (board[i][j]==-6) {std::cout <<'K';}
            else if (board[i][j]==-8) {std::cout <<'Y';}
            else if (board[i][j]==0) {std::cout <<' ';}
            std::cout << " ";
            std::cout << "I";
        }
        std::cout <<'\n';
        std::cout <<"  ---------------------------------\n";
    }
}

bool verlorenv(const std::vector<std::vector<int>>& board, int playerk) {
    int player_2 = (playerk == 6) ? 8 : -8;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == playerk || board[i][j] == player_2) {
                return false;
            }
        }
    }
    return true;
}

std::vector x_inputs={'a','b','c', 'd', 'e', 'f', 'g', 'h'};
std::vector y_inputs={8,7,6,5,4,3,2,1};

class HumanPlayer {
public:
    HumanPlayer(int token, Board board) : token(token), board(board) {}
    int token=token;
    Board board;

    bool do_these_two_vectors_have_the_same_elements(const std::vector<std::vector<int>>& vector1, const std::vector<std::vector<int>>& vector2) {
        for (size_t i=0; i<vector1.size(); ++i) {
            for (size_t j=0; j<vector1[i].size(); ++j) {
                if (vector1[i][j] != vector2[i][j]) {return false;}
            }
        }
        return true;
    }

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

    std::vector<std::vector<int>> player(std::vector<std::vector<int>>& pos) {
        int other_player = (this->token == 6) ? -6 : 6;
        std::vector<std::vector<std::vector<int>>> legal_moves;
        bool legal_move_exists = false;
        std::vector<std::vector<std::vector<int>>> all_moves = generate_childrenv(pos, this->token);
        //
        for (const auto& child_of_root : all_moves) {
            bool king_is_killed = false;
            for (const auto& child_of_child : generate_childrenv(child_of_root, other_player)) {
                if (verlorenv(child_of_child, this->token)) {
                    king_is_killed = true;
                    break;
                }
            }
            if (!king_is_killed) {
                legal_move_exists = true;
                legal_moves.push_back(child_of_root);
            }
        }
        //
        if (!legal_move_exists) {
            std::vector<std::vector<int>> empty_vector;
            return empty_vector;
        }
        //
        while (true) {
            std::vector<std::vector<int>> boardcopy = pos;
            std::vector<int> input_move = this->eingabe();
            int vy = input_move[0];
            int vx = input_move[1];
            int zy = input_move[2];
            int zx = input_move[3];

            //update boardcopy
            //1 & -1 to 5 & -5
            for (int x=0; x<boardcopy[0].size(); ++x) {
                if (boardcopy[0][x]==1) {boardcopy[0][x]=5;}
            }
            for (int x=0; x<boardcopy[7].size(); ++x) {
                if (boardcopy[7][x]==-1) {boardcopy[7][x]=-5;}
            }
            //9 & -9weg
            if (this->token==6) {
                for (int y=0; y<boardcopy.size(); ++y) {
                    for (int x=0; x<boardcopy[y].size(); ++x) {
                        if (boardcopy[y][x]==9) {boardcopy[y][x] = 1;}
                    }
                }
            }
            else if (this->token==-6) {
                for (int y=0; y<boardcopy.size(); ++y) {
                    for (int x=0; x<boardcopy[y].size(); ++x) {
                        if (boardcopy[y][x]==-9) {boardcopy[y][x] = -1;}
                    }
                }
            }
            //special moves
            bool special = false;
            //rochade
            if (vy==7 && vx==4 && zy==7 && zx==2 && this->token==6 && boardcopy[vy][vx]==8) {
                special = true;
                boardcopy[7][2] = 6;
                boardcopy[7][3] = 4;
                boardcopy[7][0] = 0;
                boardcopy[7][4] = 0;
            }
            if (vy==7 && vx==4 && zy==7 && zx==6 && this->token==6 && boardcopy[vy][vx]==8) {
                special = true;
                boardcopy[7][6] = 6;
                boardcopy[7][5] = 4;
                boardcopy[7][7] = 0;
                boardcopy[7][4] = 0;
            }
            if (vy==0 && vx==4 && zy==0 && zx==2 && this->token==-6 && boardcopy[vy][vx]==-8) {
                special = true;
                boardcopy[0][2] = -6;
                boardcopy[0][3] = -4;
                boardcopy[0][0] = 0;
                boardcopy[0][4] = 0;
            }
            if (vy==0 && vx==4 && zy==0 && zx==6 && this->token==-6 && boardcopy[vy][vx]==-8) {
                special = true;
                boardcopy[0][6] = -6;
                boardcopy[0][5] = -4;
                boardcopy[0][7] = 0;
                boardcopy[0][4] = 0;
            }
            // Bb 2 nach vorne
            if (boardcopy[vy][vx]==1 && vy==6 && boardcopy[zy][zx]==0 && boardcopy[vy-1][vx]==0 && vx==zx && zy==vy-2) {
                special = true;
                boardcopy[zy][zx] = 9;
                boardcopy[vy][vx] = 0;
            }
            if (boardcopy[vy][vx]==-1 && vy==1 && boardcopy[zy][zx]==0 && boardcopy[vy+1][vx]==0 && vx==zx && zy==vy+2) {
                special = true;
                boardcopy[zy][zx] = -9;
                boardcopy[vy][vx] = 0;
            }
            // En passant
            if (boardcopy[vy][vx]==1 && zy==vy-1 && zx==vx-1 && boardcopy[zy][zx]==0) {
                special = true;
                boardcopy[vy][vx] = 0;
                boardcopy[zy][zx] = 1;
                boardcopy[vy][zx] = 0;
            }
            if (boardcopy[vy][vx]==1 && zy==vy-1 && zx==vx+1 && boardcopy[zy][zx]==0) {
                special = true;
                boardcopy[vy][vx] = 0;
                boardcopy[zy][zx] = 1;
                boardcopy[vy][zx] = 0;
            }
            if (boardcopy[vy][vx]==-1 && zy==vy+1 && zx==vx-1 && boardcopy[zy][zx]==0) {
                special = true;
                boardcopy[vy][vx] = 0;
                boardcopy[zy][zx] = -1;
                boardcopy[vy][zx] = 0;
            }
            if (boardcopy[vy][vx]==-1 && zy==vy+1 && zx==vx+1 && boardcopy[zy][zx]==0) {
                special = true;
                boardcopy[vy][vx] = 0;
                boardcopy[zy][zx] = -1;
                boardcopy[vy][zx] = 0;
            }
            //normal
            if (!special) {
                if (boardcopy[vy][vx]==7) {boardcopy[zy][zx] = 4;}
                else if (boardcopy[vy][vx]==-7) {boardcopy[zy][zx] = -4;}
                else if (boardcopy[vy][vx]==8) {boardcopy[zy][zx] = 6;}
                else if (boardcopy[vy][vx]==-8) {boardcopy[zy][zx] = -6;}
                else {boardcopy[zy][zx] = boardcopy[vy][vx];}
                boardcopy[vy][vx] = 0;
            }
            //
            bool move_legal = false;
            for (const auto& legal_move : legal_moves) {
                if (do_these_two_vectors_have_the_same_elements(boardcopy, legal_move)) {
                    move_legal = true;
                    break;
                }
            }
            if (move_legal) {return boardcopy;}
            else {
                print_vector_board(boardcopy);
                std::cout << "legal_children" << std::endl;
                for (const auto& s : legal_moves) {print_vector_board(s);}
                std::cout << "---------------------------------" << std::endl;
                print_vector_board(pos);
                std::cout << "EINGABE NICHT KORREKT2" << std::endl;
            }
        }
    }

    Board* get_move() {
        std::vector<std::vector<int>> vector_board= convert_board_to_vector(board);
        std::vector<std::vector<int>> return_vector;
        return_vector= player(vector_board);
        Board* return_board= new Board;
        return_board=convert_vector_to_board(return_vector);
        return return_board;
    }

};

//
//minimax normal
int minimax_counter=0;

class MinimaxNode {
public:
    MinimaxNode() : value(0), value_not_none(false), children(), board(), player_am_zug(0), token(0), depth(0), expanded(false) {}

    int value;
    bool value_not_none;
    std::vector<MinimaxNode> children;
    Board board;
    int player_am_zug;
    int token;
    int depth;
    bool expanded;

    std::vector<MinimaxNode> expand_node() {
        std::vector<MinimaxNode> new_children;
        std::vector<Board> list_of_positions = this->board.generate_children(this->player_am_zug);
        for (const Board& board_position : list_of_positions) {
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
        if (this->depth>=max_depth || this->board.verloren(6) || this->board.verloren(-6)) {
            this->value = this->board.evaluate_position(this->token);
            this->value_not_none=true;
            return this->value;
        }
        //
        if (!this->expanded) {this->children=this->expand_node(); this->expanded = true;}
        if (this->children.empty()) {
            this->value = this->board.evaluate_position(this->token);
            this->value_not_none=true;
            return this->value;
        }
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
    MinimaxPlayer(int token, Board board) : token(token), board(board) {
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
    Board board;
    int max_time=5;
    int max_depth=10;
    int starting_depth=2;

    Board* minimaxer(const int depth, const std::chrono::duration<double> vergangene_zeit) {
        auto start = std::chrono::high_resolution_clock::now();
        //
        std::vector<int> values;
        std::vector<MinimaxNode> best_moves;
        MinimaxNode best_move;
        int best_value = -100000000;
        std::vector<MinimaxNode>& root_node_children=root_node.children;
        Board return_board;
        //
        for (MinimaxNode& child : root_node_children){
            int eval;
            if (child.value>-9000000) {
                eval=child.minimax(-100000000,100000000,false, depth);
                child.value=eval;
                std::cout<<"a ";//child wurde fertig berechnet
            }
            //
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> vergangene_zeit2 =(now+vergangene_zeit)-start;
            if (vergangene_zeit2.count() >= max_time) {std::cout<<" NICHT FERTIG"; break;}
        }
        //
        for (MinimaxNode& child : root_node_children) {
            if (child.value>-9000000) {
                values.push_back(child.value);
            }
        }
        if (values.empty()) {return nullptr;}
        for (int v : values) {if (v > best_value) {best_value = v;}}
        for (MinimaxNode& child : root_node_children) {if (child.value==best_value) {best_moves.push_back(child);}}
        //
        //output---------
        std::cout << std::endl;
        std::cout << best_value << std::endl;
        std::cout<<"COUNTER: "; std::cout<<minimax_counter<<std::endl;
        //---------------
        best_move=best_moves[generate_random_int(0, best_moves.size()-1)];
        return_board=best_move.board;
        //
        Board* ret=&return_board;
        return ret;

    }

    Board* get_move() {
        auto start = std::chrono::high_resolution_clock::now();
        //
        int depth=this->starting_depth;
        Board* move = new Board();
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
            Board* new_move=minimaxer(depth,vergangene_zeit);
            *move=*new_move;
            if (new_move==nullptr) {return nullptr;}
            //
            for (const MinimaxNode& child : root_node.children) {std::cout<<child.value;  std::cout<<", ";}
            std::cout<<std::endl;
            //sort+depth
            //this->root_node.sort(true);
            //
            for (MinimaxNode& child : root_node.children) {std::cout<<child.value;  std::cout<<", ";}
            std::cout<<std::endl;
            if (depth>max_depth) {break;}
            depth+=1;
        }
        return move;
    }

};

//
//erweitert nur schlagen-züge ab einer gewissen tiefe
class MinimaxNode2 {
public:
    MinimaxNode2() : value(0), value_not_none(false), children(), board(), player_am_zug(0), token(0), depth(0), expanded(false) {}

    int value;
    bool value_not_none;
    std::vector<MinimaxNode2> children;
    Board board;
    int player_am_zug;
    int token;
    int depth;
    bool expanded;

    std::vector<MinimaxNode2> expand_node() {
        std::vector<MinimaxNode2> new_children;
        std::vector<Board> list_of_positions=this->board.generate_children(this->player_am_zug);
        for (const Board& board_position : list_of_positions) {
            MinimaxNode2 child;
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

    int minimax(int alpha, int beta, bool max_player, const int max_depth, const int depth_fuer_nur_schlagen) {
        //
        minimax_counter+=1;
        //
        if (this->depth>=max_depth || this->board.verloren(6) || this->board.verloren(-6)) {
            this->value = this->board.evaluate_position(this->token);
            this->value_not_none=true;
            return this->value;
        }
        //
        if (!this->expanded) {
            if (this->depth>=depth_fuer_nur_schlagen) {
                if (this->board.schlagen) {this->children=this->expand_node(); this->expanded = true;}
                else {
                    this->value = this->board.evaluate_position(this->token);
                    this->value_not_none=true;
                    return this->value;
                }
            }
            else {this->children=this->expand_node(); this->expanded = true;}
        }
        if (this->children.empty()) {
            this->value = this->board.evaluate_position(this->token);
            this->value_not_none=true;
            return this->value;
        }
        //
        if (max_player) {
            int max_value=-1000000;
            for (MinimaxNode2& child : this->children) {
                int eval=child.minimax(alpha,beta,false,max_depth,depth_fuer_nur_schlagen);
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
            for (MinimaxNode2& child : this->children) {
                int eval=child.minimax(alpha,beta,true,max_depth,depth_fuer_nur_schlagen);
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
            std::vector<MinimaxNode2> not_none_children;
            std::list<MinimaxNode2> none_children;
            std::vector<MinimaxNode2> sorted_children;
            //
            for (const MinimaxNode2& child : this->children) {
                if (!child.value_not_none) {none_children.push_back(child);}
                else {not_none_children.push_back(child);}
            }
            //
            if (max_player) {
                std::sort(not_none_children.begin(), not_none_children.end(),[](const MinimaxNode2& a, const MinimaxNode2& b) {return a.value > b.value;});
                //
                sorted_children.insert(sorted_children.end(), not_none_children.begin(), not_none_children.end());
                sorted_children.insert(sorted_children.end(), none_children.begin(), none_children.end());
                this->children=sorted_children;
                //
                for (MinimaxNode2& child : not_none_children) {child.sort(false);}
            }
            else {
                std::sort(not_none_children.begin(), not_none_children.end(),[](const MinimaxNode2& a, const MinimaxNode2& b) {return a.value < b.value;});
                //
                sorted_children.insert(sorted_children.end(), not_none_children.begin(), not_none_children.end());
                sorted_children.insert(sorted_children.end(), none_children.begin(), none_children.end());
                this->children=sorted_children;
                //
                for (MinimaxNode2& child : not_none_children) {child.sort(true);}
            }
        }
    }

};

class MinimaxPlayer2 {
public:
    MinimaxPlayer2(int token, Board board) : token(token), board(board) {
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

    MinimaxNode2 root_node;
    int token;
    Board board;
    int max_time=10;
    int max_depth=10;
    int starting_depth=2;
    int depth_fuer_nur_schlagen=5;

    Board* minimaxer(const int depth, const std::chrono::duration<double> vergangene_zeit) {
        auto start = std::chrono::high_resolution_clock::now();
        //
        std::vector<int> values;
        std::vector<MinimaxNode2> best_moves;
        MinimaxNode2 best_move;
        int best_value = -100000000;
        std::vector<MinimaxNode2>& root_node_children=root_node.children;
        Board return_board;
        //
        for (MinimaxNode2& child : root_node_children){
            int eval;
            if (child.value>-9000000) {
                eval=child.minimax(-100000000,100000000,false, depth, depth_fuer_nur_schlagen);
                child.value=eval;
                std::cout<<"a ";//child wurde fertig berechnet
            }
            //
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> vergangene_zeit2 =(now+vergangene_zeit)-start;
            if (vergangene_zeit2.count() >= max_time) {std::cout<<" NICHT FERTIG"; break;}
        }
        //
        for (MinimaxNode2& child : root_node_children) {
            if (child.value>-9000000) {
                values.push_back(child.value);
            }
        }
        if (values.empty()) {return nullptr;}
        for (int v : values) {if (v > best_value) {best_value = v;}}
        for (MinimaxNode2& child : root_node_children) {if (child.value==best_value) {best_moves.push_back(child);}}
        //
        //output---------
        std::cout << std::endl;
        std::cout << best_value << std::endl;
        std::cout<<"COUNTER: "; std::cout<<minimax_counter<<std::endl;
        //---------------
        best_move=best_moves[generate_random_int(0, best_moves.size()-1)];
        return_board=best_move.board;
        //
        Board* ret=&return_board;
        return ret;

    }

    Board* get_move() {
        auto start = std::chrono::high_resolution_clock::now();
        //
        int depth=this->starting_depth;
        Board* move = new Board();
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
            Board* new_move=minimaxer(depth,vergangene_zeit);
            *move=*new_move;
            if (new_move==nullptr) {return nullptr;}
            //
            for (const MinimaxNode2& child : root_node.children) {std::cout<<child.value;  std::cout<<", ";}
            std::cout<<std::endl;
            //sort+depth
            this->root_node.sort(true);
            //
            for (MinimaxNode2& child : root_node.children) {std::cout<<child.value;  std::cout<<", ";}
            std::cout<<std::endl;
            if (depth>max_depth) {break;}
            depth+=1;
        }
        return move;
    }

};

//
//minimax2 + ignoriert unvollständig berechnete ebenen
class MinimaxNode3 {
public:
    MinimaxNode3() : value(0), value_not_none(false), children(), board(), player_am_zug(0), token(0), depth(0), expanded(false) {}

    int value;
    bool value_not_none;
    std::vector<MinimaxNode3> children;
    Board board;
    int player_am_zug;
    int token;
    int depth;
    bool expanded;

    std::vector<MinimaxNode3> expand_node() {
        std::vector<MinimaxNode3> new_children;
        std::vector<Board> list_of_positions=this->board.generate_children(this->player_am_zug);
        for (const Board& board_position : list_of_positions) {
            MinimaxNode3 child;
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

    int minimax(int alpha, int beta, bool max_player, const int max_depth, const int depth_fuer_nur_schlagen) {
        //
        minimax_counter+=1;
        //
        if (this->depth>=max_depth || this->board.verloren(6) || this->board.verloren(-6)) {
            this->value = this->board.evaluate_position(this->token);
            this->value_not_none=true;
            return this->value;
        }
        //
        if (!this->expanded) {
            if (this->depth>=depth_fuer_nur_schlagen) {
                if (this->board.schlagen) {this->children=this->expand_node(); this->expanded = true;}
                else {
                    this->value = this->board.evaluate_position(this->token);
                    this->value_not_none=true;
                    return this->value;
                }
            }
            else {this->children=this->expand_node(); this->expanded = true;}
        }
        if (this->children.empty()) {
            this->value = this->board.evaluate_position(this->token);
            this->value_not_none=true;
            return this->value;
        }
        //
        if (max_player) {
            int max_value=-1000000;
            for (MinimaxNode3& child : this->children) {
                int eval=child.minimax(alpha,beta,false,max_depth,depth_fuer_nur_schlagen);
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
            for (MinimaxNode3& child : this->children) {
                int eval=child.minimax(alpha,beta,true,max_depth,depth_fuer_nur_schlagen);
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
            std::vector<MinimaxNode3> not_none_children;
            std::list<MinimaxNode3> none_children;
            std::vector<MinimaxNode3> sorted_children;
            //
            for (const MinimaxNode3& child : this->children) {
                if (!child.value_not_none) {none_children.push_back(child);}
                else {not_none_children.push_back(child);}
            }
            //
            if (max_player) {
                std::sort(not_none_children.begin(), not_none_children.end(),[](const MinimaxNode3& a, const MinimaxNode3& b) {return a.value > b.value;});
                //
                sorted_children.insert(sorted_children.end(), not_none_children.begin(), not_none_children.end());
                sorted_children.insert(sorted_children.end(), none_children.begin(), none_children.end());
                this->children=sorted_children;
                //
                for (MinimaxNode3& child : not_none_children) {child.sort(false);}
            }
            else {
                std::sort(not_none_children.begin(), not_none_children.end(),[](const MinimaxNode3& a, const MinimaxNode3& b) {return a.value < b.value;});
                //
                sorted_children.insert(sorted_children.end(), not_none_children.begin(), not_none_children.end());
                sorted_children.insert(sorted_children.end(), none_children.begin(), none_children.end());
                this->children=sorted_children;
                //
                for (MinimaxNode3& child : not_none_children) {child.sort(true);}
            }
        }
    }

};

class MinimaxPlayer3 {
public:
    MinimaxPlayer3(int token, Board board) : token(token), board(board) {
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

    MinimaxNode3 root_node;
    int token;
    Board board;
    int max_time=30;
    int max_depth=10;
    int starting_depth=2;
    int depth_fuer_nur_schlagen=5;

    Board* minimaxer(const int depth, const std::chrono::duration<double> vergangene_zeit) {
        auto start = std::chrono::high_resolution_clock::now();
        //
        std::vector<int> values;
        std::vector<MinimaxNode3> best_moves;
        MinimaxNode3 best_move;
        int best_value = -100000000;
        std::vector<MinimaxNode3>& root_node_children=root_node.children;
        Board return_board;
        //
        for (MinimaxNode3& child : root_node_children){
            int eval;
            if (child.value>-9000000) {
                eval=child.minimax(-100000000,100000000,false, depth, depth_fuer_nur_schlagen);
                child.value=eval;
                std::cout<<"a ";//child wurde fertig berechnet
            }
            //
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> vergangene_zeit2 =(now+vergangene_zeit)-start;
            if (vergangene_zeit2.count() >= max_time) {std::cout<<" NICHT FERTIG"; break;}
        }
        //
        for (MinimaxNode3& child : root_node_children) {
            if (child.value>-9000000) {
                values.push_back(child.value);
            }
        }
        if (values.empty()) {return nullptr;}
        for (int v : values) {if (v > best_value) {best_value = v;}}
        for (MinimaxNode3& child : root_node_children) {if (child.value==best_value) {best_moves.push_back(child);}}
        //
        //output---------
        std::cout << std::endl;
        std::cout << best_value << std::endl;
        std::cout<<"COUNTER: "; std::cout<<minimax_counter<<std::endl;
        //---------------
        best_move=best_moves[generate_random_int(0, best_moves.size()-1)];
        return_board=best_move.board;
        //
        Board* ret=&return_board;
        return ret;

    }

    Board* get_move() {
        auto start = std::chrono::high_resolution_clock::now();
        //
        int depth=this->starting_depth;
        Board* move = new Board();
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
            Board* new_move=minimaxer(depth,vergangene_zeit);
            now = std::chrono::high_resolution_clock::now();
            vergangene_zeit = now - start;
            if (vergangene_zeit.count() <= max_time) {*move=*new_move;}
            //
            for (const MinimaxNode3& child : root_node.children) {std::cout<<child.value;  std::cout<<", ";}
            std::cout<<std::endl;
            //sort+depth
            now = std::chrono::high_resolution_clock::now();
            vergangene_zeit = now - start;
            if (vergangene_zeit.count() >= max_time) {break;}
            this->root_node.sort(true);
            //
            for (MinimaxNode3& child : root_node.children) {std::cout<<child.value;  std::cout<<", ";}
            std::cout<<std::endl;
            if (depth>max_depth) {break;}
            depth+=1;
        }
        return move;
    }

};

//

double c=1200;
int number_of_simulations=100;
int depth=3;

int mcts_counter=0;

class MCTSNode {
public:
    MCTSNode() : value(0), children(),parent(nullptr), visits(0), board(), player_am_zug(0), token(0), expanded(false) {}

    int value;
    std::vector<MCTSNode> children;
    MCTSNode* parent;
    int visits;
    Board board;
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
        std::vector<Board> list_of_positions = this->board.generate_children(this->player_am_zug);
        for (const auto& board_position : list_of_positions) {
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
        if (selected_node->expanded) {return selected_node->select_leaf_node();}
        else {return selected_node;}
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
            Board pos = this->board;
            int player = this->player_am_zug;
            for (int i = 0; i < depth; ++i) {
                std::vector<Board> next_pos = pos.generate_one_random_child(player);
                if (next_pos.empty()) {break;}
                pos = next_pos[0];
                //sim ist gut
                if (player== -6) {player= 6;}
                else if (player== 6) {player= -6;}
            }
            values.push_back(pos.evaluate_position(this->token));
        }
        for (double val : values) {value += val;}
        value /= values.size();
        //
        return value;
    }

};

class MCTSPlayer {
public:
    MCTSPlayer(int token, Board board) : token(token), board(board) {
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
    Board board;
    int max_time=10;

    bool mcts() {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        //filter legal moves
        std::vector<MCTSNode> legal_moves;
        int number_of_legal_moves = 0;
        root_node.children=root_node.expand_node();
        for (MCTSNode& child_of_root : root_node.children) {
            child_of_root.children=child_of_root.expand_node();
            bool king_is_killed = false;
            for (MCTSNode& child_of_child : child_of_root.children) {
                if (child_of_child.board.verloren(root_node.player_am_zug)) {
                    king_is_killed = true;
                    break;
                }
            }
            if (!king_is_killed) {
                legal_moves.push_back(child_of_root);
                number_of_legal_moves++;
            }
        }
        root_node.children = legal_moves;
        // No legal moves left
        if (number_of_legal_moves == 0) {return false;}
        //
        while (true) {
            mcts_counter += 1;
            MCTSNode* selected_node=root_node.select_leaf_node();
            selected_node->children=selected_node->expand_node();
            //
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

    Board* mctser() {
        mcts_counter = 0;
        //
        bool monte_carlo=mcts();
        //
        Board* best_move=new Board();
        if (monte_carlo) {
            std::cout <<"COUNTER: ";
            std::cout << mcts_counter << std::endl;
            //
            int highest_number_of_visits = -1;
            //
            for (MCTSNode& root_node_child : root_node.children) {
                if (root_node_child.visits > highest_number_of_visits) {
                    *best_move = root_node_child.board;
                    highest_number_of_visits = root_node_child.visits;
                }
            }
            return best_move;
        }
        else {delete best_move; return nullptr;}
    }

    Board* get_move() {
        return mctser();
    }

};

//

int max_turns=100;

class Schach {
public:
Board board;
int turn;
    Schach() : turn(1) {
        board.b   = 0b0000000000000000000000000000000000000000000000001111111100000000ULL;
        board.l = 0b0000000000000000000000000000000000000000000000000000000001000010ULL;
        board.x = 0b0000000000000000000000000000000000000000000000000000000000100100ULL;
        board.t   = 0b0000000000000000000000000000000000000000000000000000000000000000ULL;
        board.q  = 0b0000000000000000000000000000000000000000000000000000000000010000ULL;
        board.k   = 0b0000000000000000000000000000000000000000000000000000000000000000ULL;

        board.y   = 0b0000000000000000000000000000000000000000000000000000000000001000ULL;
        board.z   = 0b0000000000000000000000000000000000000000000000000000000010000001ULL;
        board.f   = 0b0000000000000000000000000000000000000000000000000000000000000000ULL;

        board.B   = 0b0000000011111111000000000000000000000000000000000000000000000000ULL;
        board.L = 0b0100001000000000000000000000000000000000000000000000000000000000ULL;
        board.X = 0b0010010000000000000000000000000000000000000000000000000000000000ULL;
        board.T   = 0b0000000000000000000000000000000000000000000000000000000000000000ULL;
        board.Q  = 0b0001000000000000000000000000000000000000000000000000000000000000ULL;
        board.K   = 0b0000000000000000000000000000000000000000000000000000000000000000ULL;

        board.Y   = 0b0000100000000000000000000000000000000000000000000000000000000000ULL;
        board.Z   = 0b1000000100000000000000000000000000000000000000000000000000000000ULL;
        board.F   = 0b0000000000000000000000000000000000000000000000000000000000000000ULL;

        board.schlagen=false;
    }

    int play() {
        int current = 1;
        while (true) {
            //-----------------------------------------
            HumanPlayer player_1(6, this->board);
            MinimaxPlayer3 player_2(-6, this->board);
            //-----------------------------------------
            std::cout<<this->turn<<std::endl;
            this->board.print_board();
            //
            Board new_move;
            Board* new_board;
            if (current==1) {
                std::cout <<"k ist am Zug"<<std::endl;
                new_board = player_1.get_move();
            }
            else if (current==2) {
                std::cout<<"K ist am Zug"<<std::endl;
                new_board = player_2.get_move();
            }
            //
            if (new_board != nullptr) {new_move = *new_board; delete new_board;}
            else {std::cout<<"NULLPOINTER2"<<std::endl; delete new_board;}
            //
            if (!(new_board==nullptr)) {this->board = new_move;}
            else {
                bool king_captured = false;
                int other;
                int this_players_token;
                //
                if(current == 1) {other = -6;}
                else {other = 6;}
                if (current == 1) {this_players_token = 6;}
                else {this_players_token = -6;}
                //
                std::vector<Board> children = this->board.generate_children(other);
                for (const Board& child : children) {
                    if (child.verloren(this_players_token)) {
                        king_captured = true;
                    }
                }
                if (!king_captured) {
                    this->board.print_board();
                    std::cout << "UNENTSCHIEDEN" << std::endl;
                    return 0;
                }
                else {
                    if (this_players_token == 6) {
                        this->board.print_board();
                        std::cout << "K HAT GEWONNEN" << std::endl;
                        return -1;
                    }
                    else {
                        this->board.print_board();
                        std::cout << "k HAT GEWONNEN" << std::endl;
                        return 1;
                    }
                }
            }
            //
            if (this->turn==max_turns) {this->board.print_board();; std::cout<<"UNENTSCHIEDEN"<<std::endl; return 0;}
            //
            if (current==1) {current = 2;}
            else {current = 1;}
            this->turn += 1;
        }
    }

private:
    
};

//

void spielen(int z) {
    std::cout<<"NEUES SPIEL"<<std::endl;
    int k_wins=0;
    int K_wins=0;
    int unentschieden=0;
    //
    for (int i=0; i<z; ++i) {
        Schach game;
        int r = game.play();
        if (r==1) {k_wins+=1;}
        else if (r==-1) {K_wins+=1;}
        else if (r==0) {unentschieden+=1;}
        std::cout<<"k: "<<k_wins<<std::endl;
        std::cout<<"K: "<<K_wins<<std::endl;
        std::cout<<"-: "<<unentschieden<<std::endl;
    }
    std::cout<<"FERTIG"<<std::endl;
}

//

int main() {
    srand(time(0)); //seed
    spielen(30);
    //test-------------------
    //Schach game;
    //game.board.print_board();
    //for (int i; i<10; ++i) {
        //std::vector<Board> bb=game.board.generate_one_random_child(6);
        //Board bbb=bb[0];
        //bbb.print_board();
    //}
    std::cout<<"FINISHED"<<std::endl;
    //test-----------------
}
