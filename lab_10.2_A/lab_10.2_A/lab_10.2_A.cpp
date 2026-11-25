
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <Windows.h>

using namespace std;

enum Specialty
{
    COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS,
    PHYSICS_INFORMATICS, LABOR_TRAINING
};

struct Student
{
    string surname;
    int course = 0;
    Specialty specialty = COMPUTER_SCIENCE;
    int physics = 0;
    int math = 0;
    int informatics = 0;
};

int InputInt(const string& prompt, int low, int high)
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

        if (in >> x && !(in >> extra) && x >= low && x <= high)
            return x;

        cout << "Помилка! Повторіть введення.\n";
    }
}

string InputSurname()
{
    string s;

    while (true)
    {
        cout << "Прізвище: ";

        if (!getline(cin, s))
            throw runtime_error("Input ended");

        if (s.find_first_not_of(" \t") != string::npos && s.size() <= 16)
            return s;

        cout << "Прізвище має містити від 1 до 16 символів.\n";
    }
}

string GetSpecialtyName(Specialty s)
{
    switch (s)
    {
    case COMPUTER_SCIENCE: return "Комп'ютерні науки";
    case INFORMATICS: return "Інформатика";
    case MATH_ECONOMICS: return "Математика та економіка";
    case PHYSICS_INFORMATICS: return "Фізика та інформатика";
    case LABOR_TRAINING: return "Трудове навчання";
    }
    return "";
}

void Create(Student* p, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << "\nСтудент №" << i + 1 << endl;

        p[i].surname = InputSurname();
        p[i].course = InputInt("Курс (1-6): ", 1, 6);

        cout << "0 - Комп'ютерні науки\n"
            << "1 - Інформатика\n"
            << "2 - Математика та економіка\n"
            << "3 - Фізика та інформатика\n"
            << "4 - Трудове навчання\n";

        p[i].specialty = static_cast<Specialty>(
            InputInt("Спеціальність: ", 0, 4));

        p[i].physics = InputInt("Фізика: ", 1, 5);
        p[i].math = InputInt("Математика: ", 1, 5);
        p[i].informatics = InputInt("Інформатика: ", 1, 5);
    }
}

void Print(const Student* p, int n, const int* I = nullptr)
{
    cout << "\nСПИСОК СТУДЕНТІВ\n";
    cout << string(95, '=') << endl;

    cout << "| " << setw(3) << left << "№"
        << " | " << setw(16) << "Прізвище"
        << " | " << setw(4) << "Курс"
        << " | " << setw(23) << "Спеціальність"
        << " | " << setw(6) << "Фізика"
        << " | " << setw(10) << "Математика"
        << " | " << setw(11) << "Інформатика" << " |\n";

    cout << string(95, '-') << endl;

    for (int i = 0; i < n; i++)
    {
        const Student& s = p[I ? I[i] : i];

        cout << "| " << setw(3) << right << i + 1
            << " | " << setw(16) << left << s.surname
            << " | " << setw(4) << right << s.course
            << " | " << setw(23) << left
            << GetSpecialtyName(s.specialty)
            << " | " << setw(6) << right << s.physics
            << " | " << setw(10) << s.math
            << " | " << setw(11) << s.informatics << " |\n";
    }

    cout << string(95, '=') << endl;
}

// Порівняння студентів за трьома ключами
int Compare(const Student& a, const Student& b, bool descending)
{
    if (a.course != b.course)
        return a.course < b.course ? -1 : 1;

    if (a.math != b.math)
        return a.math < b.math ? -1 : 1;

    int c = CompareStringA(0x0422, 0,
        a.surname.c_str(), -1, b.surname.c_str(), -1);

    if (c == 0)
        c = a.surname.compare(b.surname);
    else
        c -= CSTR_EQUAL;

    if (c == 0)
        return 0;

    return (c < 0) != descending ? -1 : 1;
}

// Фізичне сортування методом бульбашки
void Sort(Student* p, int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (Compare(p[j], p[j + 1], true) > 0)
                swap(p[j], p[j + 1]);
}

// Побудова індексного масиву
int* IndexSort(const Student* p, int n)
{
    int* I = new int[n];

    for (int i = 0; i < n; i++)
        I[i] = i;

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (Compare(p[I[j]], p[I[j + 1]], false) > 0)
                swap(I[j], I[j + 1]);

    return I;
}

// Бінарний пошук
int BinSearch(const Student* p, int n,
    const string& surname, int course, int math)
{
    Student key;
    key.surname = surname;
    key.course = course;
    key.math = math;

    int L = 0, R = n - 1;

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

    Student* p = nullptr;

    try
    {
        int n = InputInt("Кількість студентів: ", 1, 1000);
        p = new Student[n];

        bool created = false;
        int choice;

        do
        {
            choice = Menu();

            if (!created && choice != 1 && choice != 0)
            {
                cout << "Спочатку введіть дані студентів!\n";
                continue;
            }

            switch (choice)
            {
            case 1:
                Create(p, n);
                created = true;
                break;

            case 2:
                Print(p, n);
                break;

            case 3:
                Sort(p, n);
                Print(p, n);
                break;

            case 4:
            {
                Sort(p, n);

                string surname = InputSurname();
                int course = InputInt("Курс: ", 1, 6);
                int math = InputInt("Оцінка з математики: ", 1, 5);

                int found = BinSearch(p, n, surname, course, math);

                if (found != -1)
                    cout << "Студента знайдено. Позиція: "
                    << found + 1 << endl;
                else
                    cout << "Студента не знайдено.\n";
                break;
            }

            case 5:
            {
                int* I = IndexSort(p, n);
                Print(p, n, I);
                delete[] I;
                break;
            }
            }
        } while (choice != 0);
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
