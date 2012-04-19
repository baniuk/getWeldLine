#include "stdafx.h"

/// sprawdzanie sortowania
TEST(SDK, quick_select_case1) {
	C_Matrix_Container obj(1,5);
	obj.data[0] = 1;
	obj.data[1] = 2;
	obj.data[2] = 3;
	obj.data[3] = 4;
	obj.data[4] = 5;

	double med = obj.quick_select();
	EXPECT_EQ(3,med);

}

TEST(SDK, quick_select_case2) {
	C_Matrix_Container obj(1,6);
	obj.data[0] = 1;
	obj.data[1] = 2;
	obj.data[2] = 3;
	obj.data[3] = 4;
	obj.data[4] = 5;
	obj.data[5] = 6;

	double med = obj.quick_select();
	EXPECT_EQ(3,med);

}

TEST(SDK, quick_select_case3) {
	C_Matrix_Container obj(1,7);
	obj.data[0] = 1;
	obj.data[1] = 5;
	obj.data[2] = 9;
	obj.data[3] = -4;
	obj.data[4] = 3;
	obj.data[5] = 1;
	obj.data[5] = 1;

	double med = obj.quick_select();
	EXPECT_EQ(1,med);

}