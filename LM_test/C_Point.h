#ifndef C_Point_h__
#define C_Point_h__

/** 
 * Klasa implementuj¹ca punkt w przestreni dwuwymiarowej
 */ 
class C_Point
{
public:
	C_Point();
	C_Point(double _x, double _y);
	~C_Point();
	/// kopjuje punkt do innego obiektu dest
	void CopyToDest(C_Point &dest);
	/// ustawia wartoœæ punktu
	void setPoint(double _x, double _y);
	/// zwraca wartoœci punktów
	void getPoint(double &_x, double &_y);
private:
	double x0, y0;	// wspó³rzêdne punktu
};
#endif // C_Point_h__
