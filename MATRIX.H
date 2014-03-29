// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__65A3BD74_4E85_41EF_8637_3F88881B578D__INCLUDED_)
#define AFX_MATRIX_H__65A3BD74_4E85_41EF_8637_3F88881B578D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

 
# include "Vector.h"

class CMatrix : public CObject {
public:
	  int nLine,nColumn;
	  CVector* element;
	public:
		void Zeros();
	   void AddToLines(CVector *v);
	   CMatrix* Normalize(double low,double high);
	   void DeNormalize(CMatrix& m,double low,double high);	 
	   void Random(double range);
	   void swapLine(int l1,int l2);
	   void primX(double lamda,int line);
	   int findMaxLineinColumn(int fromLine,int inColumn,double& maine);
	   void Transfer(int targetLine,int sourceLine,double factor);
	   CMatrix* inverse();
	   CMatrix(int line,int column);
	   void init(int line,int column);
	   CMatrix(int line,double lamda=1.00);
	   CMatrix(CMatrix& m);
	   CMatrix(CMatrix& m,int line);
	   CMatrix(CMatrix* m);
	   CMatrix(CMatrix* m,int line);
	   CMatrix(CVector* u,CVector *v);
	   CMatrix() {
			nLine=0;
			nColumn=0;
			element=NULL;
			}
	   ~CMatrix();
	   CMatrix *kickVector(int n);
	   int getLines() {return nLine;}
	   int getColumns() {return nColumn;}
	   double get(int line,int column);
	   CVector *getLine(int line);
	   CVector *getColumn(int column);
	   void centrol(CVector& aBar,CVector& sigma);
	   void deCentrol(CVector& aBar,CVector& sigma);
	   void centrol(CVector* aBar,CVector* sigma);
	   void deCentrol(CVector* aBar,CVector* sigma);
	   void put(double ev,int line,int column);
	   void addput(double ev,int line,int column);
	   CMatrix* transmit();
	   void reset();
	   CMatrix& Reverse(CMatrix& a);
	   CMatrix& operator+= (CMatrix& a);
	   CMatrix& operator+= (CMatrix* a);
	   CMatrix& operator*= (double lamda);
	   CMatrix& operator+= (double lamda);
	   CMatrix& operator= (CMatrix* a);
	   CMatrix& operator= (CMatrix& a);
	   friend CMatrix* operator+ (CMatrix& a,CMatrix& b);
	   friend CMatrix* operator+ (CMatrix* a,CMatrix& b);
	   friend CMatrix* operator+ (CMatrix& a,CMatrix* b);
	   friend CMatrix* operator* (CMatrix& a,CMatrix& b);
	   friend CMatrix* operator* (CMatrix* a,CMatrix& b);
	   friend CMatrix* operator* (CMatrix& a,CMatrix* b);
	   CVector* operator* (CVector *v);
	   CVector* operator* (CVector& v);
	   void fix(char *fmt);
	   virtual void Serialize(CArchive& ar);
	};

  CMatrix* operator+ (CMatrix& a,CMatrix& b);
  CMatrix* operator+ (CMatrix* a,CMatrix& b);
  CMatrix* operator+ (CMatrix& a,CMatrix* b);
  CMatrix* operator* (CMatrix& a,CMatrix& b);
  CMatrix* operator* (CMatrix* a,CMatrix& b);
  CMatrix* operator* (CMatrix& a,CMatrix* b);


  CVector* reduce(CVector& v,CMatrix& m);
  CVector* reduce(CVector* v,CMatrix* m);
  CVector* reduce(CVector* v,CMatrix* m);
  CVector* reduce(CVector* v,CMatrix& m);

  CVector* reduce(CMatrix& v,CVector& m);
  CVector* reduce(CMatrix* v,CVector* m);
  CVector* reduce(CMatrix* v,CVector* m);
  CVector* reduce(CMatrix* v,CVector& m);

/*
  CMatrix *cross(CVector& u,CVector& v);
  CMatrix *cross(CVector* u,CVector& v);
  CMatrix *cross(CVector& u,CVector* v);
  CMatrix *cross(CVector* u,CVector* v);
*/

#endif // !defined(AFX_MATRIX_H__65A3BD74_4E85_41EF_8637_3F88881B578D__INCLUDED_)
