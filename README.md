# Chess-Engine
### Bit-board data structure is used to store the chess board
<p align="center">
<img width="600" height="600" alt="ChessBoarddrawio" src="https://github.com/user-attachments/assets/35c3fc05-6154-4ac9-b989-862a6577d2e1"/>
</p>
<br>
The bitboard method stores the chessboard as a 64-bit integer, where each bit represents a square (1 = piece present, 0 = empty).
Separate bitboards are maintained for each piece type and color (e.g., white pawns, black knights).
This allows extremely fast operations using bitwise logic for move generation, attacks, and occupancy. 
