//// Vector.cpp: implementation of the CVector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector.h"
# include  <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVector::CVector() {
	nSize=0;
	element=NULL;
	}

CVector::CVector(int size) {
	nSize=size;
	element=new double[nSize];
	for(int i=0;i<nSize;i++) element[i]=0.0l;
	}


CVector::CVector(CVector &v) {
	nSize=v.getSize();
	element=new double[nSize];
	for(int i=0;i<nSize;i++) element[i]=v.get(i);
	}

CVector::CVector(CVector *v) {
	nSize=v->getSize();
	element=new double[nSize];
	for(int i=0;i<nSize;i++) element[i]=v->get(i);
	}

CVector::CVector(FILE* fp,int n) {
	nSize=n;
	element=new double[nSize];
	for(int i=0;i<nSize;i++) fscanf(fp,"%lf",&element[i]);
	}


double dot(CVector& a,CVector& b) {
	double sum=0.0l;
	for(int i=0;i<a.getSize();i++) {
		sum+=a.get(i)*b.get(i);
		}
	return sum;
	}

double dot(CVector& a,CVector *b) {
	return dot(a,*b);
	}
/*
double dot(CVector* a,CVector& b) {
	return dot(*a,b);
	}
*/
double dot(CVector* a,CVector* b) {
	return dot(*a,*b);
	}

void CVector::reset() {
	for(int i=0;i<nSize;i++) {
		put(0.0l,i);
		}
	}
void CVector::fix(char *fmt) {
	char str[30];
	for(int i=0;i<nSize;i++) {
		sprintf(str,fmt,element[i]);
		sscanf(str,"%lf",&element[i]);
		}
	}

double CVector::get(int n) {
	if(n<nSize) {
	   return element[n];
	   }
	return 0.0l;
	}


void CVector::Centrolize(CVector& bar,CVector& sigma) {
	ASSERT((nSize==bar.getSize())&&(nSize==sigma.getSize()));
	for(int i=0;i<nSize;i++) put((get(i)-bar.get(i))/sigma.get(i),i);
	}



double CVector::getCentrol() {
	if(nSize==0) return 0.0l;
	double sum=0;
	for(int i=0;i<nSize;i++) sum+=get(i);
	return sum/double(nSize);
	}

void CVector::put(double ev,int n) {
	if(n<nSize) element[n]=ev;
	}
void CVector::addput(double ev,int n) {
	if(n<nSize) element[n]+=ev;
	}

CVector::~CVector() {
	if(nSize>0&&element!=NULL) delete[] element;
	}

void CVector::init(int size) {
	if(element!=NULL) delete[] element;
	nSize=size;
	if(nSize>0) {
		element=new double[nSize];
		for(int i=0;i<nSize;i++) element[i]=0.0l;
		}
	else element=NULL;
	}

void CVector::Zeros() {
	for(int i=0;i<nSize;i++) element[i]=0.0l;
	}

double CVector::mode() {
	double sum=0.0l;
	for(int i=0;i<nSize;i++) {
		sum+=element[i]*element[i];
		}
	return sum;
	}

CString CVector::Format(char* format) {
	CString outStr,tempStr;
	outStr="(";
	for(int i=0;i<nSize;i++) {
		tempStr.Format(format,element[i]);
		outStr+=tempStr;
		if(i!=(nSize-1)) outStr+=",";
		}
	outStr+=")";
	return outStr; 
	}

void CVector::Serialize(CArchive& ar) {
	if(ar.IsLoading()) {
		int nsize;
		ar>>nsize;
		init(nsize);
		for(int i=0;i<nsize;i++) {
			ar>>element[i];
			}
		}
	else {
		ar<<nSize;
		for(int i=0;i<nSize;i++) {
			ar<<element[i];
			}
		}
	}

double CVector::sigma() {
	double sum=mode();
	return sqrt(sum);///double(nSize-1));
	}

CVector& CVector::operator= (CVector& m) {
	if(nSize==m.getSize())
		for(int i=0;i<nSize;i++) put(m.get(i),i);
	return *this;
	}

CVector& CVector::operator= (CVector* m) {
	if(nSize==m->getSize())
		for(int i=0;i<nSize;i++) put(m->get(i),i);
	return *this;
	}

CVector& CVector:: operator*= (double lamda) {
   int i;
   for(i=0;i<nSize;i++) put(get(i)*lamda,i);
   return *this;
   }

CVector& CVector:: operator-= (double lamda) {
   int i;
   for(i=0;i<nSize;i++)
	   put(get(i)-lamda,i);
   return *this;
   }

CVector& CVector::operator+=(double v) {
   for(int i=0;i<nSize;i++) addput(v,i);
   return *this;
   }


CVector& CVector::operator+=(CVector& m) {
   for(int i=0;i<nSize;i++) {
	   element[i]+=m.get(i);
	   }
   return *this;
   }


CVector& CVector::operator+=(CVector* m) {
   for(int i=0;i<nSize;i++) {
	   element[i]+=m->get(i);
	   }
   return *this;
   }

CVector& CVector::operator-=(CVector* m) {
   for(int i=0;i<nSize;i++) {
	   element[i]-=m->get(i);
	   }
   return *this;
   }


CVector& CVector::operator-=(CVector& m) {
   for(int i=0;i<nSize;i++) {
	   element[i]-=m.get(i);
	   }
   return *this;
   }

void CVector::Random(double range) {
	for(int i=0;i<nSize;i++) element[i]=range*double(rand()-RAND_MAX/2)/double(RAND_MAX)*2.0;
	}

