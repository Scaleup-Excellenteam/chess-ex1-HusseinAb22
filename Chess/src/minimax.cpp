#include "minimax.h"
#include "Board.h"
#include "Piece.h"
#include "PriorityQueue.h"
#include "GameValidator.h"
#include "ThreadPool.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <limits>
#include <future>
#include <algorithm>

using namespace std;

static map<char, int> piece_value = {
    {'p', 100}, {'n', 320}, {'b', 330}, {'r', 500}, {'q', 900}, {'k', 20000},
    {'P', 100}, {'N', 320}, {'B', 330}, {'R', 500}, {'Q', 900}, {'K', 20000}
};

const int CHECKMATE_SCORE = 100000;
const int STALEMATE_SCORE = 0;

const int pawn_pst[8][8] = {
    {0,  0,  0,  0,  0,  0,  0,  0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    {5,  5, 10, 25, 25, 10,  5,  5},
    {0,  0,  0, 20, 20,  0,  0,  0},
    {5, -5,-10,  0,  0,-10, -5,  5},
    {5, 10, 10,-20,-20, 10, 10,  5},
    {0,  0,  0,  0,  0,  0,  0,  0}
};
const int knight_pst[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};
const int bishop_pst[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};
const int rook_pst[8][8] = {
    {0,  0,  0,  0,  0,  0,  0,  0},
    {5, 10, 10, 10, 10, 10, 10,  5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {0,  0,  0,  5,  5,  0,  0,  0}
};
const int queen_pst[8][8] = {
    {-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    {-5,  0,  5,  5,  5,  5,  0, -5},
    {0,  0,  5,  5,  5,  5,  0, -5},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20}
};
const int king_pst_midgame[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    {20, 20,  0,  0,  0,  0, 20, 20},
    {20, 30, 10,  0,  0, 10, 30, 20}
};

bool is_in_check(Board& board_obj, bool for_white_player) {
    int king_r = -1, king_c = -1;
    char king_char_to_find = for_white_player ? 'K' : 'k';

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board_obj.boardMove[r][c];
            if (p && p->getPiece() == king_char_to_find) {
                king_r = r;
                king_c = c;
                break;
            }
        }
        if (king_r != -1) break;
    }

    if (king_r == -1) return false;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board_obj.boardMove[r][c];
            if (p && p->getColor() != for_white_player) {
                if (p->isLegalMove(r, c, king_r, king_c, board_obj.boardMove)) {
                    return true;
                }
            }
        }
    }
    return false;
}

int evaluateBoard(Board& board_obj) {
    int score = 0;
    int white_pawns_on_file[8] = {0};
    int black_pawns_on_file[8] = {0};
    int white_bishops = 0;
    int black_bishops = 0;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board_obj.boardMove[r][c];
            if (!p) continue;

            char piece_char = p->getPiece();
            int material_val = piece_value[piece_char];
            bool is_white_piece = p->getColor();

            score += is_white_piece ? material_val : -material_val;

            int pst_row = is_white_piece ? r : 7 - r;
            int pst_col = c;

            if (piece_char == 'P') { score += pawn_pst[pst_row][pst_col]; white_pawns_on_file[c]++; }
            else if (piece_char == 'p') { score -= pawn_pst[pst_row][pst_col]; black_pawns_on_file[c]++; }
            else if (piece_char == 'N') score += knight_pst[pst_row][pst_col];
            else if (piece_char == 'n') score -= knight_pst[pst_row][pst_col];
            else if (piece_char == 'B') { score += bishop_pst[pst_row][pst_col]; white_bishops++; }
            else if (piece_char == 'b') { score -= bishop_pst[pst_row][pst_col]; black_bishops++; }
            else if (piece_char == 'R') score += rook_pst[pst_row][pst_col];
            else if (piece_char == 'r') score -= rook_pst[pst_row][pst_col];
            else if (piece_char == 'Q') score += queen_pst[pst_row][pst_col];
            else if (piece_char == 'q') score -= queen_pst[pst_row][pst_col];
            else if (piece_char == 'K') score += king_pst_midgame[pst_row][pst_col];
            else if (piece_char == 'k') score -= king_pst_midgame[pst_row][pst_col];

            // Rook on open/semi-open file
            if (piece_char == 'R' || piece_char == 'r') {
                bool friendly_pawn_on_file = false;
                bool enemy_pawn_on_file = false;
                for (int scan_r = 0; scan_r < 8; ++scan_r) {
                    Piece* file_pawn = board_obj.boardMove[scan_r][c];
                    if (file_pawn) {
                        if (is_white_piece && file_pawn->getPiece() == 'P') friendly_pawn_on_file = true;
                        else if (!is_white_piece && file_pawn->getPiece() == 'p') friendly_pawn_on_file = true;
                        else if (is_white_piece && file_pawn->getPiece() == 'p') enemy_pawn_on_file = true;
                        else if (!is_white_piece && file_pawn->getPiece() == 'P') enemy_pawn_on_file = true;
                    }
                }
                if (!friendly_pawn_on_file) {
                    score += is_white_piece ? 10 : -10;
                    if (!enemy_pawn_on_file) {
                        score += is_white_piece ? 15 : -15;
                    }
                }
            }
        }
    }


    for(int i=0; i<8; ++i) {
        if (white_pawns_on_file[i] > 1) score -= (white_pawns_on_file[i] - 1) * 20;
        if (black_pawns_on_file[i] > 1) score += (black_pawns_on_file[i] - 1) * 20;
    }

    if (white_bishops >= 2) score += 50;
    if (black_bishops >= 2) score -= 50;

    return score;
}

