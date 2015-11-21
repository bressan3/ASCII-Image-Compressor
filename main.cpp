//
//  main.cpp
//  Trabalho ED II C++
//
//  Created by Lucas Paiva Bressan, Gabriell Lucas, Lorran Dutra on 5/26/15.
//
//

#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

//estrutura com os parametros do arquivo a ser comprimido
typedef struct arqOriginal{
    ifstream imagem;
    int tamHoriz = 0;
    int tamVert = 0;
}arqOriginal;

//estrutura que guarda as 3 leituras do arquivo.
typedef struct leituraArquivo{
    string strHorizontal;
    string strVertical;
    string strDiagonal;
}leituraArquivo;

//IMPORTANTE (como foi feito no mac eu tive que especificar o caminho ate o diretorio completo):
//diretorio do arquivo a ser comprimido
string path = "/~/Desktop/Trabalho/Trabalho ED II C++/Trabalho ED II C++/";
double media = 0;
int contArq = 0;

//metodo responsavel pela geracao de relatorio de arquivos com taxa de comp. negativas ----------------*/
void addRelCompNegativa(string arquivo){
    ifstream lerRelatorio;
    lerRelatorio.open(path+"Relatorios/CompressoesNegativas.txt");
    
    //armazena a listagem original de arquivos em aux
    string aux;
    getline(lerRelatorio, aux);
    
    lerRelatorio.close();
    //adiciona o novo arquivo negativo em aux caso ele nao esteja na lista
    if(aux.find(arquivo) == string::npos)
        aux = aux + arquivo + ", ";
    
    ofstream relatorio;
    relatorio.open(path+"Relatorios/CompressoesNegativas.txt");
    
    //escreve aux no arquvio
    relatorio<<aux;
    
    relatorio.close();
}

//metodo responsavel pela geracao de relatorio de taxa de compressão de um mesmo arquivo em seus
//diferentes caminhos --------------------------------------------------------------------------------*/
void addRelTaxaPorCaminho(string nomeOriginal, char caminho, float taxaCompressao){
    ifstream lerRelatorio;
    lerRelatorio.open(path+"Relatorios/TaxaPorCaminho - "+nomeOriginal);
    
    string aux;
    getline(lerRelatorio, aux);
    
    lerRelatorio.close();
    
    //faz conversão de float (taxaCompressao) para string
    std::string taxa = std::to_string(taxaCompressao);
    
    //checa se a taxa de compressao de tal caminho ja fora calculada anteriormente
    if(caminho == 'h' && aux.find("horizontal") == string::npos){
        aux = aux +"Taxa de compressão horizontal: "+taxa+"%"+", ";
    }
    else if(caminho == 'v' && aux.find("vertical") == string::npos){
        aux = aux +"Taxa de compressão vertical: "+taxa+"%"+", ";
    }
    else if(caminho == 'd' && aux.find("diagonal") == string::npos){
        aux = aux +"Taxa de compressão diagonal: "+taxa+"%"+", ";
    }
    
    ofstream relatorio;
    relatorio.open(path+"Relatorios/TaxaPorCaminho - "+nomeOriginal);
    
    //escreve aux no arquvio
    relatorio<<aux;
}

