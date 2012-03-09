/**
 * \file GTEST_ADDONS.h
 *\brief Plik zawiera funkcje do testowania wi�kszych obiekt�w do u�ycia wewnatrz test�w google
 */
 
 /** \brief Funkcja obs�uguj�ca por�wnaina punkt�w - obiekt�w C_Point
 * Funkcja por�wnuje P0==P1
 */ 
void C_Point_CMP_DOUBLE(const C_Point P0, const C_Point P1)
{
	double x0,y0,x1,y1;
	P0.getPoint(x0,y0);
	P1.getPoint(x1,y1);
	ASSERT_DOUBLE_EQ(x0, x1);
	ASSERT_DOUBLE_EQ(y0, y1);
}

/** \brief Funkcja obs�uguj�ca por�wnaina punkt�w - obiekt�w C_Point
 * Funkcja por�wnuje P0==P1
 */ 
void C_Point_CMP(const C_Point P0, const C_Point P1)
{
	double x0,y0,x1,y1;
	P0.getPoint(x0,y0);
	P1.getPoint(x1,y1);
	EXPECT_EQ(x0, x1);
	EXPECT_EQ(y0, y1);
}

