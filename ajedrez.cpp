#include <iostream>
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "./globals.h"

using namespace std;

void mostrarTablero(char tablero[8][8]);
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

struct posicion{
    int x;
    int y;
};

posicion getPosicion(char pos[4]);
void setColorCasilla(int i,int j);

void pedirPieza();
void pedirDestino();

bool posicionValida(char pos[4]);
string nombrePieza();
char colorPieza(char posTablero);
bool validarMovimiento();
bool destinoValido();
void realizarMovimiento();

bool movimientosPeon();
bool movimientosCaballo();
bool movimientosTorre();
bool movimientosAlfil();
bool movimientosRey();
bool validarEnroque();

void movimientoNegras();
int jaque();
bool evitarJaque();

void jugadaBlancas();
void jugadaNegras();
int determinarGanador();

main(){
    srand(time(NULL));
    system("color 80");

    do{
        for (size_t i = 0; i < 8; i++)
            strcpy(tablero[i],distribucion[i]);
          
        do{

            turno%2!=0 ? jugadaBlancas() : jugadaNegras();

            if(pos2[0]!='0')
                ++turno;

        }while(determinarGanador()==0);

        system("cls");
        cout<<"GANARON LAS "<<(ganador==1 ? "BLANCAS" : "NEGRAS")<<"\n";
        cout<<"Ingrese 1 para salir: ";
        cin>>repetir;

    } while(repetir!='1');

    system("cls");
    cout<<"PROGRAMA FINALIZADO CON EXITO!!!\n";
    
    system("pause");
    return 0;
}

void mostrarTablero(char tablero[8][8]){
    if(hayJaque==1){
        cout<<"JAQUE A LAS NEGRAS!!!\n";
    }
    if(hayJaque==2){
        cout<<"JAQUE A LAS BLANCAS!!!\n";
    }
    
    cout<<"\t\t\t\t\t\tAJEDREZ\n";
    cout<<"\t\t\t\t\t\tTurno "<<turno<<"\n\n";

    cout<<"\t\t      a\t       b\tc\t d\t  e        f        g        h";
    SetConsoleTextAttribute(hConsole,136);

    cout<<"\n\t\t  ";
    SetConsoleTextAttribute(hConsole,136);
    cout<<"\n";

    for (int i = 0; i < 8; i++) {
        cout<<"\t\t  ";
        for (int j = 0; j < 8; j++) {
            SetConsoleTextAttribute(hConsole,136);

            setColorCasilla(i,j);
            
            cout<<"         ";
        }
        SetConsoleTextAttribute(hConsole,136);

        cout <<"\n\t";
        SetConsoleTextAttribute(hConsole,128);
        cout<<"       "<<8-i<<"  ";

        for (int j = 0; j < 8; j++) {
            SetConsoleTextAttribute(hConsole,136);

            setColorCasilla(i,j);

            cout<<"    ";
            
            posicion posPieza = getPosicion(pos);
            if(seleccionado && i==posPieza.x && j==posPieza.y)
                SetConsoleTextAttribute(hConsole,70);

            cout<<tablero[i][j];

            setColorCasilla(i,j);

            cout <<"    ";
        }
        SetConsoleTextAttribute(hConsole,136);

        SetConsoleTextAttribute(hConsole,128);
        cout<<"  "<<8-i;
        cout<<"\n\t\t  ";

        for (int j = 0; j < 8; j++) {
            SetConsoleTextAttribute(hConsole,136);

            setColorCasilla(i,j);
            cout<<"         ";
        }
        SetConsoleTextAttribute(hConsole,136);

        cout<<"\n";

    }
    SetConsoleTextAttribute(hConsole,128);
    cout<<"\n\t\t      a\t       b\tc\t d\t  e        f        g        h";
}

posicion getPosicion(char pos[4]){

    return {56-int(pos[1]),int(pos[0])-97};
}

void setColorCasilla(int i,int j){
    char pieza = tablero[i][j];

    if((i+1)%2==0){
        if((j+1)%2==0)
        
            SetConsoleTextAttribute(hConsole,isupper(pieza)?1:4);
        else
            SetConsoleTextAttribute(hConsole,isupper(pieza)?241:244);

    }
    else{
        if((j+1)%2==0)
            SetConsoleTextAttribute(hConsole,isupper(pieza)?241:244);
        else
            SetConsoleTextAttribute(hConsole,isupper(pieza)?1:4);

    }
}

