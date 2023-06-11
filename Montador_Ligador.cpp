#include <iostream>
#include <bits/stdc++.h>
#include <map>
#include <stdio.h>
#include <string.h>
using namespace std;

map<string, pair<int, int>> opcode{
    {"ADD", make_pair(1, 2)},
    {"SUB", make_pair(2, 2)},
    {"MULT", make_pair(3, 2)},
    {"MUL", make_pair(3, 2)},
    {"DIV", make_pair(4, 2)},
    {"JMP", make_pair(5, 2)},
    {"JMPN", make_pair(6, 2)},
    {"JMPP", make_pair(7, 2)},
    {"JMPZ", make_pair(8, 2)},
    {"COPY", make_pair(9, 3)},
    {"LOAD", make_pair(10, 2)},
    {"STORE", make_pair(11, 2)},
    {"INPUT", make_pair(12, 2)},
    {"OUTPUT", make_pair(13, 2)},
    {"STOP", make_pair(14, 1)},
};

string erros = "";
vector<pair<string, int>> tabela_simbolo;
vector<vector<string>> tudo;
vector<string> label_name;
map<string, int> tabela_simbolos;
vector<int> contador;
vector<vector<int>> codiu_final;
string objeto = "", nome_prog;
bool e = 0;
map<string, string> dicionario_EQU;
map<string, string> cause5hora;

/*
    Funcao responsavel por converter uma string para maiusculo

    Esse funcao eh chamada para formatar todas as palavras(comando, label, chamadas) para que nao tenha problema com letras minusculas e maiusculas
*/

string concat(vector<string> vect)
{
    string ret;
    for (int i = 0; i < vect.size(); i++)
    {
        ret += vect[i] + " ";
    }
    return ret;
}

string strupr(string upper)
{
    string aux = "";
    for (int i = 0; i < upper.size(); i++)
    {
        aux += toupper(upper[i]);
    }
    return aux;
}

/*
    Funcao responsavel por gerar o arquivo, sempre eh a ultima funcao chamada, a primeira string que le recebe contem o nome do arquivo, a segunda eh o
    tipo(.PRE, .MCR e .OBJ) e o ultimo argumento eh a string com o programa completo
*/
void cria_arquivo(string nome, string formato, string texto)
{
    FILE *arq;
    char num[80], text[1000];
    string path = nome + formato;
    strcpy(num, path.c_str());
    arq = fopen(num, "w+");

    if (arq == NULL)
    {
        cout << "Erro ao criar arquivo" << endl;
        return;
    }

    strcpy(text, texto.c_str());

    if (fputs(text, arq) == EOF)
        cout << "Erro na Gravacao" << endl;

    fclose(arq);
}

