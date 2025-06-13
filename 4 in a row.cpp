#include <bits/stdc++.h>

using namespace std;

const int RED = 6, KOL = 7, DUB = 4;
const char IGRAC = 'X', AI = 'O', PRAZNO = '.'; 

vector<vector<char>> tabla(RED, vector<char>(KOL, PRAZNO)); // deklaracija table

void printTabla() { // printovanje table prolazeci kroz svako polje
    for (auto& red : tabla) {
        for (char polje : red) cout << polje << ' ';
        cout << '\n';
    }
    for (int i = 0; i < KOL; i++) cout << i << ' ';
    cout << '\n';
}

bool validanpotez(int c) { return tabla[0][c] == PRAZNO; } // ako je odredjeno polje prazno, moze se odigrati potez u njega

bool pravipotez(int c, char p) { // u prvo prazno polje se upisuje potez igraca ili ai
    for (int r = RED - 1; r >= 0; r--){
        if (tabla[r][c] == PRAZNO){
            tabla[r][c] = p;
            return true;
        }
    }
    return false;
}

void vratipotez(int c) { // praznjenje polja da bi se dalje isprobavale kombinacije za najbolji moguci rezultat
    for (int r = 0; r < RED; r++)
        if (tabla[r][c] != PRAZNO) { tabla[r][c] = PRAZNO; break; }
}

bool pobeda(char p) { // proveravanje ako u nekom redu, koloni, ili dijagonalama postoji pobeda
    for (int r = 0; r < RED; r++)
        for (int c = 0; c < KOL - 3; c++)
            if (tabla[r][c] == p && tabla[r][c+1] == p && tabla[r][c+2] == p && tabla[r][c+3] == p)
                return true;
    for (int r = 0; r < RED - 3; r++)
        for (int c = 0; c < KOL; c++)
            if (tabla[r][c] == p && tabla[r+1][c] == p && tabla[r+2][c] == p && tabla[r+3][c] == p)
                return true;
    for (int r = 3; r < RED; r++)
        for (int c = 0; c < KOL - 3; c++)
            if (tabla[r][c] == p && tabla[r-1][c+1] == p && tabla[r-2][c+2] == p && tabla[r-3][c+3] == p)
                return true;
    for (int r = 0; r < RED - 3; r++)
        for (int c = 0; c < KOL - 3; c++)
            if (tabla[r][c] == p && tabla[r+1][c+1] == p && tabla[r+2][c+2] == p && tabla[r+3][c+3] == p)
                return true;
    return false;
}

int score(char p) { // na osnovu scorea se odredjuje u kakvoj se poziciji nalazi igrac ili ai po tome koliko ima svojih elemenata u redu
    int s = 0;
    for (int r = 0; r < RED; r++)
        for (int c = 0; c < KOL - 3; c++) {
            int b = 0;
            for (int i = 0; i < 4; i++) b += (tabla[r][c+i] == p);
            if (b == 4) return 10000;
            else if (b == 3) s += 5;
            else if (b == 2) s += 2;
        }
    return s;
}

int minimax(int d, bool p) {
    if (pobeda(AI)) return 10000;
    if (pobeda(IGRAC)) return -10000;
    if (d == 0) return score(AI) - score(IGRAC); /* nakon zavrsetka gledanja poteza unapred vraca razliku scorea za ai i za igraca,
                                                    ako je pozitivan u prednosti je ai ako je negativan u prednosti je igrac*/

    if (p) {
        int maxprocena = -100000; // 4 poteza unapred isprobava moguce poteze za ai i igraca i time odredjuje procenjenu vrednost te pozicije
        for (int c = 0; c < KOL; c++) {
            if (!validanpotez(c)) continue;
            pravipotez(c, AI);
            int procena = minimax(d-1, false);
            vratipotez(c); 
            maxprocena = max(maxprocena, procena);
        }
        return maxprocena;
    } else {
        int minprocena = 100000;
        for (int c = 0; c < KOL; c++) {
            if (!validanpotez(c)) continue;
            pravipotez(c, IGRAC);
            int procena = minimax(d-1, true);
            vratipotez(c);
            minprocena = min(minprocena, procena);
        }
        return minprocena;
    }
}

int najboljipotez() { // slicno kao minimax, samo sto nakon izvrsavanja proverava i odredjuje najvecu vrednost tj score za optimalni rezultat
    int naj = -1, najvred = -100000;
    for (int c = 0; c < KOL; c++) {
        if (!validanpotez(c)) continue;
        pravipotez(c, AI);
        int vred = minimax(DUB - 1, false);
        vratipotez(c);
        if (vred > najvred) najvred = vred, naj = c;
    }
    return naj;
}

int main() {
    printTabla();
    while (true) {
        int m;
        cout << "Tvoj potez (0-6): "; cin >> m;
        if (!validanpotez(m)) { cout << "Nevažeći potez.\n"; continue; }
        pravipotez(m, IGRAC); printTabla();
        if (pobeda(IGRAC)) { cout << "Pobedio si!\n"; break; }

        int ai = najboljipotez();
        pravipotez(ai, AI); cout << "AI bira " << ai << "\n";
        printTabla();
        if (pobeda(AI)) { cout << "AI je pobedio!\n"; break; }
    }
    return 0;
}
