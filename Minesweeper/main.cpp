#include <iostream>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "iocolor.h"

using namespace std;

#define MIN 5
#define MAX 20

struct Audio {
    bool sottofondo=true;
    bool speciali=true;
};

struct Punto {
    int r;
    int c;
};

struct Gioc{
    char nome[11];
    int turni=0;
    int bombe;
    int segni=0;
    int aperte=0;
    Punto lung;
    Audio suoni;
};

Gioc menu();
Gioc cinGioc(Gioc stats);
Punto scelta(Gioc stats, bool &azione, int tab[][MAX], bool &intorno); //azione= true aprire false segnare
bool checkScelta(Punto coord, bool azione, int tab[][MAX], bool intorno, Gioc &stats); //false se c'è una bomba, true altrimenti
bool checkWin(bool check, int tab[][MAX], Gioc stats); //true se si vince, false altrimenti
bool checkSegni(Gioc stats, int tab[][MAX]);
void sconfitta(Gioc stats);
void vittoria(Gioc stats);
void coutTab(int tab[][MAX], Gioc stats);
void inizTab(int tab[][MAX], Gioc stats); //n+20 coperto | n+10 segnato | n scoperto
void coutElementi(int tab[][MAX], Gioc stats);
void zeroApri(int r, int c, int tab[][MAX], int maxr, int maxc, int caso, Gioc &stats);

int main() {
    system("mode con: cols=160 lines=100");
    srand(time(NULL));
    int tab[MAX][MAX];
    Gioc stats;
    bool check=true, win=false, azione=false, intorno=false;
    Punto coord;
    stats=menu();
    inizTab(tab, stats);
    coutTab(tab, stats);
    do {
        coord=scelta(stats, azione, tab, intorno);
        check=checkScelta(coord, azione, tab, intorno, stats);
        win=checkWin(check, tab, stats);
        coutTab(tab, stats);
        stats.turni++;
    } while (check&&!win);
    if (!check) {
        sconfitta(stats);
    } else {
        vittoria(stats);
    }
}

void coutTab (int tab[][MAX], Gioc stats) {
    int i, j;
    system("cls");
    setColor(0,15);
    gotoXY(80-(stats.lung.c+1)*3/2, 3);
    cout << " ";
    for (i=0; i<stats.lung.c; i++) {
        if (i<9) {
            cout << "0";
        }
        cout << i+1 << " ";
    }
    gotoXY(80-(stats.lung.c+1)*3/2, 5);
    setColor(8,7);
    for (i=0; i<stats.lung.r*2+1; i++) {
        for (j=0; j<stats.lung.c; j++) {
            if (i==0) {
                if (j==0) {
                    putchar(218);
                } else if (j==stats.lung.c-1) {
                    putchar(196);
                    putchar(196);
                    putchar(194);
                    putchar(196);
                    putchar(196);
                    putchar(191);
                } else {
                    putchar(196);
                    putchar(196);
                    putchar(194);
                }
            } else if (i==stats.lung.r*2) {
                if (j==0) {
                    putchar(192);
                } else if (j==stats.lung.c-1) {
                    putchar(196);
                    putchar(196);
                    putchar(193);
                    putchar(196);
                    putchar(196);
                    putchar(217);
                } else {
                    putchar(196);
                    putchar(196);
                    putchar(193);
                }
            } else if (i%2==0) {
                if (j==0) {
                    putchar(195);
                } else if (j==stats.lung.c-1) {
                    putchar(196);
                    putchar(196);
                    putchar(197);
                    putchar(196);
                    putchar(196);
                    putchar(180);
                } else {
                    putchar(196);
                    putchar(196);
                    putchar(197);
                }
            } else {
                if (j==0) {
                    putchar(179);
                } else if (j==stats.lung.c-1) {
                    cout << "  ";
                    putchar(179);
                    cout << "  ";
                    putchar(179);
                } else {
                    cout << "  ";
                    putchar(179);
                }
            }
        }
        setColor(0,15);
        cout << endl << " ";
        if (i%2==0&&i<stats.lung.r*2) {
            gotoXY(79-(stats.lung.c+2)*3/2, i+6);
            putchar (i/2+65);
            cout << " ";
        } else {
            cout << "  ";
        }
        gotoXY(80-(stats.lung.c+1)*3/2, i+6);
        setColor(8,7);
    }
    coutElementi(tab, stats);
}

