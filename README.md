---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults

layout: page
title: Reinforcement Learning for Chess
subtitle: SARSA & Monte Carlo Algorithms
#gallery: ot-gallery
toc: true
---


This project implements SARSA and Monte Carlo reinforcement learning algorithms to approximate an optimal policy for playing chess against an opponent.

## Theoretical Background
For a deeper understanding of the algorithms used, refer to the following research papers:

<div markdown="0">
  <a href="https://arxiv.org/pdf/1902.02234">
    <div class="preview-container">
      <img src="{{ site.baseurl }}/assets/thumbnails/SARSA_thumbnail.PNG" alt="Preview of the document"/>
      <div class="hover-effect">Click to view</div>
    </div>
  </a>
</div>

<div markdown="0">
  <a href="https://arxiv.org/pdf/2206.12674">
    <div class="preview-container">
      <img src="{{ site.baseurl }}/assets/thumbnails/MC_thumbnail.PNG" alt="Preview of the document"/>
      <div class="hover-effect">Click to view</div>
    </div>
  </a>
</div>
The AI model has been trained using Stockfish at an Elo rating of 1350.

## Executable

This project comes with a ready-to-run executable, primarily developed for **macOs** in C++, but it should be compatible with any **Unix**-based system.
The full code is [here](https://github.com/Aser97/Chess.git)

### Available Gameplay Modes
Upon running the executable, you can choose from the following options:

- **Play as White**: Face the AI while controlling the white pieces
- **Play as Black**: Face the AI while controlling the black pieces
- **Play Stockfish as White**: Play against Stockfish while controlling the white pieces
- **Play Stockfish as Black**: Play against Stockfish while controlling the black pieces

## Handling New Positions
The AI utilizes Zobrist hashing to encode board positions efficiently. When encountering an unfamiliar position, it simulates multiple games against Stockfish to analyze the position and determine the best move. This process may cause a slight delay before making a move, particularly in novel board states