//metodo responsável pela geracao do relatório contendo a taxa de compressão média por caminho --------*/
void addRelTaxaMedia(float taxaCompressao, char caminho){
    ifstream lerRelatorio;
    
    //verifica o caminho e decide qual arquivo abrir
    if(caminho == 'h')
        lerRelatorio.open(path+"Relatorios/TaxaMediaHorizontal.txt");
    else if(caminho == 'v')
        lerRelatorio.open(path+"Relatorios/TaxaMediaVertical.txt");
    else if(caminho == 'd')
        lerRelatorio.open(path+"Relatorios/TaxaMediaDiagonal.txt");
    
    //formato do arquivo: (5) 30.0 (sendo 5 = numero de arquivos comprimidos e 30.0 = media atual)
    string aux;
    string aux2;
    string strContArqComp;
    int contArqComp = 0;
    float media = 0;
    getline(lerRelatorio, aux);
    if(aux != ""){
        int j = 1;
        //percorre verificndo se o numero de arquivos comprimidos tem mais de uma algarismo
        while (aux[j] != ')') {
            strContArqComp += aux[j];
            j++;
        }
        //caso o numero de arquivos tenha mais de um algarismo (seja maior que 9);
        if(j > 2)
            contArqComp = stoi(strContArqComp);
        else contArqComp = aux[1] - '0';
        //le a media ja presente no arquivo e a armazena para posterior calculo
        for (int i = 4; i < aux.length(); i++)
            aux2[i-4] = aux[i];
        media = stoi(aux2);
    }
    contArqComp++;
    lerRelatorio.close();
    
    ofstream arquivo;
    
    if(caminho == 'h')
        arquivo.open(path+"Relatorios/TaxaMediaHorizontal.txt");
    else if(caminho == 'v')
        arquivo.open(path+"Relatorios/TaxaMediaVertical.txt");
    else if(caminho == 'd')
        arquivo.open(path+"Relatorios/TaxaMediaDiagonal.txt");

    arquivo<<"("<<contArqComp<<")"<<" "<<((media * (contArqComp-1)) + taxaCompressao)/ contArqComp;
    
    arquivo.close();
}

//funcao responsavel pelo calculo da taxa de comrpessao -----------------------------------------------*/
float printTaxaDeCompressao(string nome, string nomeOriginal, char caminho){
    ifstream arquivo;
    arquivo.open(path+nome);
    
    int contChar = 0;
    string aux;
    //dimensoes do arquivo orignal
    string tamHoriz;
    string tamVert;
    bool mudaEixo = true; //true = verifica tamanho horizontal. false = verifica tamanho vertical.
    
    getline(arquivo, aux);
    contChar = (int)aux.length() + 1;
    
    //verifica o numero de caracteres no arquivo original
    for (int i = 0; i < aux.length(); i++) {
        if (aux[i] == '|')
            mudaEixo = false;
        else if(mudaEixo)
            tamHoriz += aux[i];
        else
            tamVert += aux[i];
    }
    
    //calcula o numero de caracteres no arquivo comprimido gerado
    while (!arquivo.eof()) {
        getline(arquivo, aux);
        contChar += aux.length() + 1;
    }
    
    cout<<"Taxa de compressao do arquivo"<<nome<<": "<<endl;
    //calcula a taxa de compressao [convertendo as string com as dimensoes originais em int (stoi())]
    float taxaCompressao = 100 - ((float)contChar / (((stoi(tamHoriz)*stoi(tamVert)) + stoi(tamHoriz)))*100);
    cout<<taxaCompressao<<"%"<<endl;
    addRelTaxaPorCaminho(nomeOriginal, caminho, taxaCompressao);
    addRelTaxaMedia(taxaCompressao, caminho);
    if(taxaCompressao < 0){
        cout<<"Taxa de compressão negativa. Arquivo maior que o original após compressão."<<endl;
        addRelCompNegativa(nome);
    }
    
    return taxaCompressao;
}

