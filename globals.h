char distribucion[8][8]={
    {'t','c','a','q','r','a','c','t'},
    {'p','p','p','p','p','p','p','p'},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {'P','P','P','P','P','P','P','P'},   
    {'T','C','A','R','Q','A','C','T'}
};
char tablero[8][8];
char pos[4],pos2[4];
bool seleccionado = false,coronacion=false,enroque=false;
bool condicionCaptura=false;
bool torreBlancaMovida[2]={false,false},reyBlancoMovido=false;
bool torreNegraMovida[2]={false,false},reyNegroMovido=false;
bool enrocar=false,intentoCaptura=false;
int pos_ant_b[2], pos2_ant_b[2];
int pos_ant_n[2], pos2_ant_n[2];

int turno = 1,random,random2,hayJaque,ganador=0;

char repetir;