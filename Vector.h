// Vector.h: interface for the CVector class.
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR_H__7020E145_F846_4EE9_84C1_D87F74D906BF__INCLUDED_)
#define AFX_VECTOR_H__7020E145_F846_4EE9_84C1_D87F74D906BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVector : public CObject  {
	  int nSize;
	  double *element;
  public:
	  void Centrolize(CVector& bar,CVector& sigma);
	  void Random(double range);
	  CVector();
	  CVector(int size);
	  CVector(CVector* v);
	  CVector(CVector& v);
	  CVector(FILE* fp,int size);
	  ~CVector();
      void reset();
	  double getCentrol();
	  void init(int size);
	  int getSize() {return nSize;}
	  double get(int n);
	  void fix(char *fmt);
	  void put(double ev,int n);
	  CString Format(char *format);
	  void addput(double ev,int n);
	  double mode();
	  double sigma();
	  CVector& operator+=(CVector& m);
	  CVector& operator-=(CVector& m);
	  CVector& operator+= (double v);
	  CVector& operator+=(CVector* m);
	  CVector& operator-=(CVector* m);
	  CVector& operator= (CVector& m);
	  CVector& operator-= (double v);
	  CVector& operator= (CVector* m);
	  CVector& operator*= (double lamda);
	  friend double dot(CVector& v1,CVector& v2);
	  friend double dot(CVector& v1,CVector& v2);
	  friend double dot(CVector& v1,CVector* v2);
	  friend double dot(CVector* v1,CVector* v2);
	  virtual void Serialize(CArchive& ar);
	  void Zeros();	
	};

double dot(CVector& v1,CVector& v2);
double dot(CVector& v1,CVector& v2);
double dot(CVector& v1,CVector* v2);
double dot(CVector* v1,CVector* v2);

#endif // !defined(AFX_VECTOR_H__7020E145_F846_4EE9_84C1_D87F74D906BF__INCLUDED_)