void inizTab (int tab[][MAX], Gioc stats) {
    int i=0, j=0, x=0;
    for (i=0; i<stats.lung.r; i++) {
        for (j=0; j<stats.lung.c; j++) {
            tab[i][j]=0;
        }
    }
    do {
        i=rand()%stats.lung.r;
        j=rand()%stats.lung.c;
        if (tab[i][j]!=9) {
            tab[i][j]=9;
            x++;
        }
    } while (x<stats.bombe);
    x=0;
    for (i=0; i<stats.lung.r; i++) {
        for (j=0; j<stats.lung.c; j++) {
            if (tab[i][j]<9) {
                if (tab[i-1][j-1]==9&&i>=1&&j>=1) tab[i][j]++;
                if (tab[i-1][j]==9&&i>=1) tab[i][j]++;
                if (tab[i-1][j+1]==9&&i>=1&&j<stats.lung.c-1) tab[i][j]++;
                if (tab[i][j-1]==9&&j>=1) tab[i][j]++;
                if (tab[i][j+1]==9&&j<stats.lung.c-1) tab[i][j]++;
                if (tab[i+1][j-1]==9&&i<stats.lung.r-1&&j>=1) tab[i][j]++;
                if (tab[i+1][j]==9&&i<stats.lung.r-1) tab[i][j]++;
                if (tab[i+1][j+1]==9&&i<stats.lung.r-1&&j<stats.lung.c-1) tab[i][j]++;
            }
        }
    }
    for (i=0; i<stats.lung.r; i++) {
        for (j=0; j<stats.lung.c; j++) {
            tab[i][j]+=20;
        }
    }
}

void coutElementi(int tab[][MAX], Gioc stats) {
    int r, c;
    for (r=0; r<stats.lung.r; r++) {
        for (c=0; c<stats.lung.c; c++) {
            gotoXY(81-(stats.lung.c+1)*3/2+3*c, r*2+6);
            if (tab[r][c]>=20) {
                setColor(8,8);
                putchar(219);
                putchar(219);
            } else if (tab[r][c]>=10) { //segnato
                setColor(8,4);
                putchar(179);
                putchar(16);
            } else {
                switch (tab[r][c]) {
                case 0:
                    setColor(7,7);
                    putchar(219);
                    putchar(219);
                    break;
                case 9:
                    setColor(12,0);
                    putchar(220);
                    putchar(223);
                    break;
                case 8:
                    setColor(7,15);
                    cout << " 8";
                    break;
                case 7:
                    setColor(7,14);
                    cout << " 7";
                    break;
                case 4:
                    setColor(7,9);
                    cout << " 4";
                    break;
                default:
                    setColor(7,tab[r][c]);
                    cout << " " << tab[r][c];
                    break;
                }
            }
        }
    }
    setColor(0,15);
}

