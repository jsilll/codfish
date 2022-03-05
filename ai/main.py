import berserk
import engines.NegaMax
from game import Game


def valid_challenge(challenge):
    if challenge["variant"]["key"] != "standard":
        return False
    # if challenge["rated"] != False:
    #     return False
    return True


def start_lichess_bot_client(api_token):
    session = berserk.TokenSession(api_token)
    return berserk.Client(session)


def get_account_public_info(account):
    try:
        return account.get()
    except berserk.exceptions.ResponseError as e:
        print("Exception:", e)
        return


if __name__ == "__main__":
    client = start_lichess_bot_client("API_TOKEN_HERE")
    pub_info = get_account_public_info(client.account)

    assert pub_info['title'] == "BOT"
    player_id = pub_info["id"]

    print("Connected to Lichess as:", player_id)

    for event in client.bots.stream_incoming_events():
        if event["type"] == "challenge":
            if valid_challenge(event["challenge"]):
                try:
                    client.bots.accept_challenge(event["challenge"]["id"])
                except berserk.exceptions.ResponseError:
                    pass
            else:
                try:
                    client.bots.decline_challenge(event["challenge"]["id"])
                except berserk.exceptions.ResponseError:
                    pass
        elif event["type"] == "gameStart":
            game = Game(client, event["game"]["id"],
                        player_id, engines.NegaMax.Engine)
            game.start()