void pedirPieza(){
    do
    {
        mostrarTablero(tablero);
        cout<<"\n\nIngrese que pieza quiere mover [a-g][1-8]: ";        
        cin>>pos;
        fflush(stdin);
        system("cls");
    } while (!posicionValida(pos));

    posicion posPieza = getPosicion(pos);

    // determinar si la pieza escogida es blanca
    if(colorPieza(tablero[posPieza.x][posPieza.y])!='b')
        pedirPieza();

    return;
}

bool posicionValida(char pos[4]){
    if(strlen(pos)!=2)
        return false;
    if(strlwr(pos)[0]<97 || strlwr(pos)[0]>104)
        return false;
    if(pos[1]<49 || pos[1]>56)
        return false;

    return true;
}

void pedirDestino(){

    do
    {
        mostrarTablero(tablero);
        cout<<"\n\nIngrese a donde quiere mover ";
        cout<<nombrePieza();
        cout<<" [a-g][1-8] o 0 para volver a escoger pieza: ";        
        cin>>pos2;

        system("cls");
        fflush(stdin);

        if(pos2[0]=='0')
            return;

    } while (!posicionValida(pos2));

    //si el destino es valido se realiza el movimiento
    validarMovimiento() ? realizarMovimiento() : pedirDestino();
    return;
}

string nombrePieza(){
    posicion posPieza = getPosicion(pos);
    int i = posPieza.x, j =posPieza.y;

    switch (tablero[i][j])
    {
        case 'P':
            return "el peon";
        case 'T':
            return "la torre";
        case 'C':
            return "el caballo";
        case 'A':
            return "el alfil";
        case 'Q':
            return "la reina";
        case 'R':
            return "el rey";
    }
}

char colorPieza(char pieza){
    if(islower(pieza))
        return 'n';
    if(isupper(pieza))
        return 'b';

    return 'v';
}

bool validarMovimiento(){
    intentoCaptura=false;
    posicion posPieza = getPosicion(pos);
    int i = posPieza.x,j=posPieza.y;
    char pieza = tablero[i][j];

    if(islower(pieza))
        pieza = int(pieza)-32;

    //evalua los posibles movimientos de las piezas
    switch (pieza)
    {
        case 'P':
            return movimientosPeon();
        case 'T':
            return movimientosTorre();
        case 'C':
            return movimientosCaballo();
        case 'A':
            return movimientosAlfil();
        case 'Q':
            return movimientosTorre()+movimientosAlfil();
        case 'R':
            return movimientosRey();
        default:
            return false;
    }
}

bool movimientosPeon(){
    posicion posPieza = getPosicion(pos);
    int i = posPieza.x,j=posPieza.y;
    posicion posPieza2 = getPosicion(pos2);
    int i2 = posPieza2.x,j2=posPieza2.y;
    if(colorPieza(tablero[i][j])=='b'){

        if(i==6){

            if(i-2==i2 && j==j2 && tablero[i-2][j]==' ' && tablero[i-1][j]==' ')
                return true;
        }

        if(i-1==i2 && j==j2 && tablero[i-1][j]==' ')
            return true;
        
        if(i-1==i2 && (j-1==j2 || j+1==j2)){

            if(colorPieza(tablero[i2][j2])=='n')
                return true;
            
            //evalua si hay captura al paso
            if(tablero[i][j2]=='p' && tablero[i2][j2]==' '){

                if(pos2_ant_n[0]==i && pos2_ant_n[1]==j2){

                    if(pos2_ant_n[0]==pos_ant_n[0]+2){

                        tablero[i][j2]=' ';
                        return true;
                    }
                }
            }
        }
        return false;
    }
    else{

        if(i==1){

            if(i+2==i2 && j==j2 && tablero[i+2][j]==' ' && tablero[i+1][j]==' ')
                return true;
            
        }
        if(i+1==i2 && j==j2 && tablero[i+1][j]==' ')
            return true;
        
        if(i+1==i2 && (j-1==j2 || j+1==j2)){

            if(colorPieza(tablero[i2][j2])=='b')
                return true;

            //evalua si hay captura al paso
            if(tablero[i][j2]=='P' && tablero[i2][j2]==' '){

                if(pos2_ant_b[0]==i && pos2_ant_b[1]==j2){

                    if(pos2_ant_b[0]==pos_ant_b[0]-2){

                        intentoCaptura=true;
                        tablero[i][j2]=' ';
                        return true;
                    }
                }
            }
        }

        return false;      
    }
}

