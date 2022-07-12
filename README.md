chess-engine
===
C++ Chess engine that uses bitboard representation.

Build Dependencies
===
- C++ Standard Library
- Catch2 (header included) for Unit Tests

Engine Features
===

### Interfaces
- UCI Interface
- CLI Mode for Humans

---

### Move Generation
- Bitboard Representation
- Pre-calculated Attack Tables
- Magic Bitboards
- Pseudo-Legal Move Generation
- Copy Make Approach

---

### Move Picker
- PeSTO's Evaluation Function by Ronald Friederich
- NegaMax Search
- Quiescense Search with Null Move Pruning
- Move Sorting with PV, Killer, History and MVV LVA moves
- Triangular PV-Table
- Iterative Deepening
- Principal Variation Search
- Late Move Reduction