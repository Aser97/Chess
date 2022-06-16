# Applying SARSA and Monte Carlo Reinforcement Algorithms for Chess

This project basically implements the two algorithms (SARSA and Monte Carlo) to approximate the best policy against an opponent in the game of Chess.
 
The AI has been training whith Stockfish at Elo 1350

## Executable

This project is delivered with an executable. It only runs on **macOs** for now. In this executable, you will have 4 options:
- **Play with White**: You will be playing agains the AI with white pieces
- **Play with Black**: You will be playing agains the AI with black pieces
- **Play Stockfish White**: You will be playing against Stockfish with white pieces
- **Play Stockfish Black**: You will be playing against Stockfish with black pieces

##### Dealing with new positions
The positions tables have been encoded using Zobrist hashing method. When the AI reaches an unknown position, it simulates a bunch of games against Stockfish in this position, and then makes a move with the acquired knowledge. For this reason, it might take a few second to make some moves