bool movimientosTorre(){
    posicion posPieza = getPosicion(pos);
    int i = posPieza.x,j=posPieza.y;
    posicion posPieza2 = getPosicion(pos2);
    int i2 = posPieza2.x,j2=posPieza2.y;

    if(i2>i && j==j2){
        for (int k = i+1; k <i2 ; k++){
            if(colorPieza(tablero[k][j])!='v')
                return false;
        }
    }
    else if(i2<i && j==j2){

        for (int k = i-1; k>i2 ; k--){
            if(colorPieza(tablero[k][j])!='v')
                return false;
        }
    }
    else if(i==i2 && j2>j){

        for (int k = j+1; k <j2 ; k++){
            if(colorPieza(tablero[i][k])!='v')
                return false;
        }
    }
    else if(i==i2 && j2<j){

        for (int k = j-1; k>j2 ; k--){
            if(colorPieza(tablero[i][k])!='v')
                return false;
        }
    }
    else
        return false;
    
    return destinoValido();
}

bool movimientosCaballo(){
    posicion posPieza = getPosicion(pos);
    int i = posPieza.x,j=posPieza.y;
    posicion posPieza2 = getPosicion(pos2);
    int i2 = posPieza2.x,j2=posPieza2.y;
        
    if((i2==i+2 || i2==i-2) && (j2==j+1 || j2==j-1))
        return destinoValido();

    if((i2==i+1 || i2==i-1) && (j2==j+2 || j2==j-2))
        return destinoValido();

    return false;
}

bool movimientosAlfil(){
    posicion posPieza = getPosicion(pos);
    int i = posPieza.x,j=posPieza.y;
    posicion posPieza2 = getPosicion(pos2);
    int i2 = posPieza2.x,j2=posPieza2.y;

    if(i2>i && j2>j && abs(i2-i)==abs(j2-j)){
        for (size_t k = 1; k <i2-i ; k++){
            if(colorPieza(tablero[i+k][j+k])!='v')
                return false;

        }
    }
    else if(i2>i && j2<j && abs(i2-i)==abs(j2-j)){
        for (size_t k = 1; k <i2-i ; k++){
            if(colorPieza(tablero[i+k][j-k])!='v')
                return false;

        }
    }
    else if(i2<i && j2>j && abs(i2-i)==abs(j2-j)){
        for (size_t k = 1; k <j2-j ; k++){
            if(colorPieza(tablero[i-k][j+k])!='v')
                return false;

        }
    }
    else if(i2<i && j2<j && abs(i2-i)==abs(j2-j)){
        for (size_t k = 1; k <j-j2 ; k++){
            if(colorPieza(tablero[i-k][j-k])!='v')
                return false;

        }
    }
    else
        return false;
        
    return destinoValido();
}

bool movimientosRey(){
    posicion posPieza = getPosicion(pos);
    int i = posPieza.x,j=posPieza.y;
    posicion posPieza2 = getPosicion(pos2);
    int i2 = posPieza2.x,j2=posPieza2.y;
    int posReyRival[4];

    //consigue la posicion de ambos reyes 
    for (size_t k = 0; k < 8; k++)
    {
        for (size_t l = 0; l < 8; l++)
        {
            if (colorPieza(tablero[i][j])=='b' && tablero[k][l]=='r')
            {
                posReyRival[0]=k;
                posReyRival[1]=l;
            }
            if (colorPieza(tablero[i][j])=='n' && tablero[k][l]=='R')
            {
                posReyRival[0]=k;
                posReyRival[1]=l;
            }            
        }
        
    }
    //impide que los reyes esten en casillas contiguas
    if(abs(i2-posReyRival[0])<=1 && abs(j2-posReyRival[1])<=1)
        return false;

    if((abs(i-i2)==1 && abs(j-j2)<=1) || (abs(j-j2)==1 && abs(i-i2)<=1))
        return destinoValido();

    enrocar = validarEnroque();
    return enrocar;
}

