# Chess-Engine
### Bit-board data structure is used to store the chess board
<p align="center">
<img width="650" height="650" alt="ChessBoard" src="https://github.com/user-attachments/assets/a276216d-23ef-489e-9d62-67c4fdd53bb4" />
</p>
<br>
The bitboard method stores the chessboard as a 64-bit integer, where each bit represents a square (1 = piece present, 0 = empty).
Separate bitboards are maintained for each piece type and color (e.g., white pawns, black knights).
This allows extremely fast operations using bitwise logic for move generation, attacks, and occupancy. 
