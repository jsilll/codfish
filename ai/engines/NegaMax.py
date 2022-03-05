import chess
from chess.polyglot import zobrist_hash, MemoryMappedReader
from random import randint

PIECES_VALUES_MG = {
    1: 100,
    2: 320,
    3: 330,
    4: 500,
    5: 900,
    6: 20000
}

position_values = {
    1:  ((0,  0,  0,  0,  0,  0,  0,  0),
         (50, 50, 50, 50, 50, 50, 50, 50),
         (10, 10, 20, 30, 30, 20, 10, 10, ),
         (5,  5, 10, 25, 25, 10,  5,  5),
         (0,  0,  0, 20, 20,  0,  0,  0),
         (5, -5, -10,  0,  0, -10, -5,  5),
         (5, 10, 10, -20, -20, 10, 10,  5),
         (0,  0,  0,  0,  0,  0,  0,  0)),

    2:  ((-50, -40, -30, -30, -30, -30, -40, -50),
         (-40, -20,  0,  0,  0,  0, -20, -40),
         (-30,  0, 10, 15, 15, 10,  0, -30),
         (-30,  5, 15, 20, 20, 15,  5, -30),
         (-30,  0, 15, 20, 20, 15,  0, -30),
         (-30,  5, 10, 15, 15, 10,  5, -30),
         (-40, -20,  0,  5,  5,  0, -20, -40),
         (-50, -40, -30, -30, -30, -30, -40, -50)),

    3:  ((-20, -10, -10, -10, -10, -10, -10, -20),
         (-10,  0,  0,  0,  0,  0,  0, -10),
         (-10,  0,  5, 10, 10,  5,  0, -10),
         (-10,  5,  5, 10, 10,  5,  5, -10),
         (-10,  0, 10, 10, 10, 10,  0, -10),
         (-10, 10, 10, 10, 10, 10, 10, -10),
         (-10,  5,  0,  0,  0,  0,  5, -10),
         (-20, -10, -10, -10, -10, -10, -10, -20)),

    4:  ((0,  0,  0,  0,  0,  0,  0,  0),
         (5, 10, 10, 10, 10, 10, 10,  5),
         (-5,  0,  0,  0,  0,  0,  0, -5),
         (-5,  0,  0,  0,  0,  0,  0, -5),
         (-5,  0,  0,  0,  0,  0,  0, -5),
         (-5,  0,  0,  0,  0,  0,  0, -5),
         (-5,  0,  0,  0,  0,  0,  0, -5),
         (0,  0,  0,  5,  5,  0,  0,  0)),

    5:  ((-20, -10, -10, -5, -5, -10, -10, -20),
         (-10,  0,  0,  0,  0,  0,  0, -10),
         (-10,  0,  5,  5,  5,  5,  0, -10),
         (-5,  0,  5,  5,  5,  5,  0, -5),
         (0,  0,  5,  5,  5,  5,  0, -5),
         (-10,  5,  5,  5,  5,  5,  0, -10),
         (-10,  0,  5,  0,  0,  0,  0, -10),
         (-20, -10, -10, -5, -5, -10, -10, -20)),

    6:  ((-30, -40, -40, -50, -50, -40, -40, -30),
         (-30, -40, -40, -50, -50, -40, -40, -30),
         (-30, -40, -40, -50, -50, -40, -40, -30),
         (-30, -40, -40, -50, -50, -40, -40, -30),
         (-20, -30, -30, -40, -40, -30, -30, -20),
         (-10, -20, -20, -20, -20, -20, -20, -10),
         (20, 20,  0,  0,  0,  0, 20, 20),
         (20, 30, 10,  0,  0, 10, 30, 20))

}


class Engine:
    def __init__(self, game_id, color):
        self.game_id = game_id
        self.color = color
        self.board = chess.Board()
        self.depth = 3
        self.positions = 0
        self.opening_moves = MemoryMappedReader("theory/book.bin")

    def eval_piece_static(self, piece: chess.Piece, square: chess.Square, color: bool):
        rank = 7 - \
            chess.square_rank(square) if color else chess.square_rank(square)
        file = chess.square_file(square)
        score = - 1 if (color != piece.color) else 1
        score *= PIECES_VALUES_MG[piece.piece_type] + \
            position_values[piece.piece_type][rank][file]
        return score

    def eval_board_static(self, color: bool):
        if not self.board.legal_moves:
            if (self.board.is_check()):
                return -1e8  # checkmate
            return 0  # stalemate
        return sum(
            self.eval_piece_static(self.board.piece_at(square), square, color)
            if self.board.piece_at(square) is not None else 0
            for square in chess.SQUARES)

    def eval_move(self, move: chess.Move, color):
        evaluation_estimate = 0
        moved_piece = self.board.piece_at(move.from_square)
        captured_piece = self.board.piece_at(move.to_square)

        # Prioritize capturing opponent's most
        # valuable pieces with our least valuabe pieces
        if (captured_piece != None):
            evaluation_estimate = 10 * \
                PIECES_VALUES_MG[captured_piece.piece_type] - \
                PIECES_VALUES_MG[moved_piece.piece_type]

        # Priotize promoting pawns
        if (move.promotion != None):
            evaluation_estimate += PIECES_VALUES_MG[move.promotion]

        # Penalize moving our pieces to a square attacked by an opponent pawn
        if (self.board.is_attacked_by(not color, move.to_square)):
            evaluation_estimate -= PIECES_VALUES_MG[moved_piece.piece_type]

        return evaluation_estimate

    def quiescence(self, alpha, beta, color: bool):
        """
            Too slow to be used
        """
        stand_pat = self.eval_board_static(color)
        if (stand_pat >= beta):
            return beta
        alpha = max(alpha, stand_pat)
        for move in sorted(filter(lambda move: self.board.is_capture(move) or self.board.gives_check(move) or move.promotion, self.board.legal_moves), key=lambda move: self.eval_move(move, color), reverse=True):
            self.board.push(move)
            score = - self.quiescence(-beta, -alpha, not color)
            self.board.pop()
            if (beta <= score):
                return beta
            alpha = max(alpha, score)
        return alpha

    def negamax(self, depth, alpha, beta, color):
        if depth == 0:
            # return self.quiescence(alpha, beta, color)
            return self.eval_board_static(color)
        for move in sorted(list(self.board.legal_moves), key=lambda move: self.eval_move(move, color), reverse=True):
            self.positions += 1
            self.board.push(move)
            local_score = - self.negamax(depth - 1, -beta, -alpha, not color)
            self.board.pop()
            alpha = max(alpha, local_score)
            if beta <= alpha:
                return alpha
        return alpha

    def search_root(self):
        best_evaluation = -1e8
        best_move = None
        for move in sorted(list(self.board.legal_moves), key=lambda move: self.eval_move(move, self.color), reverse=True):
            self.board.push(move)
            local_score = - \
                self.negamax(self.depth - 1, -1e8, 1e8, not self.color)
            self.board.pop()
            if local_score > best_evaluation:
                best_evaluation = local_score
                best_move = move
        return best_move

    def play(self):
        self.positions = 0
        if self.opening_moves:
            try:
                book_move = self.opening_moves.choice(self.board).move
                self.board.push(book_move)
                return book_move
            except IndexError:
                pass
        best_move = self.search_root()
        if best_move != None:
            self.board.push(best_move)
        return best_move