bool validarEnroque(){
    posicion posPieza = getPosicion(pos);
    int i = posPieza.x,j=posPieza.y;    
    posicion posPieza2 = getPosicion(pos2);
    int i2 = posPieza2.x,j2=posPieza2.y;

    if(colorPieza(tablero[i][j])=='b'){
        if(!reyBlancoMovido){
            if(i2==7 && j2==1 && !torreBlancaMovida[0]){
                if(tablero[7][1]==' ' && tablero[7][2]==' ')
                    return true;
                
            }
            if (i2==7 && j2==5 && !torreBlancaMovida[1])
            {
                if(tablero[7][4]==' ' && tablero[7][5]==' ' && tablero[7][6]==' ')
                    return true;

            }
            
        }
    }
    else{
        if(!reyNegroMovido){
            if(i2==0 && j2==2 && !torreNegraMovida[0]){
                if(tablero[0][1]==' ' && tablero[0][2]==' ' && tablero[0][3]==' ')
                    return true;
    
            }
            if (i2==0 && j2==6 && !torreNegraMovida[1])
            {
                if(tablero[0][5]==' ' && tablero[0][6]==' ')
                    return true;

            } 
        }
    }

    return false;
}

bool destinoValido(){
    posicion posPieza = getPosicion(pos);
    posicion posPieza2 = getPosicion(pos2);
    int i = posPieza.x, j = posPieza.y;
    int i2 = posPieza2.x,j2=posPieza2.y;

    if(colorPieza(tablero[i][j])=='b' && colorPieza(tablero[i2][j2])!='b')
        return true;
    if(colorPieza(tablero[i][j])=='n' && colorPieza(tablero[i2][j2])!='n')
        return true;

    return false;   
}

void realizarMovimiento(){
    posicion posPieza = getPosicion(pos);
    int i = posPieza.x,j=posPieza.y;
    posicion posPieza2 = getPosicion(pos2);
    int i2 = posPieza2.x,j2=posPieza2.y;

    tablero[i2][j2]=tablero[i][j];
    tablero[i][j]=' ';

    //coronacion de las blancas
    if(tablero[i2][j2]=='P' && i2==0){
        char nuevaPieza;
        cout<<"Ingrese la letra de la pieza a la que quiere cambiar: ";
        cin>>nuevaPieza;
        tablero[i2][j2]=nuevaPieza;
    }
    //coronacion de las negras
    if(tablero[i2][j2]=='p' && i2==7)
        tablero[i2][j2]='q';

    //complemento del enroque
    if(enrocar){
        if(i2==0 && j2==6){
            tablero[0][7]=' ';
            tablero[0][5]='t';
        }
        if(i2==0 && j2==2){
            tablero[0][0]=' ';
            tablero[0][3]='t';
        }
        if(i2==7 && j2==1){
            tablero[7][0]=' ';
            tablero[7][2]='T';
        }
        if(i2==7 && j2==5){
            tablero[7][7]=' ';
            tablero[7][4]='T';
        }    
        enrocar=false;    
    }
    //determina si las torres o rey han sido movidos
    if(tablero[0][0]!='t')
        torreNegraMovida[0]=true;
    if(tablero[0][7]!='t')
        torreNegraMovida[1]=true;
    if(tablero[7][0]!='T')
        torreBlancaMovida[0]=true;
    if(tablero[7][7]!='T')
        torreBlancaMovida[1]=true;
    if(tablero[0][4]!='r')
        reyNegroMovido=true;
    if(tablero[7][3]!='R')
        reyBlancoMovido=true;

}