Gioc menu() {
    int menu;
    bool uscita=false;
    Gioc stats;
    do {
        system ("Cls");
        cout << "Che cosa vuoi fare?" << endl;
        cout << "\nGioca[1]\nRegolamento[2]\nSuoni[3]\n";
        cin >> menu;
        switch (menu){
        case 3:
            do{
                gotoXY(0,9);
                cout << " ";
                gotoXY(0,6);
                if (stats.suoni.sottofondo){
                    cout << "Sottofondi[1]: ";
                    cout << "ON " << endl;
                } else{
                    cout << "Sottofondi[1]: ";
                    cout << "OFF" << endl;
                }
                if (stats.suoni.speciali){
                    cout << "Speciali[2]: ";
                    cout << "ON " << endl;
                } else{
                    cout << "Speciali[2]: ";
                    cout << "OFF" << endl;
                }
                cout << "Esci[3]" << endl;
                cin >> menu;
                switch (menu){
                case 1:
                    if (stats.suoni.sottofondo==true){
                        stats.suoni.sottofondo=false;
                        sndPlaySound(NULL, SND_ASYNC);
                    }
                    else{
                        stats.suoni.sottofondo=true;
                        sndPlaySound("suoni\\menu.wav", SND_ASYNC | SND_LOOP);
                    }
                    break;
                case 2:
                    if (stats.suoni.speciali==true){
                        stats.suoni.speciali=false;
                    }
                    else{
                        stats.suoni.speciali=true;
                    }
                    break;
                case 3:
                    uscita=true;
                }
            }while(!uscita);
            uscita=false;
            break;
        case 2:
            do {
                gotoXY(0,10);
                cout << " ";
                gotoXY(0,12);
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                cout << "                                                                                  " << endl;
                gotoXY(0,6);
                cout << "Info basilari[1]\nComandi[2]\nGriglia[3]\nEsci[4]" << endl;
                cin >> menu;
                gotoXY(0,12);
                switch (menu){
                case 1:
                    cout << "Giocatori: 1\nDurata media:<5-30 minuti>" << endl;
                    cout << "\nLo scopo del gioco e' di individuare (e segnare) tutte le bombe presenti.";
                    cout << "\nLe bombe vengono piazzate casualmente all'inizio della partita." << endl;
                    cout << "\nIl giocatore potra' selezionare una casella da visualizzare o da segnare";
                    cout << "\nLe caselle in prossimita' di una bomba saranno contrassegnate da un numero,";
                    cout << "\nche indica quante bombe sono presenti nelle 8 caselle adiacenti" << endl;
                    break;
                case 2:
                    cout << "\nPer scegliere una casella, e' necessario indicare la riga\ne la colonna corrispondenti";
                    cout << "\nSuccessivamente si dovrà decidere se svelare la casella,";
                    cout << "\ncontrassegnarla con una bandiera o rimuovere la bandiera\nin caso sia gia' contrassegnata.";
                    cout << "\nIl numero di bombe e la grandezza della griglia vengono\nselti all'inizio dal giocatore" << endl;
                    break;
                case 3:
                    cout << "\nSulla griglia, le zone segnate saranno indicate con il rosso,\ncon rosso e nero se e' stata trovata una bomba,";
                    cout << "\ncon il grigio scuro se la casella e' coperta\ne con un numero altrimenti" << endl;
                    break;
                case 4:
                    uscita=true;
                }
                system("Pause");
            } while (!uscita);
            uscita=false;
            break;
        case 1:
            stats=cinGioc(stats);
            uscita=true;
        }
    } while (uscita==false);
    return stats;
}

Gioc cinGioc(Gioc stats){
    system("cls");
    cout << "Inserisci il tuo nome [MAX 10]: ";
    cin.ignore();
    cin.getline(stats.nome,11);
    do {
        system("cls");
        cout << "Inserisci il numero di righe [MIN " << MIN << " MAX " << MAX << "]: ";
        cin >> stats.lung.r;
    } while (stats.lung.r>MAX||stats.lung.r<MIN);
    do {
        system("cls");
        cout << "Inserisci il numero di colonne [MIN " << MIN << " MAX " << MAX << "]: ";
        cin >> stats.lung.c;
    } while (stats.lung.c>MAX||stats.lung.c<MIN);
    do {
        system("cls");
        cout << "Inserisci il numero di bombe [MAX " << stats.lung.r*stats.lung.c*2/3 << "]: ";
        cin >> stats.bombe;
    } while (stats.bombe>stats.lung.r*stats.lung.c*2/3);
    return stats;
}

Punto scelta(Gioc stats, bool &azione, int tab[][MAX], bool &intorno) {
    Punto coord;
    int men=0;
    char r;
    while (men!=1&&men!=2) {
        intorno=false;
        men=0;
        do {
            cout << endl << "Inserisci la riga (lettera): "; //mettere gotoXY sulla base delle righe totali
            cin >> r;
            coord.r=r-96;
        } while (coord.r<1||coord.r>stats.lung.r);
        coord.r--;
        do {
            cout << "Inserisci la colonna: ";
            cin >> coord.c;
        } while (coord.c<1||coord.c>stats.lung.c);
        coord.c--;
        cout << endl;
        if (tab[coord.r][coord.c]>19) {
            while (men!=1&&men!=2&&men!=3) {
                cout << "\nChe cosa vuoi fare?\nApri casella[1]\nSegna casella[2]\nCambia selezione[3]" << endl;
                cin >> men;
            }
        } else if (tab[coord.r][coord.c]>9) {
            while (men!=1&&men!=2) {
                cout << "\nChe cosa vuoi fare?\nCancella segno[1]\nCambia selezione[2]" << endl;
                cin >> men;
            }
            men++;
        } else {
            while (men!=1&&men!=2) {
                cout << "\nChe cosa vuoi fare?\nApri circostanti[1]\nCambia selezione[2]" << endl;
                cin >> men;
            }
            men++;
            if (men==2) {
                intorno=true;
            }
        }
    }
    if (men==1) {
        azione=false;
    } else if (men==2) {
        azione=true;
    }
    return coord;
}

