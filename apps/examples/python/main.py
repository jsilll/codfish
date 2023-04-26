import chess

board = chess.Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1")

true_moves = set([move.uci() for move in board.legal_moves])

move_list = []

codfish_moves = set(move_list)

repeated = set([x for x in move_list if move_list.count(x) > 1])
if len(repeated) > 0:
    for move in repeated:
        print("Codfish move %s is repeated %d times!" % (move, move_list.count(move)))

print("True has %d unique moves" % len(true_moves))
print("Codfish has %d unique moves" % len(codfish_moves))

for move in codfish_moves:
    if move not in true_moves:
        print("Codfish move %s is not legal!" % move)

for move in true_moves:
    if move not in codfish_moves:
        print("Legal move %s is not in Codfish!" % move)