import os
import time
import threading
import berserk
from chess import Board
from datetime import datetime, tzinfo


class Game(threading.Thread):
    def __init__(self, client: berserk.Client, game_id, player_id, strategy, **kwargs):
        super().__init__(**kwargs)
        info = client.games.export(game_id)
        white_id = info["players"]["white"]["user"]["id"]
        black_id = info["players"]["black"]["user"]["id"]

        self.client = client
        self.game_id = game_id
        self.player_id = player_id
        self.opponent_id = black_id if white_id == player_id else white_id

        # brains of the operation
        self.waiting = False
        self.engine = strategy(self.game_id, white_id == player_id)

        print(f"[{self.game_id}] Game Started: {white_id} vs {black_id}")

    def run(self):
        for event in self.client.bots.stream_game_state(self.game_id):
            if event["type"] == "gameFull":  # we just joined a game
                if (datetime.now(tz=event["createdAt"].tzinfo) - event["createdAt"]).total_seconds() >= (10 * 60):
                    print(f"[{self.game_id}] Aborting Game")
                    try:
                        self.client.bots.abort_game(self.game_id)
                    except berserk.exceptions.ResponseError:
                        self.client.bots.resign_game(self.game_id)

                    try:
                        os.remove(f"data/cache_[{self.game_id}]")
                    except OSError:
                        pass

                    return
                self.send_chat("Thanks for playing!")
                self.current_game_state = None
                self.handle_state_change(event["state"])
            elif event["type"] == "gameState":
                self.handle_state_change(event)
            elif event["type"] == "chatLine":
                self.handle_chat_line(event)

    def handle_state_change(self, game_state):
        if game_state["status"] != "started":
            print(f"[{self.game_id}] Game Over: " + game_state["status"])
            return
        all_moves = game_state["moves"].split()
        if (len(all_moves) + 1) % 2 == (1 if self.engine.color else 0):
            if not len(self.engine.board.move_stack):
                for move in all_moves:
                    self.engine.board.push_uci(move)
            else:
                self.engine.board.push_uci(all_moves[-1])
            self.play_move()

    def handle_chat_line(self, chat_line):
        if (chat_line["username"] != self.player_id):
            msg = chat_line["username"] + ": " + chat_line["text"]
            print(f"[{self.game_id}]", msg)

    def play_move(self):

        start = time.time()
        move = self.engine.play()
        end = time.time()

        if move == None:
            self.client.bots.resign_game(self.game_id)
            return

        try:
            self.client.bots.make_move(self.game_id, move.uci())
        except (berserk.exceptions.ResponseError, berserk.exceptions.ApiError) as e:
            time.sleep(1)
            if e is berserk.exceptions.ApiError:
                self.play_move()
        print(f"[{self.game_id}] Sending move: {move.uci()}, {round(end-start, 2)}s")

    def send_chat(self, chat_line):
        print(
            f"[{self.game_id}] Sending player {self.opponent_id} message: {chat_line}")
        self.client.bots.post_message(self.game_id, chat_line)