//funcao de compressao (retorna a taxa de comrpessão atingida) ----------------------------------*/
float comprimirArq(string str, int largura, int altura, string nome, string nomeOriginal, char caminho){
    ofstream novoArquivo;
    novoArquivo.open(path+nome);
    
    //escreve as dimensoes originais da imagem para a descompressao e o tipo de leitura feito (h = horizontal, v = vertical, d = diagonal).
    novoArquivo<<largura<<"|"<<altura<<caminho<<endl;
    
    int contAux = 0;
    char aux;
    
    for (int i = 0; i < str.length(); i++) {
        aux = str[i];
        novoArquivo<<aux;
        //calcula o numero de repeticoes de um caracter
        while (str[i] == aux) {
            contAux++;
            i++;
        }
        if (contAux == 1) {
            novoArquivo<<endl;
        }
        else{
            //faz conversao para tabela ascii (em seu range ja determinado)
            char dezena,unidade;
            if(contAux > 94)
            {
                //criacao de uma base numerica de 95 elementos baseados na tabela ascii
                //(tabela ascii tem 95 char entao dividimos por 95 assim como nas conversoes em binario e etc).
                dezena = (int)(contAux/95) + 32; //transforma em dezena
                unidade = (int)(contAux % 95) + 32; //transforma em unidade
                novoArquivo<<dezena<<unidade<<endl;
            }
            //soma 32 para que sempre de um caracter da tabela ascii (ja que esta so vai de 32 a 127)
            else
            {
                novoArquivo<<(char)(contAux + 32)<<endl; //transforma em unidade
            }
        }
        contAux = 0;
        i--;
    }
    
    novoArquivo.close();
    cout<<"Arquivo comprimido com sucesso!"<<endl;
    float taxaCompressao = printTaxaDeCompressao(nome,nomeOriginal,caminho);
    
    return taxaCompressao;
}

void descomprime(string nomeArquivoDesc){
    ifstream arquivo;
    arquivo.open(path+nomeArquivoDesc+".txt");
    
    //se o arquivo nao existir
    if(!arquivo.good())cout<<"Arquivo selecionado inválido"<<endl;
    
    else{
        string aux;
        getline(arquivo, aux);//copia a string de dimensoes para aux
    
        string tamHoriz;
        string tamVert;
        char caminho = aux[aux.length() - 1];
        bool mudaEixo = true;//true = verifica tamanho horizontal. false = verifica tamanho vertical.
    
        //verifica o numero de caracteres no arquivo original e suas dimensoes
        for (int i = 0; i < aux.length() - 1; i++) {
            if (aux[i] == '|')
                mudaEixo = false;
            else if(mudaEixo)
                tamHoriz += aux[i];
            else
                tamVert += aux[i];
        }
        
        //aloca matriz com dimensoes do arquivo original com as informcoes de dimensoes obtidas
        char matDescomp[(int)stoi(tamHoriz)][(int)stoi(tamVert)];
        //string responsavel por armazenar a string do arquivo completa (descomprimida)
        string strMat;
        
        //faz a leitura do arquivo e descomprime os caracteres colocando-os na string strMat
        while (!arquivo.eof()) {
            int contRepet = 0;
            getline(arquivo, aux);
            if(aux.length() == 1)
                contRepet = 1;
            else if(aux.length() == 2){
                contRepet = aux[1] - 32;
            }
            else{
                contRepet = ((aux[1] - 32)*95) + (aux[2] - 32);
            }
            for (int i = 0; i < contRepet; i++) {
                strMat += aux[0];
            }
        }
        
        //faz a transferencia da string (strMat) para a matriz de descompressao (dependendo to tipo de caminhamento) --------------------------*/
        int k = 0;
        
        //arquivo horizontal
        if (caminho == 'h') {
            for (int j = 0; j < (int)stoi(tamVert); j++) {
                for (int i = 0; i < (int)stoi(tamHoriz); i++) {
                    matDescomp[i][j] = strMat[k];
                    k++;
                }
            }
        }
        //arquivo vertical
        else if(caminho == 'v'){
            for (int j = 0; j < (int)stoi(tamHoriz); j++) {
                for (int i = 0; i < (int)stoi(tamVert); i++) {
                    matDescomp[j][i] = strMat[k];
                    k++;
                }
            }
        }
        
        //caso seja diagonal
        else if (caminho == 'd'){
            int somaMaxima = stoi(tamHoriz) + stoi(tamVert) - 2;
            string aux;
            string strDiagonal;
            bool direcao = false;
            
            //ordena a stirng strMat de acordo com os padroes de leitura diagonal originais
            for (int soma = 0; soma <= somaMaxima; soma++) {
                for (int i = 0; i < stoi(tamHoriz); i++) {
                    for (int j = 0; j < stoi(tamVert); j++) {
                        if (i + j - soma == 0) {
                            aux += strMat[k];
                            k++;
                        }
                    }
                }
                if (direcao) {
                    for (int i = (int)aux.length() - 1; i >= 0; i--) {
                        strDiagonal += aux[i];
                    }
                    direcao = false;
                }
                else{
                    strDiagonal += aux;
                    direcao = true;
                }
                aux.clear();
            }
            
            k = 0;
            
            //preenche a matriz com a string diagonal ordenada
            for (int soma = 0; soma <= somaMaxima; soma++) {
                for (int i = 0; i < stoi(tamHoriz); i++) {
                    for (int j = 0; j < stoi(tamVert); j++) {
                        if (i + j - soma == 0) {
                            matDescomp[i][j] = strDiagonal[k];
                            k++;
                        }
                    }
                }
            }
        }
        
        /*-------------------------------------------------------------------------------------------------------------------------------------*/
        
        arquivo.close();
        
        ofstream arquivoDescomp;
        arquivoDescomp.open(path+nomeArquivoDesc+"Descomprimido.txt");
        
        //le a matriz de descompressao escreve no novo arquivo criado (arquivoDescomp) pelo caminho horizontal
        for (int j = 0; j < (int)stoi(tamVert); j++) {
            for (int i = 0; i < (int)stoi(tamHoriz); i++) {
                arquivoDescomp<<matDescomp[i][j];
            }
            arquivoDescomp<<endl;
        }
    }
}

