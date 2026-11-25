
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <Windows.h>
using namespace std;

enum Specialnist
{
    KOMP_NAUKY, INFORMATYKA, MATEM_EKON, FIZ_INF, TRUD_NAVCH
};

struct Student
{
    string prizv;
    int kurs = 0;
    Specialnist spec = KOMP_NAUKY;
    int fizyka = 0;
    int matematyka = 0;

    union
    {
        int prohramuvannya = 0;
        int chyselni_metody;
        int pedahohika;
    };
};

int InputInt(const string& prompt, int lo, int hi)
{
    string line;
    int x = 0;
    char extra;

    while (true)
    {
        cout << prompt;
        if (!getline(cin, line))
            throw runtime_error("Input ended");

        istringstream in(line);
        if (in >> x && !(in >> extra) && x >= lo && x <= hi)
            return x;

        cout << "Помилка! Введіть число від "
            << lo << " до " << hi << ".\n";
    }
}

string InputPrizv()
{
    string s;
    while (true)
    {
        cout << "Прізвище: ";
        if (!getline(cin, s))
            throw runtime_error("Input ended");

        if (s.find_first_not_of(" \t") != string::npos && s.size() <= 15)
            return s;

        cout << "Некоректне прізвище!\n";
    }
}

void Create(Student* p, int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "\nСтудент № " << i + 1 << endl;

        p[i].prizv = InputPrizv();
        p[i].kurs = InputInt("Курс (1-6): ", 1, 6);

        p[i].spec = static_cast<Specialnist>(InputInt(
            "0 - Комп'ютерні науки\n"
            "1 - Інформатика\n"
            "2 - Математика та економіка\n"
            "3 - Фізика та інформатика\n"
            "4 - Трудове навчання\n"
            "Спеціальність: ", 0, 4));

        p[i].fizyka = InputInt("Фізика: ", 1, 5);
        p[i].matematyka = InputInt("Математика: ", 1, 5);

        switch (p[i].spec)
        {
        case KOMP_NAUKY:
            p[i].prohramuvannya = InputInt("Програмування: ", 1, 5);
            break;
        case INFORMATYKA:
            p[i].chyselni_metody = InputInt("Чисельні методи: ", 1, 5);
            break;
        default:
            p[i].pedahohika = InputInt("Педагогіка: ", 1, 5);
        }
    }
}

void Print(const Student* p, int N, const string* specStr,
    const int* I = nullptr)
{
    cout << "\nСПИСОК СТУДЕНТІВ\n";
    cout << string(127, '=') << endl;

    cout << "| " << setw(3) << left << "№"
        << " | " << setw(15) << "Прізвище"
        << " | " << setw(4) << "Курс"
        << " | " << setw(23) << "Спеціальність"
        << " | " << setw(6) << "Фізика"
        << " | " << setw(10) << "Математика"
        << " | " << setw(13) << "Програмування"
        << " | " << setw(15) << "Чисельні методи"
        << " | " << setw(10) << "Педагогіка" << " |\n";

    cout << string(127, '-') << endl;

    for (int i = 0; i < N; i++)
    {
        const Student& s = p[I ? I[i] : i];

        cout << "| " << setw(3) << right << i + 1
            << " | " << setw(15) << left << s.prizv
            << " | " << setw(4) << right << s.kurs
            << " | " << setw(23) << left << specStr[s.spec]
            << " | " << setw(6) << right << s.fizyka
            << " | " << setw(10) << s.matematyka
            << " | " << setw(13)
            << (s.spec == KOMP_NAUKY ? to_string(s.prohramuvannya) : "")
            << " | " << setw(15)
            << (s.spec == INFORMATYKA ? to_string(s.chyselni_metody) : "")
            << " | " << setw(10)
            << (s.spec != KOMP_NAUKY && s.spec != INFORMATYKA
                ? to_string(s.pedahohika) : "") << " |\n";
    }

    cout << string(127, '=') << endl;
}

