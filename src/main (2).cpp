#include <iostream>
#include <print>
#include <vector>
#include <algorithm>
#include <ranges>
#include "obiekty_gry.h"
#include "winbgi2.h"

class Plansza
{
public:
    void inicjuj() {
        for (int i = 1; i < 6; i++) { for (int j = 1; j < 5; j++) { plansza_[i][j] = 0; } }  //aby uproœciæ odczytywanie wspó³rzêdnych i zachowaæ w nim 
        czy_przygotowana = true;                                                            //spójnoœæ indeksowanie zaczynamy od 0 
    }

    void drukuj() {
        if (czy_przygotowana)
            for (int i = 1; i < 6; i++) { std::print("\t\t\t\t\t"); for (int j = 1; j < 5; j++) { std::print("{}  ", plansza_[i][j]); } std::print("\n"); }
        //std::print("\n\n\n");
    }

    void wypelnij(int x, int y, int wartosc) { plansza_[x][y] = wartosc; }
    int sprawdz(int x, int y) { return plansza_[x][y]; }

private:
    int plansza_[7][6];                 //plansza wiêksza ni¿ pola do gry o wiersz i kolumnê o indeksach 0 oraz 
    bool czy_przygotowana{ false };     //kolumnê 6 i wiersz 7 - plansza gry jest otoczona "ramk¹" z zer aby uniemo¿liwiæ indeksowanie poza zakres tablicy
};

void rysuj(Element Obiekt)
{
    rectangle(Obiekt.pozycjaX_ * 50, Obiekt.pozycjaY_ * 50, (Obiekt.pozycjaX_ + Obiekt.szerokosc_) * 50, (Obiekt.pozycjaY_ + Obiekt.wysokosc_) * 50);
    line((double)Obiekt.pozycjaX_*50, (double)Obiekt.pozycjaY_*50, (Obiekt.pozycjaX_ + Obiekt.szerokosc_)*50., (Obiekt.pozycjaY_ + Obiekt.wysokosc_)*50.);
    line((Obiekt.pozycjaX_ + Obiekt.szerokosc_) * 50., Obiekt.pozycjaY_ * 50., Obiekt.pozycjaX_ * 50., (Obiekt.pozycjaY_ + Obiekt.wysokosc_) * 50.);
    //rectangle(int left, int top, int right, int bottom)
}

void static UmiescElementNaPlanszy(Plansza& plansza, Element e)
{
    for (int i = e.pozycjaY_; i < e.wysokosc_ + e.pozycjaY_; i++)
    {
        for (int j = e.pozycjaX_; j < e.szerokosc_ + e.pozycjaX_; j++)
        {
            plansza.wypelnij(i, j, e.typ_);
            rysuj(e);
        }
    }
}

void static aktualizacjaPlanszy(Plansza& plansza, std::vector <Element> elementy)
{
    plansza.inicjuj();
    clear();
    setlinestyle(2, 1, 4);
    line(50, 50, 250, 50);
    line(50, 50, 50, 300);
    line(250, 50, 250, 300);
    line(50, 300, 100, 300);
    line(250, 300, 200, 300);
    setlinestyle(1, 3, 1);
    for (Element& e : elementy)
    {
        UmiescElementNaPlanszy(plansza, e);
    }
}

bool CzyLegalny(Plansza p, Element e, char kierunek) // Sprawdza czy w miejscu w które chcemy przesun¹æ element s¹ tylko 0
{
    switch (kierunek)
    {
    case 'd':
        for (int i = e.pozycjaX_; i < e.szerokosc_ + e.pozycjaX_; i++) // sprawdzenie czy na ca³ej szerokoœci obiektu poni¿ej niego s¹ same 0
        {
            if (p.sprawdz(e.pozycjaY_ + e.wysokosc_, i) != 0)
                return false;
        }
        return true;

    case 'g':
        for (int i = e.pozycjaX_; i < e.szerokosc_ + e.pozycjaX_; i++)
        {
            if (p.sprawdz(e.pozycjaY_ - 1, i) != 0)
                return false;
        }
        return true;
    case 'p':
        for (int i = e.pozycjaY_; i < e.wysokosc_ + e.pozycjaY_; i++)
        {
            if (p.sprawdz(i, e.pozycjaX_ + e.szerokosc_) != 0)
                return false;
        }
        return true;
    case 'l':
        for (int i = e.pozycjaY_; i < e.wysokosc_ + e.pozycjaY_; i++)
        {
            int a = e.pozycjaX_ - 1;
            if (p.sprawdz(i, a) != 0)
                return false;
        }
        return true;
    }
    return false;
}