/*
    A funcao prog eh sempre a primeira funcao chamada, ela é responsavel por formatar os dados lidos do arquivo, esses dados sao colocados
    em um vector<vector<string>> onde cada linha eh um vetor dentro de uma matriz e cada palavra eh uma string dentro desse vetor,
    assim facilitando para as outras funcoes.
*/
void prog(FILE *program, int tipo)
{
    int band = 1;
    int passou_data = 0, passou_text = 0;
    while (1)
    {
        vector<string> linha_comando = {};
        char linha[100];
        string result;
        if (fgets(linha, 100, program) != NULL)
        {
            result = linha;
        }
        else
        {
            band = 0;
            break;
        }
        int espaco = 0;
        string ultimo_comando;
        string exemplo;
        int j = 0, contou = 0, dois_pontos = 0;
        for (int i = 0; i < result.size() - 1; i++)
        {
            if (result[i] == ';')
                break;
            if (result[i] != ' ' && result[i] != '\n' && result[i] != ',')
            {
                if ((result[i] < 'A' || result[i] > 'Z') && (result[i] < 'a' || result[i] > 'z') && result[i] != '_' && result[i] != ':' && (result[i] < '0' || result[i] > '9'))
                {
                    string erro = "Erro Lexico: caracter especial\nLinha: ";
                    erro += result;
                    erros += erro;
                    cout << erro << "Erro salvo em Log.txt na pasta do programa\n";
                }
                exemplo += result[i];
                if (result[i] == ':')
                {
                    dois_pontos++;
                    if (dois_pontos > 1)
                    {
                        string erro = "Erro Sintático: dois rotulos na mesma linha, esse erro impossibilita gerar o exc\nLinha: ";
                        erro += result;
                        erros += erro;
                        cout << erro << "Erro salvo em Log.txt na pasta do programa\n";
                        cria_arquivo("Log", ".txt", erros);
                        e = 1;
                        return;
                    }
                }
            }
            else
            {
                if (result.size() - 1 != i + 3)
                {
                    if (result[i + 1] == '+')
                    {
                        exemplo += result[i + 1];
                        i += 2;
                    }
                    else
                    {
                        if (exemplo != "" && exemplo != "\n" && exemplo != " ")
                        {
                            if (exemplo[0] >= '0' && exemplo[0] <= '9')
                            {
                                if (exemplo[(exemplo.size() - 1)] == ':')
                                {
                                    string erro = "Erro Lexico: numero no inicio de um rotulo\nLinha: ";
                                    erro += result;
                                    erros += erro;
                                    cout << erro << "Erro salvo em Log.txt na pasta do programa\n";
                                }
                            }
                            if ((strupr(exemplo) == "CONST" || strupr(exemplo) == "SPACE") && passou_data == 0)
                            {
                                string erro = "Erro: dado criado antes da sessao data\nLinha: ";
                                erro += result;
                                erros += erro;
                                cout << erro << "Erro salvo em Log.txt na pasta do programa\n";
                            }
                            if (strupr(exemplo) == "DATA")
                            {
                                passou_data = 1;
                            }
                            if (strupr(exemplo) == "TEXT")
                            {
                                passou_text = 1;
                            }
                            linha_comando.push_back(strupr(exemplo));
                        }

                        exemplo = "";
                    }
                }
                else
                {
                    if (exemplo != "" && exemplo != "\n" && exemplo != " ")
                    {
                        if (exemplo[0] >= '0' && exemplo[0] <= '9')
                        {
                            if (exemplo[(exemplo.size() - 1)] == ':')
                            {
                                string erro = "Erro Lexico: numero no inicio de um rotulo\nLinha: ";
                                erro += result;
                                erros += erro;
                                cout << erro << "Erro salvo em Log.txt na pasta do programa\n";
                            }
                        }
                        if ((strupr(exemplo) == "CONST" || strupr(exemplo) == "SPACE") && passou_data == 0)
                        {
                            string erro = "Erro: dado criado antes da sessao data\nLinha: ";
                            erro += result;
                            erros += erro;
                            cout << erro << "Erro salvo em Log.txt na pasta do programa\n";
                        }
                        if (strupr(exemplo) == "DATA")
                        {
                            passou_data = 1;
                        }
                        if (strupr(exemplo) == "TEXT")
                        {
                            passou_text = 1;
                        }
                        linha_comando.push_back(strupr(exemplo));
                    }
                    exemplo = "";
                }
            }
        }
        if (exemplo != " " && exemplo != "\n" && exemplo != "," && exemplo != "")
        {
            if (exemplo[0] >= '0' && exemplo[0] <= '9')
            {
                if (exemplo[(exemplo.size() - 1)] == ':')
                {
                    string erro = "Erro Lexico: numero no inicio de um rotulo\nLinha: ";
                    erro += result;
                    erros += erro;
                    cout << erro << "Erro salvo em Log.txt na pasta do programa\n";
                }
            }
            if ((strupr(exemplo) == "CONST" || strupr(exemplo) == "SPACE") && passou_data == 0)
            {
                string erro = "Erro: dado criado antes da sessao data\nLinha: ";
                erro += result;
                erros += erro;
                cout << erro << "Erro salvo em Log.txt na pasta do programa\n";
            }
            if (strupr(exemplo) == "DATA")
            {
                passou_data = 1;
            }
            if (strupr(exemplo) == "TEXT")
            {
                passou_text = 1;
            }
            linha_comando.push_back(strupr(exemplo));
        }
        tudo.push_back(linha_comando);
    }
    if (passou_text == 0)
    {
        string erro = "Erro: sessao text nao criado";
        erros += erro;
        cout << erro << "Erro salvo em Log.txt na pasta do programa\n";
    }
    fclose(program);
}

