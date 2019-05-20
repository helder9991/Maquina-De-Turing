# maquina-de-turing


Algoritmo que simula uma maquina de Turing.

-> Leitura do arquivo:
  
  (simbolos que a maquina consiguira ler):alfabeto
  
  (simbolos que a maquina poderá escrever apos a leitura):alfabeto aux
  
  (palavra que a maquina percorrerá):palavra
  
  vertice de partida, leitura, escrita, direção, vertice destino
  
  Exemplo:
  
    01:alfabeto
    SN:alfabeto aux
    110101:Palavra
    0,#,#,>,1
    1,*,*,>,2
    1,0,0,>,8
    1,1,1,>,4
    2,_,N,>,3
    4,1,1,>,5
    4,0,0,>,8
    5,1,1,>,5
    5,0,0,>,5
    5,*,*,>,6
    6,_,S,>,7
    8,0,0,>,8
    8,1,1,>,8
    8,*,*,>,2
    
