#include <iostream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//programa cliente
int main(int argc, char *argv[]){
  	int MeuSocket; //socket do cliente
    int serverAddrLen; //tamanho do endereço do servidor
    int option,loop;
  	char* peerHost = (char*) "localhost"; //host
    char message[1024];
  	short peerPort = 8080;
  	char buffer[1024]; //buffer que armazena mensagens recebidas
  	struct sockaddr_in server; //servidor
  
    if(argc > 1 && *(argv[1]) == '-') return -1;
 
 		//Meusocket = sozket(familia_de_protocolos: AF_INET = ARPA, tipo_de_socket_buscado, num_protocolo. 0 = Internet Protocol. 6 = TCP. 17 = UDP.)
    MeuSocket = socket(AF_INET, SOCK_STREAM, 6);
    if (MeuSocket < 0){
      return -1;
    }
  
 		//Associação com o IP
    bzero(&server, sizeof(server)); 

    //muda peerHost se tiver algum na entrada, caso contrario usa o padrão
    if(argc > 1) peerHost = argv[1];
 
    //Conversão de nome simbólico para IP
    struct hostent *host = gethostbyname(peerHost);
    if(host == NULL) return -1;
 
    server.sin_family = AF_INET;
  
    if(argc >= 3) peerPort = (short) atoi(argv[2]);
    server.sin_port = htons(peerPort);
 
    //Escrita do IP
    memmove(&(server.sin_addr.s_addr), host->h_addr_list[0], 4);
 
    //Conexão ao servidor remoto
  	
  	if(connect(MeuSocket, (struct sockaddr*) &server, sizeof(server)) < 0){
    	//erro
      return -1;
    }
  
  	if(read(MeuSocket, buffer, 1024) < 0){
      //erro
    	return -1;
    }
    
    std::cout << "Received:" << "\n" << buffer;
 
  	//Envio de mensagem para o servidor
    //write(MeuSocket, "Thanks! Bye-bye...\r\n", 20);
    loop = 1;
    while(loop) {
      //system("clear");
      printf("Seja bem vindo(a) a Cookies, nossa loja de doces virtual.");
      printf("Options:\n 1-Cardápio\n 2-Compra\n 0-Saída\n");
      scanf("%d", &option);
      switch(option) {
        case 0://opcao de fechar o programa
          write(MeuSocket, "Option 2", 9);
          break;

        case 1://opcao de exibir itens
          printf("Temos bolos, tortas, cookies, brigadeiros, beijinhos. Digite 2 para realizar uma compra, ou 0 para sair.\n");
          write(MeuSocket, "Option 0", 9);
          break;

        case 2://opcao de realizar compra
          int produto;
          write(MeuSocket, "Option 1", 9);
          printf("Faça sua compra. Digite o produto que deseja comprar.\n");
          printf(" 1-Bolos\n 2-Tortas\n 3-Cookies\n 4-Brigadeiros\n 5-Beijinhos\n");
            scanf("%d", &produto);
            switch(produto){
              case 1:
                write(MeuSocket, "bolos", 6);
                break;
              case 2:
                write(MeuSocket, "tortas", 7);
                break;
              case 3:
                write(MeuSocket, "cookies", 8);
                break;
              case 4:
                write(MeuSocket, "brigadeiros", 12);
                break;
              case 5:
                write(MeuSocket, "beijinhos", 10);
                break;
              default: write(MeuSocket, "bolos", 6);
                break;
            }
            read(MeuSocket, buffer, 1024);
            printf("%s\n", buffer);
          break;

        default: 
          write(MeuSocket, "Option 2", 9);
          break;
      }
    }

    close(MeuSocket);
    return 0;
}