int contemSTR(string palavra, string palavra_externa)
{
    int mais = 0;
    string numero = "";
    for (int i = 0; i < palavra.size(); i++)
    {
        if (palavra_externa.size() - 1 > i)
        {
            if (palavra[i] != palavra_externa[i])
            {
                return 0;
            }
        }
        else if (palavra[i] == '+')
        {
            mais = 1;
        }
        else if ((mais == 1) && (palavra[i] > '0' && palavra[i] < '9'))
        {
            numero += palavra[i];
        }
    }
    int numero_int = stoi(numero);
    return numero_int;
}

void passa_arq()
{
    string novo_arq;
    for (int i = 0; i < tudo.size(); i++)
    {
        for (int j = 0; j < tudo[i].size(); j++)
        {
            novo_arq += string(tudo[i][j]) + " ";
        }
        novo_arq += "\n";
    }
    novo_arq.pop_back();
    cria_arquivo(nome_prog, ".pre", novo_arq);
}

void passa_arq_obj(vector<int> porg_obj)
{
    string novo_arq;
    for (int i = 0; i < porg_obj.size(); i++)
    {
        novo_arq += to_string(porg_obj[i]) + " ";
    }
    novo_arq.pop_back();
    cria_arquivo(nome_prog, ".exc", novo_arq);
}

pair<string, int> tem_mais(string palavra)
{
    string saida = "";
    string numeroS = "0";
    int numero = 0;
    int mais = 0;
    for (int i = 0; i < palavra.size(); i++)
    {
        if (palavra[i] != '+' && mais == 0)
        {
            saida += palavra[i];
        }
        else if (palavra[i] == '+')
        {
            mais = 1;
        }
        else if (mais == 1 && (palavra[i] >= '0' && palavra[i] <= '9'))
        {
            numeroS += palavra[i];
        }
    }
    numero = stoi(numeroS);
    return make_pair(saida, numero);
}

/* 
    Algoritmo de passagem unica, le e adiciona automaticamente ao vertor final_exc 
 */

