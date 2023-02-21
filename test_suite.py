#!/usr/bin/env python3
"""
Script for testing bots. Note that wins are reported as original player,
e.g. if swap happens and "Black" wins, it counts as a win for White.
"""
import argparse
import multiprocessing
import shlex
import subprocess
import time

RUST_PREAMBLE = "cargo run --bin central_controller --release -- matchup"
BOARD_SIZE = 10
BLACK_PATH = "../../hexbot/cpp_bot/main.out"
WHITE_PATH = "../../hexbot/cpp_bot/main.out"
RUN_TIME_SECS = 20


def play_games(args, black_wins, white_wins, start_time, run_time):
    while time.perf_counter() - start_time < float(run_time):
        p = subprocess.Popen(args,
                             stdin=subprocess.PIPE,
                             stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE,
                             universal_newlines=True,
                             bufsize=0)

        p.stdin.write("run " + str(BOARD_SIZE ** 2 + 1))  # swap move and fill board
        p.stdin.close()

        swap = 0
        for line in p.stdout:
            if "move: swap" in line:
                swap += 1
            elif ("Black has won" in line and swap % 2 == 0) or ("White has won" in line and swap % 2 == 1):
                with black_wins.get_lock():
                    black_wins.value += 1
                    p.kill()
            elif ("White has won" in line and swap % 2 == 0) or ("Black has won" in line and swap % 2 == 1):
                with white_wins.get_lock():
                    white_wins.value += 1
                    p.kill()


def main():
    parser = argparse.ArgumentParser(description="Test two bots.")
    parser.add_argument('-b', default=BLACK_PATH,
                        help="Executable path for black (first player)")
    parser.add_argument('-w', default=WHITE_PATH,
                        help="Executable path for white (second player)")
    parser.add_argument('-s', default=BOARD_SIZE,
                        help="Board size (side length)")
    parser.add_argument('-t', default=RUN_TIME_SECS,
                        help="Run time (seconds), script will not start new games after this time has elapsed")
    parser.add_argument('-l', default="",
                        help="Log path (will log win counts to path)")

    args = parser.parse_args()
    popen_args = shlex.split("{} {} {} {}".format(RUST_PREAMBLE, args.s, args.b, args.w))

    procs = []
    BLACK_WINS = multiprocessing.Value('i', 0)
    WHITE_WINS = multiprocessing.Value('i', 0)

    start_time = time.perf_counter()

    for cpu in range(0, multiprocessing.cpu_count()):
        proc = multiprocessing.Process(target=play_games, args=(popen_args, BLACK_WINS, WHITE_WINS, start_time, args.t))
        procs.append(proc)
        proc.start()
    for proc in procs:
        proc.join()

    print("BLACK WIN", BLACK_WINS.value, "WHITE WIN", WHITE_WINS.value)
    if args.l != "":
        f = open(args.l, 'w', encoding="utf-8")
        f.write("BLACK WIN {} WHITE WIN {}\n".format(BLACK_WINS.value, WHITE_WINS.value))
        f.close()


if __name__ == "__main__":
    main()
