import chess

board = chess.Board()

true_moves = set([move.uci() for move in board.legal_moves])

codfish_moves = set(["b2a1", "c2a1", "d2a1", "g2a1", "a2a3", "b2b3", "c2c3", "d2d3", "e2e3", "f2f3", "g2g3", "h2h3", "b1a3", "b1c3", "g1a3", "g1f3", "g1h3"])

for move in codfish_moves:
    if move not in true_moves:
        print("Codfish move %s is not legal!" % move)

for move in true_moves:
    if move not in codfish_moves:
        print("Legal move %s is not in Codfish!" % move)