void passagem_unica()
{
    int k = 0, comando = 0, text = 0, data = 0, flag_label = 0;
    int endereco_atual = 0, linha_atual = 0;
    set<char> chares = {'_', '-', ':', '+'};
    string final;
    vector<int> final_exc;
    map<string, vector<int>> lista_simbolo;
    for (int i = 0; i < tudo.size(); i++)
    {
        flag_label = 0;
        int section = 0, cont_space = 0, const_band = 0, stop = 0, copy = 0;
        for (int j = 0; j < tudo[i].size(); j++)
        {
            if (tudo[i][j] == "SECTION")
            {
                section = 1;
                if (tudo[i].size() - 1 > j)
                {
                    if (tudo[i][j + 1] == "TEXT")
                    {
                        text++;
                    }
                    else if (tudo[i][j + 1] == "DATA")
                    {
                        data++;
                    }
                    j = tudo[i].size();
                }
            }
            else if (tudo[i][j][tudo[i][j].size() - 1] == ':')
            {
                flag_label = 1;
                if (tudo[i][j] != "")
                {
                    label_name.push_back(tudo[i][j].substr(0, tudo[i][j].size() - 1));
                    tabela_simbolos[tudo[i][j].substr(0, tudo[i][j].size() - 1)] = linha_atual;
                }
            }
            else if (comando == 0)
            {
                comando = 1;
                if (tudo[i][j] == "COPY")
                {
                    pair<int, int> soma = opcode.find(tudo[i][j])->second;
                    final_exc.push_back(soma.first);
                    endereco_atual++;
                    pair<string, int> saida1 = tem_mais(tudo[i][j + 1]);
                    lista_simbolo[saida1.first].push_back(endereco_atual);
                    endereco_atual++;
                    pair<string, int> saida2 = tem_mais(tudo[i][j + 2]);
                    lista_simbolo[saida2.first].push_back(endereco_atual);
                    endereco_atual++;
                    final_exc.push_back(saida1.second);
                    final_exc.push_back(saida2.second);
                    copy = 1;
                    j = tudo[i].size();
                }
                else if (tudo[i][j] == "STOP")
                {
                    pair<int, int> soma = opcode.find(tudo[i][j])->second;
                    final_exc.push_back(soma.first);
                    endereco_atual++;
                    stop = 1;
                    j = tudo[i].size();
                }
                else if (tudo[i][j] == "SPACE")
                {
                    endereco_atual++;
                    if (tudo[i].size() - 1 > j)
                    {
                        for (int k = 0; k < stoi(tudo[i][j + 1]); k++)
                        {
                            final_exc.push_back(0);
                            endereco_atual++;
                            cont_space++;
                        }
                    }
                    else
                    {
                        final_exc.push_back(0);
                        endereco_atual++;
                        cont_space++;
                    }
                    j = tudo[i].size();
                }
                else if (tudo[i][j] == "CONST")
                {
                    if(tudo[i].size()-1 != j){
                        if(tudo[i][j+1][0] == '0' && tudo[i][j+1][1]== 'X'){
                            int num = stoi(tudo[i][j+1], nullptr, 16);
                            final_exc.push_back(num);
                            endereco_atual++;
                            const_band = 1;
                            j = tudo[i].size();
                        }else{
                            final_exc.push_back(stoi(tudo[i][j + 1]));
                            endereco_atual++;
                            const_band = 1;
                            j = tudo[i].size();
                        }
                    }
                }
                else
                {
                    pair<int, int> soma = opcode.find(tudo[i][j])->second;
                    final_exc.push_back(soma.first);
                    endereco_atual++;
                    pair<string, int> saida = tem_mais(tudo[i][j + 1]);
                    if (tabela_simbolos.find(saida.first) == tabela_simbolos.end())
                    {
                        lista_simbolo[saida.first].push_back(endereco_atual);
                        final_exc.push_back(saida.second);
                    }
                    else
                    {
                        final_exc.push_back(tabela_simbolos.find(saida.first)->second + saida.second);
                    }
                    endereco_atual++;
                    j = tudo[i].size();
                }
            }
            else
            {
            }
        }
        if (section == 0 && tudo[i].size() > 0)
        {
            if (copy == 1)
            {
                linha_atual += 3;
            }
            else if (stop == 1 || const_band == 1)
            {
                linha_atual += 1;
            }
            else if (cont_space != 0)
            {
                linha_atual += cont_space;
            }
            else
            {
                linha_atual += 2;
            }
        }
        section = 0;
        stop = 0;
        copy = 0;
        const_band = 0;
        cont_space = 0;
        comando = 0;
    }
    for (int i = 0; i < label_name.size(); i++)
    {
        map<string, vector<int>>::iterator teste = lista_simbolo.find(label_name[i]);

        if (teste != lista_simbolo.end())
        {
            vector<int> saida = lista_simbolo.find(label_name[i])->second;
            for (int j = 0; j < saida.size(); j++)
            {
                if (tabela_simbolos.find(label_name[i]) != tabela_simbolos.end())
                {
                    final_exc[saida[j]] = final_exc[saida[j]] + tabela_simbolos.find(label_name[i])->second;
                }
            }
        }
    }
   /*  cout << "\n";
    for (int i = 0; i < final_exc.size(); i++)
    {
        cout << final_exc[i] << " ";
    } */
    passa_arq_obj(final_exc);
}

bool existe_string(vector<string> vetor, string name){
    for(int i = 0; i < vetor.size(); i++){
        if(vetor[i] == name){
            return true;
        }
    }
    return false;
}

