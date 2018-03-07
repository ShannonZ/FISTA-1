#include "stdafx.h"
#include "CppUnitTest.h"
#include "fista.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(NormalCase_proj_l1)
		{
			// Normal case for proj_l1
			for (int iteration = 0; iteration < 10; iteration++)
			{
				// TODO: Your test code here
				mat U = randu<mat>(100, 700);

				for (double lambda = 0; lambda < 1.0; lambda += 0.05) {
					mat result = proj_l1(U, lambda, true);
					result = proj_l1(U, lambda, false);
				}
			}
		}
		TEST_METHOD(IncorrectLambdaCase_proj_l1)
		{
			// Incorrect Lambda value for proj_l1
			wchar_t message[200];

			mat U = randu<mat>(100, 700);
			bool pos = true;

			for (double lambda = 1.1; lambda < 2.0; lambda += 0.5) 
			{		
				try
				{
					mat result = proj_l1(U, lambda, pos);
				}
				catch (std::out_of_range ex)
				{
					continue;
				}
			}

			for ( double lambda = -2.0; lambda < 0.0; lambda += 0.1) {
				try
				{
					mat result = proj_l1(U, lambda, pos);
				}
				catch (std::out_of_range ex)
				{
					continue;
				}
			}
		}
	};
}