int main(int argc, const char * argv[]) {
    while (true) {
    cout<<"DIGITE OS NOMES DE TODOS ARQUIVOS SEM EXTENSAO!"<<endl<<endl;
    char operacao;
    cout<<"Digite c para comprimir um arquivo, d para descomprimir um arquivo ou s para sair: ";
    cin>>operacao;
        
    //cria pasta de relatorios
    string pathRelatorios = path+"Relatorios";
    mkdir(pathRelatorios.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        
    if (operacao == 'c') {
    //string com endereco do arquivo
    string nomeArqOriginal;
    cout<<"Digite o nome do arquivo a ser comprimido: ";
    cin>>nomeArqOriginal;
    nomeArqOriginal += ".txt";
    
    arqOriginal *arquivo = new arqOriginal();
    arquivo->imagem.open(path+nomeArqOriginal);
    
    //checa se o arquivo existe.
    if(!arquivo->imagem.good()){
        cout<<"Arquivo não encontrado."<<endl;
    }
    
    else{
        
    /*checa o arquivo a ser comprimido e define seus parametros -----------------------------------*/
    int alturaAux = 1;
    string str;
    
    getline(arquivo->imagem,str);
    arquivo->tamHoriz = (int)str.length();
    
    while (!arquivo->imagem.eof()) {
        getline(arquivo->imagem,str);
        alturaAux++;
    }
    arquivo->tamVert = alturaAux;
    arquivo->imagem.close();
    /*---------------------------------------------------------------------------------------------*/
    
    //cria matriz para copia de caracteres e leitura destes e abre o arquivo novamente
    char matImagem[arquivo->tamHoriz][arquivo->tamVert];
    arquivo->imagem.open(path+nomeArqOriginal);
    
    /*copia caracteres da imagem para a matriz ----------------------------------------------------*/
    string aux;
    for (int j = 0; j < arquivo->tamVert; j++) {
        getline(arquivo->imagem,aux);
        for (int i = 0; i < arquivo->tamHoriz; i++) {
            matImagem[i][j] = aux[i];
        }
    }
    arquivo->imagem.close();
    /*---------------------------------------------------------------------------------------------*/
    
    /*cria as três strings a serem comprimidas e as popula a partir da matriz criada --------------*/
    
    leituraArquivo arqCompressao;
    
    //faz atribuicao da string de leitura horizontal
    for (int j = 0; j < arquivo->tamVert; j++){
        for (int i = 0; i < arquivo->tamHoriz; i++){
            arqCompressao.strHorizontal += matImagem[i][j];
        }
    }
    
    //faz atribuicao da string de leitura vertical
    for (int j = 0; j < arquivo->tamHoriz; j++) {
        for (int i = 0; i < arquivo->tamVert; i++) {
            arqCompressao.strVertical += matImagem[j][i];
        }
    }
    
    //faz atribuicao da string de leitura diagonal
    int somaMaxima = arquivo->tamHoriz + arquivo->tamVert - 2;
    bool direcao = false; //false = de baixo para cima | true = de cima para baixo
    aux.clear();
    
    for (int soma = 0; soma <= somaMaxima; soma++) {
        for (int i = 0; i < arquivo->tamHoriz; i++) {
            for (int j = 0; j < arquivo->tamVert; j++) {
                if (i + j - soma == 0) {
                    aux += matImagem[i][j];
                }
            }
        }
        //le a string auxiliar ao contrario caso direcao seja true e dpeois inverte a direcao de leitura da matriz
        if(direcao){
            for (int i = (int)aux.length() - 1; i >= 0; i--) {
                arqCompressao.strDiagonal += aux[i];
            }
            direcao = false;
        }
        //le string auxiliar normalmente e depois inverte direcao de leitura da matriz
        else{
            arqCompressao.strDiagonal += aux;
            direcao = true;
        }
        aux.clear();
    }
    /*---------------------------------------------------------------------------------------------*/
    /*fim do caminhamento e atribuicao das strings a serem comprimidas ----------------------------*/
    
    /*Teste de caminhamento:
    cout<<arqCompressao.strHorizontal<<endl;
    cout<<arqCompressao.strVertical<<endl;
    cout<<arqCompressao.strDiagonal<<endl;*/
    
    //Chamada para a funcao de comrpessao dos arquivos nos tres caminhamentos ---------------------*/
    cout<<"Digite o nome do arquivo de saida: ";
    string nomeArqSaida;
    cin>>nomeArqSaida;
    cout<<"Comprimindo..."+nomeArqOriginal<<endl;
    float taxaStrHoriz = comprimirArq(arqCompressao.strHorizontal,arquivo->tamHoriz,arquivo->tamVert,nomeArqSaida+"Horizontal.txt",nomeArqOriginal,'h');
    float taxaStrVert = comprimirArq(arqCompressao.strVertical,arquivo->tamHoriz,arquivo->tamVert,nomeArqSaida+"Vertical.txt",nomeArqOriginal,'v');
    float taxaStrDiag = comprimirArq(arqCompressao.strDiagonal,arquivo->tamHoriz,arquivo->tamVert,nomeArqSaida+"Diagonal.txt",nomeArqOriginal,'d');
    contArq++;
    /*---------------------------------------------------------------------------------------------*/
    
    //impressao da taxa de comrpessão media -------------------------------------------------------*/
    cout<<"Taxa de compressão media atingida: "<<endl;
    cout<<(taxaStrHoriz + taxaStrVert + taxaStrDiag) / 3<<"%"<<endl;
    /*---------------------------------------------------------------------------------------------*/
        double melhorTaxa = taxaStrHoriz;
        
        if(melhorTaxa <= taxaStrVert)
            melhorTaxa = taxaStrVert;
        else if(melhorTaxa <= taxaStrDiag)
            melhorTaxa = taxaStrDiag;
        
        media += melhorTaxa;
        
    arquivo->imagem.close();
    }
    }
    
    else if(operacao == 'd'){
        string nomeArquivoDesc;
        cout<<"Digite o nome do arquivo a ser descomprimido: ";
        cin>>nomeArquivoDesc;
        cout<<"Descomprimindo "+nomeArquivoDesc+".txt (...)"<<endl;
        descomprime(nomeArquivoDesc);
        cout<<"Arquivo descomprimido com sucesso!"<<endl;
    }
        
    else if (operacao == 's'){
        media = media/contArq;
        cout<<"Media melhor caminho: "<<media<<"%"<<endl;
        cout<<"Sessao concluida."<<endl;
        exit(0);
    }
    
    else cout<<"Operacao selecionada inválida"<<endl;
    }
    
    return 0;
}