bool checkScelta(Punto coord, bool azione, int tab[][MAX], bool intorno, Gioc &stats) {
    bool scelta=true;
    if ((tab[coord.r][coord.c]==20||tab[coord.r][coord.c]==10)&&!azione) {
        zeroApri(coord.r, coord.c, tab, stats.lung.r, stats.lung.c, 0, stats);
    } else if (intorno) {
        zeroApri(coord.r, coord.c, tab, stats.lung.r, stats.lung.c, 1, stats);
        if (tab[coord.r-1][coord.c-1]==9||tab[coord.r-1][coord.c]==9||tab[coord.r-1][coord.c+1]==9||tab[coord.r][coord.c-1]==9||tab[coord.r][coord.c+1]==9||tab[coord.r+1][coord.c-1]==9||tab[coord.r+1][coord.c]==9||tab[coord.r+1][coord.c+1]==9) {
            scelta=false;
        }
    } else if (azione) {
        if (tab[coord.r][coord.c]>19) {
            tab[coord.r][coord.c]-=10;
            stats.segni++;
        } else if (tab[coord.r][coord.c]>9) {
            tab[coord.r][coord.c]+=10;
            stats.segni--;
        }
    } else {
        tab[coord.r][coord.c]-=20;
        if (tab[coord.r][coord.c]==9) {
            scelta=false;
        } else {
            stats.aperte++;
        }
    }
    return scelta;
}

bool checkWin(bool check, int tab[][MAX], Gioc stats) {
    int i, j;
    bool win=false;
    if(!check) {
        for (i=0; i<stats.lung.r; i++) {
            for (j=0; j<stats.lung.c; j++) {
                if (tab[i][j]==19||tab[i][j]==29)
                tab[i][j]=9;
            }
        }
    } else if (stats.lung.c*stats.lung.r==stats.aperte+stats.bombe) {
        win=true;
    } else if (stats.segni==stats.bombe) {
        win=checkSegni(stats, tab);
    }
    return win;
}

void zeroApri(int r, int c, int tab[][MAX], int maxr, int maxc, int caso, Gioc &stats) {
    int i=0, j=0, n=0;
    for(n=0; n<9; n++) {
        switch (n) {
        case 1:
            i=-1;
            j=-1;
            break;
        case 2:
            i=-1;
            j=0;
            break;
        case 3:
            i=-1;
            j=1;
            break;
        case 4:
            i=0;
            j=-1;
            break;
        case 5:
            i=0;
            j=1;
            break;
        case 6:
            i=1;
            j=-1;
            break;
        case 7:
            i=1;
            j=0;
            break;
        case 8:
            i=1;
            j=1;
            break;
        }
        if (r+i>=0&&r+i<maxr&&c+j>=0&&c+j<maxc) {
            if(tab[r+i][c+j]==10||tab[r+i][c+j]==20) {
                tab[r+i][c+j]=0;
                stats.aperte++;
                zeroApri(r+i, c+j, tab, maxr, maxc, 0, stats);
            } else if ((caso==0&&tab[r+i][c+j]>9)||(caso==1&&tab[r+i][c+j]>19)) {
                tab[r+i][c+j]%=10;
                stats.aperte++;
            }
        }
    }
}

bool checkSegni(Gioc stats, int tab[][MAX]) {
    bool win=false;
    int i, j, n=0;
    for (i=0; i<stats.lung.r; i++) {
        for (j=0; j<stats.lung.c; j++) {
            if (tab[i][j]==19) {
                n++;
            }
        }
    }
    if (n==stats.bombe) {
        win=true;
    }
    return win;
}

void sconfitta(Gioc stats) {
    gotoXY(74, stats.lung.r*2+15);
    setColor(4, 0);
    cout << "Hai perso!";
    setColor(0, 15);
    gotoXY(74, stats.lung.r*2+25);
}

void vittoria(Gioc stats) {
    system("cls");
    gotoXY(70, 10);
    cout << "Vittoria!";
    gotoXY(70, 15);
    cout << stats.nome << " :";
    gotoXY(74, 17);
    cout << "Dimensioni: " << stats.lung.r << " x " << stats.lung.c;
    gotoXY(74, 19);
    cout << "Bombe: " << stats.bombe;
    gotoXY(74, 21);
    cout << "Turni: " << stats.turni;
    gotoXY(74, 23);
    cout << "Punteggio: " << 100 * stats.bombe * stats.bombe / (stats.lung.c * stats.lung.r * stats.turni);
}