void arq(vector<pair<string, int>> tabela_uso, vector<pair<string, int>> tabela_definicao, vector<int> relativos, vector<int> final_exc){
    string saida;
    saida += "USO\n";
    for (int i = 0; i < tabela_uso.size(); i++)
    {
        saida += tabela_uso[i].first + " " + to_string(tabela_uso[i].second) + "\n";
    }
    saida += "DEF\n";
    for (int i = 0; i < tabela_definicao.size(); i++)
    {
        saida += tabela_definicao[i].first + " " + to_string(tabela_definicao[i].second) + "\n";
    }
    saida += "RELATIVOS\n";
    for (int i = 0; i < relativos.size(); i++)
    {
        saida += relativos[i] + " ";
    }
    saida += "CODE\n";
    for (int i = 0; i < final_exc.size(); i++)
    {   
        saida += final_exc[i] + " ";
    }
    cria_arquivo(nome_prog, ".obj", saida);
}

void passagem_unica_ligador()
{
    int k = 0, comando = 0, text = 0, data = 0, flag_label = 0;
    int endereco_atual = 0, linha_atual = 0;
    set<char> chares = {'_', '-', ':', '+'};
    string final;
    vector<int> final_exc;
    map<string, vector<int>> lista_simbolo;
    vector<string> label_name_extern;
    vector<string> label_name_public;
    vector<pair<string, int>> tabela_uso;
    vector<pair<string, int>> tabela_definicao;
    vector<int> relativos;
    pair<string, int> begin_label;
    for (int i = 0; i < tudo.size(); i++)
    {
        flag_label = 0;
        int section = 0, cont_space = 0, const_band = 0, stop = 0, copy = 0;
        for (int j = 0; j < tudo[i].size(); j++)
        {
            if (tudo[i][j] == "SECTION")
            {
                section = 1;
                if (tudo[i].size() - 1 > j)
                {
                    if (tudo[i][j + 1] == "TEXT")
                    {
                        text++;
                    }
                    else if (tudo[i][j + 1] == "DATA")
                    {
                        data++;
                    }
                    j = tudo[i].size();
                }
            }
            else if (tudo[i][j][tudo[i][j].size() - 1] == ':')
            {
                flag_label = 1;
                if (tudo[i][j] != "")
                {
                    if(tudo[i][j].substr(0, tudo[i][j].size() - 1) == "EXTERN"){
                        section = 1;
                        label_name_extern.push_back(tudo[i][j+1]);
                        j = tudo[i].size();
                    }else if(tudo[i][j+1] == "BEGIN"){
                        begin_label = make_pair(tudo[i][j].substr(0, tudo[i][j].size() - 1), 0);
                        section = 1;
                        j = tudo[i].size();
                    }else{
                        label_name.push_back(tudo[i][j].substr(0, tudo[i][j].size() - 1));
                        tabela_simbolos[tudo[i][j].substr(0, tudo[i][j].size() - 1)] = linha_atual;
                    }
                }
            }
            else if(tudo[i][j] == "PUBLIC")
            {
                if (tudo[i].size()-1 > j)
                {
                    section = 1;
                    if(tudo[i][j+1] == begin_label.first){
                        tabela_definicao.push_back(begin_label);
                    }else{
                        label_name_public.push_back(tudo[i][j+1]);
                    }
                    j = tudo[i].size();
                }
            }
            else if (comando == 0)
            {
                comando = 1;
                if (tudo[i][j] == "COPY")
                {
                    pair<int, int> soma = opcode.find(tudo[i][j])->second;
                    final_exc.push_back(soma.first);
                    endereco_atual++;
                    relativos.push_back(linha_atual+1);
                    relativos.push_back(linha_atual+1);
                    pair<string, int> saida1 = tem_mais(tudo[i][j + 1]);
                    if(existe_string(label_name_extern, saida1.first)){
                        pair<string, int> par_uso;
                        par_uso = make_pair(saida1.first, linha_atual+1);
                        tabela_uso.push_back(par_uso);
                    }else if(existe_string(label_name_public, saida1.first)){
                        pair<string, int> par_definicao;
                        par_definicao = make_pair(saida1.first, linha_atual+1);
                        tabela_definicao.push_back(par_definicao);
                    }
                    else{
                        lista_simbolo[saida1.first].push_back(endereco_atual);
                    }
                    endereco_atual++;
                    pair<string, int> saida2 = tem_mais(tudo[i][j + 2]);
                    if(existe_string(label_name_extern, saida2.first)){
                        pair<string, int> par_uso;
                        par_uso = make_pair(saida2.first, linha_atual+1);
                        tabela_uso.push_back(par_uso);
                    }else if(existe_string(label_name_public, saida2.first)){
                        pair<string, int> par_definicao;
                        par_definicao = make_pair(saida2.first, linha_atual+1);
                        tabela_definicao.push_back(par_definicao);
                    }
                    else{
                        lista_simbolo[saida2.first].push_back(endereco_atual);
                    }
                    lista_simbolo[saida2.first].push_back(endereco_atual);
                    endereco_atual++;
                    final_exc.push_back(saida1.second); 
                    final_exc.push_back(saida2.second);
                    copy = 1;
                    j = tudo[i].size();
                }
                else if (tudo[i][j] == "STOP")
                {
                    pair<int, int> soma = opcode.find(tudo[i][j])->second;
                    final_exc.push_back(soma.first);
                    endereco_atual++;
                    stop = 1;
                    j = tudo[i].size();
                }
                else if (tudo[i][j] == "SPACE")
                {
                    endereco_atual++;
                    if (tudo[i].size() - 1 > j)
                    {
                        for (int k = 0; k < stoi(tudo[i][j + 1]); k++)
                        {
                            final_exc.push_back(0);
                            endereco_atual++;
                            cont_space++;
                        }
                    }
                    else
                    {
                        final_exc.push_back(0);
                        endereco_atual++;
                        cont_space++;
                    }
                    j = tudo[i].size();
                }
                else if (tudo[i][j] == "CONST")
                {
                    if(tudo[i].size()-1 != j){
                        if(tudo[i][j+1][0] == '0' && tudo[i][j+1][1]== 'X'){
                            int num = stoi(tudo[i][j+1], nullptr, 16);
                            final_exc.push_back(num);
                            endereco_atual++;
                            const_band = 1;
                            j = tudo[i].size();
                        }else{
                            final_exc.push_back(stoi(tudo[i][j + 1]));
                            endereco_atual++;
                            const_band = 1;
                            j = tudo[i].size();
                        }
                    }
                }
                else
                {
                    relativos.push_back(linha_atual+1);
                    pair<int, int> soma = opcode.find(tudo[i][j])->second;
                    final_exc.push_back(soma.first);
                    endereco_atual++;
                    pair<string, int> saida = tem_mais(tudo[i][j + 1]);
                    if (tabela_simbolos.find(saida.first) == tabela_simbolos.end())
                    {
                        if(existe_string(label_name_extern, saida.first)){
                            pair<string, int> par_uso;
                            par_uso = make_pair(saida.first, linha_atual+1);
                            tabela_uso.push_back(par_uso);
                        }else if(existe_string(label_name_public, saida.first)){
                            pair<string, int> par_definicao;
                            par_definicao = make_pair(saida.first, linha_atual+1);
                            tabela_definicao.push_back(par_definicao);
                        }
                        else{
                            lista_simbolo[saida.first].push_back(endereco_atual);
                        }
                        final_exc.push_back(saida.second);
                    }
                    else
                    {
                        if(existe_string(label_name_extern, saida.first)){
                            pair<string, int> par_uso;
                            par_uso = make_pair(saida.first, linha_atual+1);
                            tabela_uso.push_back(par_uso);
                        }else if(existe_string(label_name_public, saida.first)){
                            pair<string, int> par_definicao;
                            par_definicao = make_pair(saida.first, linha_atual+1);
                            tabela_definicao.push_back(par_definicao);
                        }
                        else{
                            lista_simbolo[saida.first].push_back(endereco_atual);
                        }
                        final_exc.push_back(tabela_simbolos.find(saida.first)->second + saida.second);
                    }
                    endereco_atual++;
                    j = tudo[i].size();
                }
            }
        }
        if (section == 0 && tudo[i].size() > 0)
        {
            if (copy == 1)
            {
                linha_atual += 3;
            }
            else if (stop == 1 || const_band == 1)
            {
                linha_atual += 1;
            }
            else if (cont_space != 0)
            {
                linha_atual += cont_space;
            }
            else
            {
                linha_atual += 2;
            }
        }
        section = 0;
        stop = 0;
        copy = 0;
        const_band = 0;
        cont_space = 0;
        comando = 0;
    }
    for (int i = 0; i < label_name.size(); i++)
    {
        map<string, vector<int>>::iterator teste = lista_simbolo.find(label_name[i]);

        if (teste != lista_simbolo.end())
        {
            vector<int> saida = lista_simbolo.find(label_name[i])->second;
            for (int j = 0; j < saida.size(); j++)
            {
                if (tabela_simbolos.find(label_name[i]) != tabela_simbolos.end())
                {
                    final_exc[saida[j]] = final_exc[saida[j]] + tabela_simbolos.find(label_name[i])->second;
                }
            }
        }
    }
    /* cout << "\n";
    for (int i = 0; i < final_exc.size(); i++)
    {
        cout << final_exc[i] << " ";
    }
    cout << "\n";
    cout << "tabela definicao" << "\n";
    for (int i = 0; i < tabela_definicao.size(); i++)
    {
        cout << tabela_definicao[i].first << " " << tabela_definicao[i].second << "\n";
    }
    cout << "\n";
    cout << "tabela uso" << "\n";
    for (int i = 0; i < tabela_uso.size(); i++)
    {
        cout << tabela_uso[i].first << " " << tabela_uso[i].second << "\n";
    }
    cout << "\n";
    cout << "relaticos" << "\n";
    for (int i = 0; i < relativos.size(); i++)
    {
        cout << relativos[i] << "\n";
    } */
    arq(tabela_uso, tabela_definicao, relativos, final_exc);
    //passa_arq_obj(final_exc);
}