// Порівняння студентів
int Compare(const Student& a, const Student& b, bool descending)
{
    if (a.kurs != b.kurs)
        return a.kurs < b.kurs ? -1 : 1;

    if (a.matematyka != b.matematyka)
        return a.matematyka < b.matematyka ? -1 : 1;

    // Порівняння прізвищ за українським алфавітом
    int c = CompareStringA(0x0422, 0,
        a.prizv.c_str(), -1, b.prizv.c_str(), -1);

    if (c == 0 || c == CSTR_EQUAL)
        c = a.prizv.compare(b.prizv);
    else
        c -= CSTR_EQUAL;

    if (c == 0)
        return 0;

    return (c < 0) != descending ? -1 : 1;
}

// Фізичне сортування методом бульбашки
void Sort(Student* p, int N)
{
    for (int i = 0; i < N - 1; i++)
        for (int j = 0; j < N - i - 1; j++)
            if (Compare(p[j], p[j + 1], true) > 0)
                swap(p[j], p[j + 1]);
}

// Бінарний пошук
int BinSearch(const Student* p, int N,
    const string& prizv, int kurs, int matematyka)
{
    Student key;
    key.prizv = prizv;
    key.kurs = kurs;
    key.matematyka = matematyka;

    int L = 0, R = N - 1;

    while (L <= R)
    {
        int m = L + (R - L) / 2;
        int c = Compare(p[m], key, true);

        if (c == 0)
            return m;

        if (c < 0)
            L = m + 1;
        else
            R = m - 1;
    }

    return -1;
}

// Індексне сортування методом вставок
int* IndexSort(const Student* p, int N)
{
    int* I = new int[N];

    for (int i = 0; i < N; i++)
        I[i] = i;

    for (int i = 1; i < N; i++)
    {
        int value = I[i];
        int j = i - 1;

        while (j >= 0 && Compare(p[I[j]], p[value], false) > 0)
        {
            I[j + 1] = I[j];
            j--;
        }

        I[j + 1] = value;
    }

    return I;
}

int Menu()
{
    cout << "\nМЕНЮ\n"
        << "1 - Введення даних\n"
        << "2 - Виведення таблиці\n"
        << "3 - Фізичне сортування\n"
        << "4 - Бінарний пошук\n"
        << "5 - Індексне сортування\n"
        << "0 - Вихід\n";

    return InputInt("Ваш вибір: ", 0, 5);
}

int main()
{
    if (!SetConsoleCP(1251) || !SetConsoleOutputCP(1251))
        return 1;

    const string specStr[] = {
        "Комп'ютерні науки",
        "Інформатика",
        "Математика та економіка",
        "Фізика та інформатика",
        "Трудове навчання"
    };

    Student* p = nullptr;

    try
    {
        int N = InputInt("Кількість студентів: ", 1, 1000);
        p = new Student[N];

        int menuItem;
        bool created = false;

        do
        {
            menuItem = Menu();

            if (!created && menuItem != 1 && menuItem != 0)
            {
                cout << "Спочатку введіть дані!\n";
                continue;
            }

            switch (menuItem)
            {
            case 1:
                Create(p, N);
                created = true;
                break;

            case 2:
                Print(p, N, specStr);
                break;

            case 3:
                Sort(p, N);
                Print(p, N, specStr);
                break;

            case 4:
            {
                Sort(p, N);

                string prizv = InputPrizv();
                int kurs = InputInt("Курс: ", 1, 6);
                int mat = InputInt("Математика: ", 1, 5);

                int found = BinSearch(p, N, prizv, kurs, mat);

                if (found != -1)
                    cout << "Студента знайдено в позиції "
                    << found + 1 << endl;
                else
                    cout << "Студента не знайдено.\n";

                break;
            }

            case 5:
            {
                int* I = IndexSort(p, N);
                Print(p, N, specStr, I);
                delete[] I;
                break;
            }
            }
        } while (menuItem != 0);
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
        delete[] p;
        return 1;
    }

    delete[] p;
    return 0;
}
