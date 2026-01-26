#include <print>
#include <vector>

class WzorElementu
{
public:
    virtual ~WzorElementu() = default;

    void SetTyp(int typ) { typ_ = typ; }
    void SetWysokosc(int wysokosc) { wysokosc_ = wysokosc; }
    void SetSzerokosc(int szerokosc) { szerokosc_ = szerokosc; }

    int GetTyp() { return typ_; }
    int GetWys() { return wysokosc_; }
    int GetSzer() { return szerokosc_; }

private:
    int typ_;
    int wysokosc_;
    int szerokosc_;
};

class Element : public WzorElementu
{
public:
    Element() : pozycjaX_{ 0 }, pozycjaY_{ 0 } { SetTyp(0), SetSzerokosc(0), SetWysokosc(0); }
    Element(int typ, int X, int Y, int wysokosc, int szerokosc) : pozycjaX_{ X }, pozycjaY_{ Y } { SetTyp(typ), SetWysokosc(wysokosc), SetSzerokosc(szerokosc); }
    ~Element() = default;

    void Drukuj() {
        std::print("\n\n");
        for (int i = 0; i < GetWys(); i++)
        {
            for (int j = 0; j < GetSzer(); j++)
            {
                std::print("{} ", GetTyp());
            }
            std::print("\n");
        }
    }

    int GetX() { return pozycjaX_; }
    int GetY() { return pozycjaY_; }

    void Set_pozycjaX(int x) { pozycjaX_ = x; }
    void Set_pozycjaY(int y) { pozycjaY_ = y; }

private:
    int pozycjaX_;
    int pozycjaY_;
};

std::vector <Element> ObiektyGry()
{
    std::vector <Element> retval;

    Element kwadrat1{ 11,2,1,2,2 };
    Element kwadrat2{ 2,1,5,1,1 };
    Element kwadrat3{ 3,2,4,1,1 };
    Element kwadrat4{ 4,3,4,1,1 };
    Element kwadrat5{ 5,4,5,1,1 };

    Element prostokat1{ 6,1,1,2,1 };
    Element prostokat2{ 7,1,3,2,1 };
    Element prostokat3{ 8,4,1,2,1 };
    Element prostokat4{ 9,4,3,2,1 };
    Element prostokat5{ 1,2,3,1,2 };

    retval.push_back(kwadrat1);
    retval.push_back(kwadrat2);
    retval.push_back(kwadrat3);
    retval.push_back(kwadrat4);
    retval.push_back(kwadrat5);

    retval.push_back(prostokat1);
    retval.push_back(prostokat2);
    retval.push_back(prostokat3);
    retval.push_back(prostokat4);
    retval.push_back(prostokat5);

    return retval;
}