vector<string> generateLegalMoves(Board& board_obj, bool for_white_player) {
    vector<string> moves;
    string original_board_str = board_obj.getBoardString();

    for (int src_row = 0; src_row < 8; ++src_row) {
        for (int src_col = 0; src_col < 8; ++src_col) {
            Piece* piece = board_obj.boardMove[src_row][src_col];
            if (piece && piece->getColor() == for_white_player) {
                for (int dest_row = 0; dest_row < 8; ++dest_row) {
                    for (int dest_col = 0; dest_col < 8; ++dest_col) {
                        if (src_row == dest_row && src_col == dest_col) continue;

                        string move_str = string(1, 'a' + src_row) + string(1, '1' + src_col) +
                                          string(1, 'a' + dest_row) + string(1, '1' + dest_col);

                        string temp_board_str_for_validation = original_board_str;

                        int result = validateMove(move_str, temp_board_str_for_validation, for_white_player, true);

                        if (result >= 41) {
                            moves.push_back(move_str);
                        }
                    }
                }
            }
        }
    }
    return moves;
}

int minimax(Board& board_state, int depth, int alpha, int beta, bool maximizing_player_is_white) {
    if (depth == 0) {
        return evaluateBoard(board_state);
    }

    vector<string> legal_moves = generateLegalMoves(board_state, maximizing_player_is_white);

    if (legal_moves.empty()) {
        if (is_in_check(board_state, maximizing_player_is_white)) {
            return maximizing_player_is_white ? -CHECKMATE_SCORE : CHECKMATE_SCORE;
        } else {
            return STALEMATE_SCORE;
        }
    }

    if (maximizing_player_is_white) {
        int max_eval = -CHECKMATE_SCORE -1;
        for (const string& move : legal_moves) {
            string board_str_before_move = board_state.getBoardString();
            string board_str_after_move = board_str_before_move;

            int validation_code = validateMove(move, board_str_after_move, true, true);
            if (validation_code < 41) continue;

            Board next_board_state;
            next_board_state.loadFromString(board_str_after_move);

            int eval = minimax(next_board_state, depth - 1, alpha, beta, false);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return max_eval;
    } else {
        int min_eval = CHECKMATE_SCORE + 1;
        for (const string& move : legal_moves) {
            string board_str_before_move = board_state.getBoardString();
            string board_str_after_move = board_str_before_move;

            int validation_code = validateMove(move, board_str_after_move, false, true);
            if (validation_code < 41) continue;

            Board next_board_state;
            next_board_state.loadFromString(board_str_after_move);

            int eval = minimax(next_board_state, depth - 1, alpha, beta, true);
            min_eval = min(min_eval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return min_eval;
    }
}

vector<pair<int, string>> getBestMoves(string current_board_str, bool player_is_white, int depth, int num_threads) {
    Board initial_board;
    initial_board.loadFromString(current_board_str);
    vector<string> candidate_moves = generateLegalMoves(initial_board, player_is_white);

    PriorityQueue<pair<int, string>> pq;

    auto evaluate_move_task =
        [&pq, current_board_str, player_is_white, depth](const string& move_to_evaluate) {
        string board_str_after_move = current_board_str;

        int validation_result = validateMove(move_to_evaluate, board_str_after_move, player_is_white, true);

        if (validation_result >= 41) {
            Board board_after_player_moves;
            board_after_player_moves.loadFromString(board_str_after_move);

            int score_from_white_perspective = minimax(board_after_player_moves, depth - 1,
                                                       -CHECKMATE_SCORE -100,
                                                       CHECKMATE_SCORE + 100,
                                                       !player_is_white);

            int move_score_for_this_player;
            if (player_is_white) {
                move_score_for_this_player = score_from_white_perspective;
            } else {
                move_score_for_this_player = -score_from_white_perspective;
            }
            pq.push({move_score_for_this_player, move_to_evaluate});
        }
    };

    if (num_threads > 1 && candidate_moves.size() > 1) {
        ThreadPool pool(num_threads);
        vector<future<void>> results;
        for (const auto& mv : candidate_moves) {
            results.emplace_back(pool.enqueue(evaluate_move_task, mv));
        }
        for (auto&& result : results) {
            try {
                result.get();
            } catch (const std::exception& e) {
                cerr << "Exception in thread: " << e.what() << endl;
            }
        }
    } else {
        for (const auto& mv : candidate_moves) {
            evaluate_move_task(mv);
        }
    }

    return pq.to_vector();
}