void movimientoNegras(){
    char jugadas[16][20][4];
    int cont=0,cont2=0,random=0,random2=0;
    char prioridad[6]={'R','Q','T','A','C','P'};
    posicion posInicio,posFinal;
    char aux,aux2;
    int ordenRandom[16];
    //genera un orden aleatorio para las piezas 
    for (size_t i = 0; i < 16; i++)
    {
        random = rand()%16;
        ordenRandom[i]=random;
        for (size_t j = 0; j <i; j++)
        {
            if (random==ordenRandom[j]){
                i--;
                break;
            }
        } 
    }
    //evita jaque de ser posible
    if (hayJaque==1)
    {
        if (evitarJaque())
            return;
    
    }
    //realiza jaque mate de ser posible
    if(hayJaque==2){
        for (size_t i = 0; i < 8; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                if(tablero[i][j]=='R'){
                    tablero[i][j]=' ';
                    return;
                }
            }  
        }
    }

    for (size_t i = 0; i < 16; i++)
    {
        for (size_t j = 0; j < 20; j++)
            jugadas[i][j][0]='\0';
         
    }
    //llena matriz jugadas de las posiciones de las posibles jugadas
    for (int i = 0; i <8; i++)
    {
        for (int j = 0; j <8; j++)
        {
            if(colorPieza(tablero[i][j])=='n'){
                cont2=0;

                for (int k = 0; k<8; k++)
                {
                    for (int l = 0; l<8; l++)
                    {
                        if(cont2==0){

                            pos[0]=j+97;
                            pos[1]=(7-i)+49;
                            pos[2]='\0';
                            strcpy(jugadas[cont][cont2],pos);
                            cont2++;
                        }

                        pos2[0]=l+97;
                        pos2[1]=(7-k)+49;
                        pos2[2]='\0';

                        if(validarMovimiento()){
                            if(intentoCaptura)
                                tablero[pos2_ant_b[0]][pos2_ant_b[1]]='P';
                            
                            strcpy(jugadas[cont][cont2],pos2);
                            cont2++;
                        }
                    }
                        
                }
                    
                cont++;
            }
        }    
    }

    for (size_t i = 0; i < 6; i++)
    {
        for (size_t j = 0; j < 16; j++)
        {
            for (size_t k = 1;jugadas[ordenRandom[j]][k][0]!='\0'; k++)
            {
                posInicio = getPosicion(jugadas[ordenRandom[j]][0]);
                posFinal = getPosicion(jugadas[ordenRandom[j]][k]);

                strcpy(pos,jugadas[ordenRandom[j]][0]);
                strcpy(pos2,jugadas[ordenRandom[j]][k]);
                aux = tablero[posInicio.x][posInicio.y];
                aux2 = tablero[posFinal.x][posFinal.y];

                realizarMovimiento();
                //evalua si deja en jaque para descartarla
                if(jaque()==1){
                    tablero[posInicio.x][posInicio.y]=aux;
                    tablero[posFinal.x][posFinal.y]=aux2; 
                    strcpy(pos,jugadas[ordenRandom[j]][0]);
                    strcpy(pos2,jugadas[ordenRandom[j]][k]);                    
                    if(validarMovimiento()){

                        if(intentoCaptura)
                            tablero[pos2_ant_b[0]][pos2_ant_b[1]]='P';
                    } 

                    continue;
                }
                //evalua si hace jaque para hacerla
                if(jaque()==2)
                    return;

                tablero[posInicio.x][posInicio.y]=aux;
                tablero[posFinal.x][posFinal.y]=aux2; 

                if(intentoCaptura)  
                    tablero[pos2_ant_b[0]][pos2_ant_b[1]]='P';

                strcpy(pos,jugadas[ordenRandom[j]][0]);
                strcpy(pos2,jugadas[ordenRandom[j]][k]);

                //evalua si se puede eliminar alguna pieza rival
                if(tablero[posFinal.x][posFinal.y]==prioridad[i]){
                    
                    strcpy(pos,jugadas[ordenRandom[j]][0]);
                    strcpy(pos2,jugadas[ordenRandom[j]][k]);
                    realizarMovimiento();
                    return;
                }
                //evalua captura al paso
                if(validarMovimiento() && prioridad[i]=='P' && intentoCaptura){
                    strcpy(pos,jugadas[ordenRandom[j]][0]);
                    strcpy(pos2,jugadas[ordenRandom[j]][k]);
                    realizarMovimiento();   
                    return;                 
                }
            } 
        }
    }
    //si no se cumplen esas condiciones solo se ve si no pone en jaque
    for (size_t i = 0; i < 16; i++)
    {
        for (size_t j = 1; jugadas[ordenRandom[i]][j][0]!='\0'; j++)
        {
            posInicio = getPosicion(jugadas[ordenRandom[i]][0]);
            posFinal = getPosicion(jugadas[ordenRandom[i]][j]);

            strcpy(pos,jugadas[ordenRandom[i]][0]);
            strcpy(pos2,jugadas[ordenRandom[i]][j]);
            aux = tablero[posInicio.x][posInicio.y];
            aux2 = tablero[posFinal.x][posFinal.y];
            realizarMovimiento();   

            if(jaque()==0)
                return;
            
            else{
                tablero[posInicio.x][posInicio.y]=aux;
                tablero[posFinal.x][posFinal.y]=aux2;   
            }   
        }   
    }
    //si no se cumle ninguna condicion hace un movimiento al azar
    do{
        random = rand()%cont;
        random2 = (rand()%19)+1;
    } while(jugadas[random][random2][0]=='\0');
    
    strcpy(pos,jugadas[random][0]);
    strcpy(pos2,jugadas[random][random2]);

    realizarMovimiento();
}

