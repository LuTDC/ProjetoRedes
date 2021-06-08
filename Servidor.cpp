#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <mutex>
#include <thread>
#include <queue>

#define limiteCliente 3

std::thread tid[limiteCliente]; //vetor de threads
std::mutex mutexFila;
std::queue<int> qLivre;

void options(int Peersocket, int id){
  char* buffer;
  write(Peersocket ,"Bem-vindo\r\n" , 11);
  printf("id: %d\n", id);
  while(strcmp(buffer, "Option -1") != 0){
    printf("inside options wait for read\n");
		read(Peersocket , buffer, 1024);
    printf("received: %s\n", buffer);
  	if(strcmp(buffer, "Option 0") == 0) printf("Option 0 received.\n");
    if(strcmp(buffer, "Option 1") == 0) printf("Option 1 received.\n");
  }
       
  //loop de finalizacao
	while(1){
    // testa se conseguiu abrir a porta. Se nao, fica em loop
    if(mutexFila.try_lock()){

      // armazena a identidade da funcao de volta na fila
      qLivre.push(id);

      // sai da regiao critica
      mutexFila.unlock();

      // desliga thread com return

      

      return;
    }
  } 
  return;
}



//programa servidor
int main(int argc, char *argv[]){

  int MINHA_PORTA = 8080; //porta escolhida para o servidor
  int Meusocket, Peersocket; //socket do servidor e descritor
  struct sockaddr_in local_addr; //endereço local
  struct sockaddr_in peer_addr; //endereço do descritor
  socklen_t peerAddrLen; //tamanho do endereço do socket auxiliar
  int id = 0;
  
  
  for(int i = 0; i < limiteCliente; i++){
  	qLivre.push(i); 
  }
  
  if (argc > 1) MINHA_PORTA = atoi(argv[1]);
  
  //Meusocket = sozket(familia_de_protocolos: AF_INET = ARPA, tipo_de_socket_buscado, num_protocolo. 0 = Internet Protocol. 6 = TCP. 17 = UDP.)
	Meusocket = socket(AF_INET, SOCK_STREAM, 6);
  if(Meusocket < 0){
  	printf("erro sockect");
    return -1;
  }
         
	local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(MINHA_PORTA);
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  //conexão do socket com o IP
  if(bind(Meusocket, (struct sockaddr *) &local_addr, sizeof(struct sockaddr)) < 0){
    printf("erro bind");
    return -1;
  }
         
  //retorna 0 se sucesso. retorn < 0 se erro
  if(listen(Meusocket, 1) != 0){ //1 é o tamanho máximo da fila
  	printf("erro listen");
    return -1;
  }

  mutexFila.unlock();
  while(1){
    
    //aceite de conexão com os clientes
    printf("Peersocket accept\n");
   	Peersocket = accept(Meusocket, (struct sockaddr *) &peer_addr, &peerAddrLen);
  	if(Peersocket < 0){
  		printf("peersocket error\n");
    	return -1;
  	}
    int aux = Peersocket;

    //se regiao critica nao estiver sendo acessada (fila de espacos vazios)
    if(mutexFila.try_lock()) {
      if(!qLivre.empty()) {
        // se fila populada, recebe um dos itens
        id = qLivre.front();
        qLivre.pop();
        printf("accessing id %d\n", id);
        //if(pthread_create(&tid[id], NULL, options(Peersocket, id), &Peersocket) != 0){
        tid[id] = std::thread(options, aux, id);
        tid[id].detach();
          // uma nova thread é criada para cada cliente recorrente com config (NULL) padrao, usando a funcao options.
          /*printf("Failed to create thread\n");
          qLivre.push(id);*/
        
      }else {
        write(Peersocket ,"Fila Cheia\r\n" , 12);
        shutdown(Peersocket,SHUT_RDWR);
      }
      mutexFila.unlock();
    }
  }
      
	return 0;
}
      
      
      
      