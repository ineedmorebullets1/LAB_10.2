#include "pch.h"
#include "CppUnitTest.h"

#define main LabMain
#include "C:\Users\aizen\Documents\lab c++\Lab_10\lab_10.2_A\lab_10.2_A\lab_10.2_A.cpp"
#undef main

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest102A
{
    TEST_CLASS(UnitTest102A)
    {
    public:
        TEST_METHOD(TestBinSearch)
        {
            Student p[3] =
            {
                {"Ivanenko", 1, COMPUTER_SCIENCE, 5, 4, 5},
                {"Petrenko", 2, INFORMATICS, 4, 5, 4},
                {"Shevchenko", 3, MATH_ECONOMICS, 5, 3, 4}
            };

            Sort(p, 3);

            Assert::AreEqual(1,
                BinSearch(p, 3, "Petrenko", 2, 5));

            Assert::AreEqual(-1,
                BinSearch(p, 3, "Petrenko", 2, 3));
        }
    };
}