bool evitarJaque(){
    char aux,aux2;

    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            if(colorPieza(tablero[i][j])=='n'){
                for (size_t k = 0; k < 8; k++)
                {
                    for (size_t l = 0; l < 8; l++)
                    {
                        pos[0]=j+97;
                        pos[1]=(7-i)+49;
                        pos2[0]=l+97;
                        pos2[1]=(7-k)+49;
                        if(validarMovimiento()){
                            aux=tablero[i][j];
                            aux2=tablero[k][l];
                            realizarMovimiento();


                            hayJaque=jaque();
                            if(hayJaque==1){
                                tablero[i][j]=aux;
                                tablero[k][l]=aux2;
                                if (intentoCaptura)
                                    tablero[pos2_ant_b[0]][pos2_ant_b[1]]='P';  
                                
                            }
                            else
                                return true;
                            
                        }
                    }
                }  
            }
        } 
    }

    return false;
}

int jaque(){
    for (size_t i = 0; i < 8; i++){

        for (size_t j = 0; j < 8; j++){

            if(tablero[i][j]=='r'){
                pos2[0]=j+97;
                pos2[1]=(7-i)+49;
                break;
            }
        }
        
    }
    for(size_t i = 0; i < 8; i++)
    {
        for(size_t j = 0; j < 8; j++)
        {
            if(colorPieza(tablero[i][j])=='b')
            {
                pos[0]=j+97;
                pos[1]=(7-i)+49;
                
                if (validarMovimiento())
                    return 1;
                
            }
        }
    }
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            if(tablero[i][j]=='R'){
                pos2[0]=j+97;
                pos2[1]=(7-i)+49;
                break;
            }
        }
    }
    for(size_t i = 0; i < 8; i++)
    {
        for(size_t j = 0; j < 8; j++)
        {
            if(colorPieza(tablero[i][j])=='n')
            {
                pos[0]=j+97;
                pos[1]=(7-i)+49;
                if (validarMovimiento())
                    return 2;
                   
            }
        }
    }
    return 0;
}

void jugadaBlancas(){
    pedirPieza();
    seleccionado = true;
    pedirDestino();

    if(pos2[0]=='0'){
        seleccionado = false;
        pos[0]='\0';
        return;
    }

    pos_ant_b[0] = getPosicion(pos).x;
    pos_ant_b[1] = getPosicion(pos).y;
    pos2_ant_b[0] = getPosicion(pos2).x;
    pos2_ant_b[1] = getPosicion(pos2).y;
    hayJaque = jaque();

}

void jugadaNegras(){
    movimientoNegras();
    seleccionado=false;

    pos_ant_n[0] = getPosicion(pos).x;
    pos_ant_n[1] = getPosicion(pos).y;
    pos2_ant_n[0] = getPosicion(pos2).x;
    pos2_ant_n[1] = getPosicion(pos2).y;

    hayJaque = jaque();
}

int determinarGanador(){
    bool blancas=false,negras=false;
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            if(tablero[i][j]=='r'){
                negras=true;
                i=j=10;
            }
        }
        
    }

    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            if(tablero[i][j]=='R'){
                blancas=true;
                i=j=10;
            }
        }
        
    }

    if(!blancas) return 1;

    if(!negras) return 2;
    
    return 0;
}