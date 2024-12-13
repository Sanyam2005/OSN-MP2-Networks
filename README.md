

### **Networking Task 1: Multiplayer Tic-Tac-Toe Game (15 points)**

#### **Overview**
- **Server Responsibilities**: 
  - Manage the game state (Tic-Tac-Toe board).
  - Handle communication between players.
  - Enforce game rules, including turn-taking and move validation.
  - Determine the winner or draw outcome.

- **Client Responsibilities**:
  - Allow players to send their moves to the server.
  - Display the board state and outcomes in a user-friendly format.
  - Notify the server of readiness and game continuation decisions.

---

#### **Game Rules and Flow**
1. **Initial Setup**:
   - The server initializes an empty 3x3 grid.
   - Players connect to the server. Once both are ready, Player 1 is assigned "X" and Player 2 is assigned "O".

2. **Gameplay**:
   - Players alternate turns. The server ensures proper turn order.
   - Moves specify a grid position (e.g., `row col` for top-left as `1 1`).
   - Invalid moves are rejected by the server, prompting the player to try again.
   - The server updates the board and broadcasts the new state to both clients.
   - Each client displays the updated board and receives instructions for the next turn.

3. **Winning and Drawing**:
   - Win conditions: Three matching symbols in a row, column, or diagonal.
   - Draw: All positions filled without a winner.
   - The server announces the result (e.g., "Player 1 Wins!", "It's a Draw!") and displays the final board.

4. **After the Game**:
   - Players decide if they want to replay.
   - If both agree, the server resets the board and starts a new game.
   - If either declines, both connections are terminated.

#### **Implementation**
- **TCP Version**:
  - Reliable communication between clients and server.
  - Ensure strict sequencing and proper flow control.

- **UDP Version**:
  - Use acknowledgments and retries to handle packet loss or out-of-order delivery.
  - Ensure game state consistency despite UDP's unreliability.

---

### **Networking Task 2: Simulated TCP Over UDP (15 points)**

#### **Functionalities**
1. **Data Sequencing**:
   - Split data into fixed-size chunks or a fixed number of chunks.
   - Each chunk includes:
     - Sequence number.
     - Total number of chunks.
   - Receiver reassembles the data in sequence and displays the complete text.

2. **Retransmissions**:
   - Sender waits for ACKs from the receiver.
   - If an ACK for a specific chunk is not received within 0.1 seconds, the sender retransmits that chunk.
   - Sender continues transmitting other chunks while waiting for acknowledgments (pipelining).
   - **Testing Retransmissions**: Simulate missing ACKs (e.g., skip every third ACK) to test the retransmission mechanism. Remove this simulation in the final submission.

---

#### **Implementation Notes**
- **Server and Client**:
  - Both act as senders and receivers to simulate bidirectional communication.
  - Use structs for packing and unpacking data chunks and ACKs.

- **Design Choices**:
  - **Threads**: Optional, can simplify handling simultaneous sending and receiving.
  - **Non-blocking Sockets**: Alternative for asynchronous behavior without threads.

#### **Expected Workflow**:
1. **Sender**:
   - Divides data into chunks.
   - Sends each chunk along with sequence number and total chunk count.
   - Tracks ACKs and retransmits missing chunks after a timeout.

2. **Receiver**:
   - Receives chunks and sends ACKs referencing the sequence numbers.
   - Reassembles data in sequence.
   - Skips ACKs (for testing retransmission) and comments out this feature in the final submission.

---

### **Deliverables**
- **Part A**: Multiplayer Tic-Tac-Toe game using both TCP and UDP.
- **Part B**: Simulated TCP functionalities over UDP, including data sequencing and retransmissions.
- **Submission Directory**: `<mini-project2-directory>/networks/partB`.

