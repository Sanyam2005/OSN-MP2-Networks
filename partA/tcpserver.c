#include "header.h"
#include <stdio.h>
//cd Desktop/osn/mini-project-2-Sanyam2005/networks/partA
char m[3][3];

void initialize(){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            m[i][j] =' ';
        }
    }
}
int gamestatus(){
    // checking if any row has 3 X or O
   for (int i = 0; i < 3; i++) {
        if (m[i][0] == m[i][1] && m[i][1] == m[i][2] && m[i][0] != ' '){
            if(m[i][0]=='X')
            return 1;
            else
            return 2;
        }
       
    }
     // checking if any column has 3 X or O
    for (int i = 0; i < 3; i++){
        if (m[0][i] == m[1][i] && m[1][i] == m[2][i] && m[0][i] != ' '){
            if(m[i][0]=='X')
            return 1;
            else
            return 2;
        }
    }
    //checking for diagonals
     if (m[0][0] == m[1][1] && m[1][1] == m[2][2] && m[0][0] != ' '){
        if(m[0][0]=='X')
        return 1;
        else
        return 2;
     }
    if (m[0][2] == m[1][1] && m[1][1] == m[2][0] && m[0][2] != ' '){
        if(m[0][2]=='X')
        return 1;
        else
        return 2;
    }
     for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
             if (m[i][j] == ' ') return 0;  // Game is still ongoing
        }      
     }
    return 3;  // Draw
       
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    int serverSock;
    char buffer[bufsize];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    // creating a socket AF_INET for IPv4
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // defining the server address
    serv_addr.sin_family = AF_INET; //server used ipv4
    serv_addr.sin_port = htons(atoi(argv[1]));//port number of server
    serv_addr.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY is a special value. Server can accept connections from clients running on the same machine or on any other machine that can reach it over the network.

    // binding socket to address
    if (bind(serverSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }
    // listening for connection from clients 
    listen(serverSock, MAX_QUEUE);

    printf("WELCOME TO THE GAME OF TIC-TAC -TOE(SERVER END)!\n");

    // accepting connection1
    int clientSocket1 = accept(serverSock, NULL, NULL); //CHECK WHY null
    printf("Player 1 has connected\n");
    // accepting connection2
    int clientSocket2 = accept(serverSock, NULL, NULL);
     printf("Player 2 has connected\n");

    while(1){
    char ms[bufsize];
    strcpy(ms,"\nDo You want to start the game: ");
    send(clientSocket1,ms,strlen(ms),0);
    send(clientSocket2,ms,strlen(ms),0);
    char wantplayer1[bufsize];
    recv(clientSocket1, wantplayer1, sizeof(wantplayer1), 0);
    char wantplayer2[bufsize];
    recv(clientSocket2, wantplayer2, sizeof(wantplayer2), 0);
    //printf("%s %sd",wantplayer1,wantplayer2);
    if(strstr(wantplayer1,"no")&&strstr(wantplayer2,"no")) break;
    else if(strstr(wantplayer1,"yes")&&strstr(wantplayer2,"no")){
        strcpy(ms,"Player 2 refuses to play\n");
        send(clientSocket1,ms,strlen(ms),0);
        break;
    }
    else if(strstr(wantplayer1,"no")&&strstr(wantplayer2,"yes")){
        strcpy(ms,"Player 1 refuses to play\n");
        send(clientSocket2,ms,strlen(ms),0);
        break;
    }
    else if(!strstr(wantplayer1,"yes")||!strstr(wantplayer2,"yes")){
        strcpy(ms,"Invalid input by a player\n");
        send(clientSocket1,ms,strlen(ms),0);
        send(clientSocket2,ms,strlen(ms),0);
        break;
    }
    //initialize the players
    char msg1[30]="You are Player 1 (X)\n";
    char msg2[30]="You are Player 2(O)\n";
    send(clientSocket1,msg1,strlen(msg1),0);
    send(clientSocket2,msg2,strlen(msg2),0);
    
    //start the game
    int status = 0; //tracks the status of game 0->ongoing 1->player 1 win 2->player 2 win 3->draw
    int turn = 1;
    char str[bufsize];
    initialize();
     printf("Starting new game\n");
    while (status==0)
    {   
        int currsocket ;
        if(turn==1){
            currsocket = clientSocket1;
        }
        else{
             currsocket = clientSocket2;
        }
        //receive the move from connected client-> move stored in data
        strcpy(buffer,"Enter your move (row and column): ");
        send(currsocket,buffer,strlen(buffer),0);
       
        char data[bufsize];
        recv(currsocket, data, sizeof(data), 0);
       printf("Player %d has made move : %s\n",turn, data);

       //extract the row and column number from received message
        char *token;
        int row, col;
        token = strtok(data, " \n");
        if (token != NULL) {
            row = atoi(token);
            //printf("%d",row);
            token = strtok(NULL, " ");
            if (token != NULL) {
                col = atoi(token);
            }
            else{
                send(currsocket, "Invalid move. Try again.\n", 25, 0);
                continue;

            }
        } else {
            send(currsocket, "Invalid move. Try again.\n", 25, 0);
            continue;
        }
        // Get the second token (col)
        
        row--;
        col--;
        //printf("%d %d",row,col);
        //check validity
        bool valid=false;
        if(row>=0&&row<3&&col>=0&&col<3){
            if(m[row][col]==' ') valid=true;
        }
        //add the move if valid
        if(valid){
            if(turn==1){
                m[row][col] = 'X';
            }
            else if(turn==2){
                m[row][col] = 'O';
            }
            //print the current m contnets on both clients
            char board_str[bufsize] = "\n";
            for (int i = 0; i < 3; i++) {
                char row[20];
                snprintf(row, sizeof(row), " %c | %c | %c \n", m[i][0], m[i][1], m[i][2]);
                strncat(board_str, row, sizeof(board_str) - strlen(board_str) - 1);
                if (i < 2) {
                    strncat(board_str, "---+---+---\n", sizeof(board_str) - strlen(board_str) - 1);
                }
            }
            send(clientSocket1, board_str, strlen(board_str), 0);
            send(clientSocket2, board_str, strlen(board_str), 0);

             status =gamestatus();
            char suc[1024];
            if(status==0){
                if(turn==1)turn=2;
                else turn =1;
                continue;
            }
            
            else if(status==1){
                strcpy(suc,"\nPlayer 1 wins\nGAME OVER!!\n");
                send(clientSocket1,suc,strlen(suc),0);
                send(clientSocket2,suc,strlen(suc),0);
                break;
            }
            else if(status==2){
                strcpy(suc,"\nPlayer 2 wins!!\nGAME OVER!!\n");
                send(clientSocket1,suc,strlen(suc),0);
                send(clientSocket2,suc,strlen(suc),0);
                break;
            }
            else if(status==3){
                strcpy(suc,"\nIt is a Draw\nGAME OVER!!\n");
                send(clientSocket1,suc,strlen(suc),0);
                send(clientSocket2,suc,strlen(suc),0);
                break;
            }
        }
        else{
            int  size = strlen("\nInvalid move. Try again.\n");
            send(currsocket, "\nInvalid move. Try again.\n", size, 0);
        }

        
        
    }
    }
    // closing server socket
    char closing[1024];
    strcpy(closing,"Ending the Game....\n");
    send(clientSocket1,closing,strlen(closing),0);
    send(clientSocket2,closing,strlen(closing),0);
    printf("%s",closing);
    recv(clientSocket1, closing, sizeof(closing), 0);
       printf("%s",closing);
     recv(clientSocket1, closing, sizeof(closing), 0);
        printf("%s",closing);
    close(clientSocket1);
    close(clientSocket2);
    close(serverSock);
    return 0;
}