/* auto znajdz(const std::vector< Element >& Obiekty, int typ)
{
    auto iter = std::find_if(Obiekty.begin(), Obiekty.end(), [&](const Element& e) { return e.typ_ == typ; });
    return *iter;
}*/

void ruch(std::vector<Element>& Obiekty, int typ, char kierunek, Plansza& P) // weŸ obiekt o danym typie, sprawdŸ jego po³o¿enie i wymiary, sprawdŸ czy ruch jest legalny, wykonaj ruch
{
    //auto e = &znajdz(Obiekty, typ);
    //Element d = *std::find_if(Obiekty.begin(), Obiekty.end(), [&](const Element& e) { return e.typ_ == typ; });
    //auto e = std::find_if(Obiekty.begin(), Obiekty.end(), [&](const Element& e) { return e.typ_ == typ; });
    try
    {
        auto e = std::ranges::find_if(Obiekty, [&](const Element& e) { return e.typ_ == typ; });

        if (kierunek == 'd' && CzyLegalny(P, *e, kierunek))
            e->pozycjaY_ = (e->pozycjaY_ + 1);
        else if (kierunek == 'g' && CzyLegalny(P, *e, kierunek))
            e->pozycjaY_ = (e->pozycjaY_ - 1);
        else if (kierunek == 'p' && CzyLegalny(P, *e, kierunek))
            e->pozycjaX_ = (e->pozycjaX_ + 1);
        else if (kierunek == 'l' && CzyLegalny(P, *e, kierunek))
            e->pozycjaX_ = (e->pozycjaX_ - 1);
        else
            std::print("\nBledna komenda lub ruch jest nielegalny!\n");

        aktualizacjaPlanszy(P, Obiekty);

        if (e->typ_ == 11 && e->pozycjaX_ == 2 && e->pozycjaY_ == 4)
        {
            std::print("\n\n\t\tWygrana!!\n\n\n");
            wait();
        }

    }
    catch (std::exception& err)
    {
        std::cerr << err.what();
    }

    
}

void gra(std::vector<Element>& Obiekty, Plansza& P)
{
    P.inicjuj();
    aktualizacjaPlanszy(P, Obiekty);
    P.drukuj();

    int komenda_typ = 1;
    char komenda_kier = 'a';
    std::print("Aby wykonac ruch wprowadz numer elementu ktory chcesz przesunac, a w nastepnym kroku kierunek ruchu. \
Elementy maja numery od 1 do 11, dozwolone polecenia kierunkow to: \n\'d\' - dol\n\'g\' - gora\n\'p\' - prawo\n\'l\' - lewo\n\
Aby zakonczyc, zamiast kierunku wpisz \'k\'\n");
    //try
    {
        std::cin >> komenda_typ;
        if (komenda_typ < 1 || komenda_typ>11)
        {
            do
            {
                std::print("Podaj numer elementu mieszczacy sie pomiedzy 1 a 11:\n");
                std::cin >> komenda_typ;
            } while (!(komenda_typ >= 1 && komenda_typ <= 11));
        }
        std::cin >> komenda_kier;
    }
    //catch (std::exception& err)
    {
    //    std::cerr << err.what();
    }
    
    do
    {
        ruch(Obiekty, komenda_typ, komenda_kier, P);
        P.drukuj();
        animate(1);
        std::cin >> komenda_typ;
        if (komenda_typ < 1 || komenda_typ > 11)
        {
            do
            {
                std::print("Podaj numer elementu mieszczacy sie pomiedzy 1 a 11:\n");
                std::cin >> komenda_typ;
            } while (komenda_typ < 1 || komenda_typ > 11);
        }
        std::cin >> komenda_kier;

    } while (komenda_kier != 'k');

    std::print("\n\tZakonczyles gre! Aby wyjsc, kliknij na okno graficzne i wcisnij enter.\n\n");
}

int main()
{
    Plansza plansza;
    std::vector <Element> obiekty = ObiektyGry();
    graphics(350, 400);
    gra(obiekty, plansza); 
    wait();
}

