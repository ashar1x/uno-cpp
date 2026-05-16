# 🃏 UNO - Terminal Card Game

A fully playable 2-player UNO game that runs entirely in the terminal. Built in C++ as a first-semester project with no frameworks, no libraries, just raw logic and a lot of pointer headaches.

---

## What is this?

It's UNO. In your terminal. With color.

Two players take turns on the same machine, playing cards, calling UNO, and occasionally rage-quitting. The entire game (deck, hands, special cards, win tracking) is handled from scratch using arrays and file I/O.

---

## Features

- **Full UNO deck** with all four colors, numbered cards 0-9, and every special card
- **Special cards that actually work** (Skip, Reverse, Draw +2, Wild, Wild Draw +4)
- **UNO call system** where you call it at the right time or get penalized with 2 extra cards
- **Color-coded terminal output** where each card shows in its actual color using Windows console attributes
- **Persistent scoreboard** that saves your win/loss record to a file and reloads it next session
- **Pause and exit mid-game** because sometimes you need a break

---

## How to Run

> Windows only. The game uses `windows.h` for colored output and `conio.h` for input handling so it won't compile on Linux or Mac.

**Compile:**
```bash
g++ uno.cpp -o uno
```

**Run:**
```bash
./uno
```

No external dependencies. Just a C++ compiler and a Windows terminal.

---

## How to Play

- Press `P` to start a game, `S` to view the scoreboard
- On your turn, choose to play a card or draw a card
- Match the color or number of the top card to play
- Wild cards can be played anytime
- Press `U` to call UNO when you're down to 2 cards or you get penalized
- First player to empty their hand wins

---

## Project Structure

```
uno.cpp            # everything: deck, game loop, UI, file I/O
winlossrecord.txt  # auto-generated, stores cumulative win counts
```

---

## Notes

This was written in the first semester of a BS Software Engineering degree so yes, the code has `goto` statements and global arrays. Zero regrets. It works, it's fun, and it taught more about C++ fundamentals than any textbook chapter could.

---

## License

Feel free to use, modify, or learn from this. A credit would be nice but isn't required.
