#include "pch.h"
#include "CppUnitTest.h"

#define main LabMain
#include "C:\Users\aizen\Documents\lab c++\Lab_10\lab_10.2_B\lab_10.2_B\lab_10.2_B.cpp"
#undef main

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest102B
{
    TEST_CLASS(UnitTest102B)
    {
    public:
        TEST_METHOD(TestBinSearch)
        {
            Student p[3];

            p[0].prizv = "Ivanenko";
            p[0].kurs = 1;
            p[0].matematyka = 4;

            p[1].prizv = "Petrenko";
            p[1].kurs = 2;
            p[1].matematyka = 5;

            p[2].prizv = "Shevchenko";
            p[2].kurs = 3;
            p[2].matematyka = 3;

            Sort(p, 3);

            Assert::AreEqual(1,
                BinSearch(p, 3, "Petrenko", 2, 5));

            Assert::AreEqual(-1,
                BinSearch(p, 3, "Petrenko", 2, 3));
        }
    };
}