/*
    A main faz o tratamento da entrada do programa, escolhendo as funcoes para rodar e o tipo de arquivo a ser pego
    caso aconteca algum erro em -o uma flag eh acionada e ao ter o return na funcao e voltar para a main se a flag eh 0
    o programa encerra salvando o Log.txt
*/
int main(int argc, char *argv[])
{
    FILE *program;
    char num[80];
    int i = 1;
    if (argc == 2)
    {
        nome_prog = argv[1];
        string path = string(argv[1]) + ".asm";
        strcpy(num, path.c_str());
        program = fopen(num, "rb");
        prog(program, 1);
        cria_arquivo("Log", ".txt", erros);
        passa_arq();
        passagem_unica();
    }else if(argc >= 2){
        nome_prog = argv[1];
        string path = string(argv[1]) + ".asm";
        strcpy(num, path.c_str());
        program = fopen(num, "rb");
        prog(program, 1);
        cria_arquivo("Log", ".txt", erros);
        passa_arq();
        passagem_unica_ligador();
    }
    /* for(int i = 0; i < tudo.size(); i++){
        cout << tudo[i].size();
        for(int j = 0; j < tudo[i].size(); j++){
            cout << tudo[i][j]+" ";
        }
        cout << "\n";
    } */
    // pre_processamento();
    /* for(int i = 0; i < tudo.size(); i++){
        for(int j = 0; j < tudo[i].size(); j++){
            cout << tudo[i][j]+" ";
        }
        cout << "\n";
    } */
    //}
    /* else if(string(argv[1]) == "-m"){
        string path = string(argv[2])+".pre";
        strcpy(num, path.c_str());
        program = fopen(num, "rt");
        prog(program, 2);
        macros();
    }
    else if(string(argv[1]) == "-o"){
        string path = string(argv[2])+".mcr";
        strcpy(num, path.c_str());
        program = fopen(num, "rt");
        prog(program, 3);
        if (e) return 0;
        primeira_passada();
        if (e) return 0;
        cout << " a";
        segunda_passada();
        if (e) return 0;
    } */
}