# Grupo

Gustavo Akira Hirakawa - 11381768 <br>
Leandro Satoshi de Siqueira - 10893103 <br>
Luana Terra do Couto - 11218917 <br>
Paolo Victor Hitoshi Scassa - 10783312 <br>
Yago Poletto Salgado - 10724455 <br><br>

Versão do Linux: 20.04.2 LTS<br>
Compilador: g++<br>

Trabalho 1 para SSC0142 - Redes de Computadores (2021)<br><br>

Foi feita a implementação de uma loja virtual para doces, com o código para o servidor e para o cliente. O cliente pode selecionar uma das opções de doces disponíveis e o servidor verifica se há disponibilidade no estoque. Caso a compra possa ser feita, o servidor retorna o sucesso da compra e retira uma unidade do produto do estoque (todas as compras são unitárias). Caso o doce não esteja disponível, o servidor retorna a falta no estoque, e o cliente pode tentar comprar outro doce.<br><br>
Detectamos um problema ao encerrar a execução do servidor. Em alguns testes houveram problemas de "Segmentation fault" no servidor assim que selecionamos a opção de saída com o cliente. Mesmo assim, toda a parte da conexão em si funciona como esperado, e o cliente consegue realizar a compra do seu doce sem problemas.<br><br>
Foram realizadas algumas alterações na documentação dos códigos após a apresentação do dia 09/06/2021.
