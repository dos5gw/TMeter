//// Matrix.cpp: implementation of the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matrix.h"
# include <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMatrix::CMatrix(int line,int column) {
	nLine=line;nColumn=column;
	if(nLine>0) {
		element=new CVector[nLine];
		for(int i=0;i<nLine;i++) (element+i)->init(nColumn);
		}
	else element=NULL;
	}

void CMatrix::init(int line,int column) {
	if(element!=NULL) delete[] element;
	nLine=line;nColumn=column;
	if(line>0) {
		element=new CVector[nLine];
		for(int i=0;i<nLine;i++) (element+i)->init(nColumn);
		}
	else element=NULL;
	}

CVector *CMatrix::getLine(int line) {
	if(line<nLine) return (element+line);
	else return NULL;
	}


CVector *CMatrix::getColumn(int column) {
	CVector *c=new CVector(nLine);
	for(int i=0;i<nLine;i++) c->put(get(i,column),i);
	return c;
	}

CMatrix::CMatrix(CMatrix& m) {
	nLine=m.getLines();
	nColumn=m.getColumns();
	element=new CVector[nLine];
	for(int i=0;i<nLine;i++) {
	   (element+i)->init(nColumn);
	   element[i]=m.getLine(i);
	   }
	}

CMatrix::CMatrix(CMatrix& m,int line) {
	if(line>=m.getLines()) line=nLine=m.getLines();
	else nLine=m.getLines()-1;
	nColumn=m.getColumns();
	element=new CVector[nLine];
	for(int i=0;i<line;i++) {
	   (element+i)->init(nColumn);
	   element[i]=m.getLine(i);
	   }
	for(i=line;i<nLine;i++) {
	   (element+i)->init(nColumn);
	   element[i]=m.getLine(i+1);
	   }
	}

CMatrix::CMatrix(CMatrix* m,int line) {
	if(line>=m->getLines()) line=nLine=m->getLines();
	else nLine=m->getLines()-1;
	nColumn=m->getColumns();
	element=new CVector[nLine];
	for(int i=0;i<line;i++) {
	   (element+i)->init(nColumn);
	   element[i]=m->getLine(i);
	   }
	for(i=line;i<nLine;i++) {
	   (element+i)->init(nColumn);
	   element[i]=m->getLine(i+1);
	   }
	}

CMatrix::~CMatrix() {
	if(nLine>0&&element!=NULL) {
		delete[] element;
		}
	}

CMatrix::CMatrix(CMatrix* m) {
	nLine=m->getLines();
	nColumn=m->getColumns();
	if(nLine>0) {
		element=new CVector[nLine];
		for(int i=0;i<nLine;i++) {
			(element+i)->init(nColumn);
			element[i]=m->getLine(i);
			}
		}
	else element=NULL;
	}

CMatrix::CMatrix(CVector *u,CVector *v) {
	int i,j;
	nLine=u->getSize();
	nColumn=v->getSize();
	if(nLine>0) {
		element=new CVector[nLine];
		for(i=0;i<nLine;i++)  {
			(element+i)->init(nColumn);
			for(j=0;j<nColumn;j++)  put(u->get(i)*v->get(j),i,j);
			}
		}
	else element=NULL;
	}

CMatrix::CMatrix(int line,double lamda) {
	int i,j;
	nLine=nColumn=line;
	if(nLine>0) {
		element=new CVector[nLine];
		for(i=0;i<nLine;i++) {
			(element+i)->init(nColumn);
			for(j=0;j<nColumn;j++) {
				if(i==j) put(lamda,i,j);
				else put(0.0l,i,j);
				}
			}
		}
	else element=NULL;
	}

CMatrix* CMatrix::transmit() {
   int i,j;
   CMatrix *mt=new CMatrix(nColumn,nLine);
  
   for(i=0;i<nColumn;i++)
	  for(j=0;j<nLine;j++)
		  mt->put(get(j,i),i,j);
   return mt;
   }

void  CMatrix::swapLine(int l1,int l2) {
	CVector *vl1,*vl2,vt(nColumn);
	vl1=getLine(l1);vl2=getLine(l2);
	vt=vl1;*vl1=vl2;*vl2=vt;
	}

CMatrix *CMatrix::kickVector(int l) {
	CVector *v1,*v2;int i;
	if(l<nLine) {
	   CMatrix *mt=new CMatrix(nLine-1,nColumn);
	   for(i=0;i<l;i++) {
		  v1=mt->getLine(i);
		  v2=getLine(i);
		  *v1=v2;
		  }
	   for(i=l+1;i<nLine;i++) {
		  v1=mt->getLine(i-1);
		  v2=getLine(i);
		  *v1=v2;
		  }
	   return mt;
	   }
	else return NULL;
	}

void CMatrix::primX(double lamda,int line) {
	 if(line<nLine) {
		 CVector *vl=getLine(line);
		 (*vl)*=lamda;
		 }
	 }

int CMatrix::findMaxLineinColumn(int fromLine,int inColumn,double& maine) {
	int maxLine;
	double maxElement=get(fromLine,inColumn),t;
	maxLine=fromLine;
	if(fromLine==nLine) return maxLine;
	for(int i=fromLine+1;i<nLine;i++) {
	   t=get(i,inColumn);
	   if(fabs(t)>fabs(maxElement)) {
		   maxElement=t;
		   maxLine=i;
		   }
	   }
	maine=maxElement;
	return maxLine;
	}

void CMatrix::centrol(CVector& aBar,CVector& sg) {
	centrol(&aBar,&sg);
	}

void CMatrix::centrol(CVector* aBar,CVector* sg) {
	aBar->init(nLine);
	sg->init(nLine);
	for(int i=0;i<nLine;i++) {
	   CVector* cv=getLine(i);
	   double ev=cv->getCentrol();
	   aBar->put(ev,i);
	   (*cv)-=ev;
	   double s=cv->sigma();
	   if(s==0) s=1.0;
	   sg->put(s,i);
	   (*cv)*=(1.0l/s);
	   }
	}

void CMatrix::deCentrol(CVector& aBar,CVector& sg) {
	for(int i=0;i<nLine;i++) {
	   CVector* cv=getLine(i);
	   (*cv)*=sg.get(i);
	   (*cv)+=(aBar.get(i));
	   }
	}
void CMatrix::deCentrol(CVector* aBar,CVector* sg) {
	deCentrol(*aBar,*sg);
	}


void CMatrix::Transfer(int targetLine,int sourceLine,double factor) {
	int j;double ds,dd;
	for(j=0;j<nColumn;j++) {
	   ds=get(sourceLine,j);
	   dd=get(targetLine,j);
	   dd-=ds*factor;
	   put(dd,targetLine,j);
	   }
	}


CMatrix* CMatrix::inverse() {
   int i,ii,ml=getLines(),mc=getColumns();
   int maxLine;double Lij,mainElement;
   if(ml!=mc) return NULL;
   CMatrix *mt=new CMatrix(ml,1.00);
   
   for(i=0;i<ml;i++) {
	  maxLine=findMaxLineinColumn(i,i,mainElement);
	  swapLine(maxLine,i);
	  mt->swapLine(maxLine,i);
	  for(ii=0;ii<ml;ii++) {
		 if(ii!=i) {
			 Lij=get(ii,i)/mainElement;
			 Transfer(ii,i,Lij);
			 mt->Transfer(ii,i,Lij);
			 }
		 }
	  }

   for(i=0;i<ml;i++) {
	  Lij=1.0/get(i,i);
	  primX(Lij,i);
	  mt->primX(Lij,i);
	  }
   return mt;
   }

CMatrix& CMatrix::operator+= (CMatrix& m) {
   int i,j;
   for(i=0;i<nLine;i++)
	  for(j=0;j<nColumn;j++)
		  put(get(i,j)+m.get(i,j),i,j);
   return *this;
   }

CMatrix& CMatrix::operator+= (CMatrix* m) {
   int i,j;
   for(i=0;i<nLine;i++)
	  for(j=0;j<nColumn;j++)
		 put(get(i,j)+m->get(i,j),i,j);
   return *this;
   }

double CMatrix::get(int line,int column) {
   return (element+line)->get(column);
   }

void CMatrix::put(double ev,int line,int column) {
   if(line<nLine&&column<nColumn) (element+line)->put(ev,column);
   }

void CMatrix::addput(double ev,int line,int column) {
	if(line<nLine&&column<nColumn) {
	   double vt=get(line,column);
	   (element+line)->put(ev+vt,column);
	   }
    }

void CMatrix::Serialize(CArchive& ar) {
	if(ar.IsLoading()) {
		if(element!=NULL) delete element;
		int n,m;ar>>n;ar>>m;init(n,m);
		}
	else {
		ar<<nLine;ar<<nColumn;
		}
	for(int i=0;i<nLine;i++) (element+i)->Serialize(ar);
	}

CMatrix* operator+ (CMatrix& a,CMatrix& b) {
   CMatrix *mt=new CMatrix(a);
   *mt+=b;
   return mt;
   }

CMatrix* operator+ (CMatrix* a,CMatrix& b) {
   return *a+b;
   }

CMatrix* operator+ (CMatrix& a,CMatrix* b) {
   return a+*b;
   }

CMatrix& CMatrix:: operator*= (double lamda) {
   int i,j;
   for(i=0;i<nLine;i++)
	  for(j=0;j<nColumn;j++)
		   put(get(i,j)*lamda,i,j);
   return *this;
   }

CMatrix& CMatrix:: operator+= (double lamda) {
   int i,j;
   for(i=0;i<nLine;i++)
	  for(j=0;j<nColumn;j++)
		   put(get(i,j)+lamda,i,j);
   return *this;
   }

CMatrix& CMatrix:: operator= (CMatrix& ms) {
   int i;
   for(i=0;i<nLine;i++)
	  *getLine(i)=ms.getLine(i);
   return *this;
   }

CMatrix& CMatrix:: operator= (CMatrix* ms) {
   int i;
   for(i=0;i<nLine;i++)
	  (*getLine(i))=ms->getLine(i);
   return *this;
   }

CMatrix* operator* (CMatrix& a,CMatrix& b) {
   int i,j,ml,mc;CVector *l,*m;double v;
   CMatrix* bt=b.transmit();
   if(a.getColumns()!=b.getLines()) return NULL;
   CMatrix *mt=new CMatrix(ml=a.getLines(),mc=b.getColumns());
   for(i=0;i<ml;i++)
	  for(j=0;j<mc;j++) {
		  l=a.getLine(i);
		  m=bt->getLine(j);
		  v=dot(l,m);
		  mt->put(v,i,j);
		  }
   delete bt;
   return mt;
   }

CMatrix* operator* (CMatrix& a,CMatrix* b) {
   return a*(*b);
   }

/*CMatrix* operator* (CMatrix* a,CMatrix& b) {
   return (*a)*b;
   }
*/
void CMatrix::reset() {
	for(int i=0;i<nLine;i++) (element+i)->reset();
	}

void CMatrix::fix(char *fmt) {
	for(int i=0;i<nLine;i++) (element+i)->fix(fmt);
	}

CVector* CMatrix::operator* (CVector& s) {
	CVector *vt=NULL;
	int i=nColumn;
	if(nColumn==s.getSize()) {
		vt=new CVector(nLine);
		for(int i=0;i<nLine;i++) {
			vt->put(dot(getLine(i),&s),i);
		   }
		}
	return vt;
	}

CVector* CMatrix::operator* (CVector *s) {
	CVector *vt=NULL;
	if(nColumn==s->getSize()) {
		vt=new CVector(nLine);
		for(int i=0;i<nLine;i++) {
		   vt->put(dot(getLine(i),s),i);
		   }
		}
	return vt;
	}

void CMatrix::Random(double range) {
	CVector* vector;
	for(int i=0;i<nLine;i++) {
		vector=getLine(i);
		vector->Random(range);
		}
	}



CVector* reduce(CVector& v,CMatrix& m) {
	int i,ml,mc,vc;
	if((ml=m.getLines())!=(vc=v.getSize())) return NULL;
	else {
		mc=m.getColumns();
		CVector *vector=new CVector(mc);
		for(i=0;i<mc;i++) {
			CVector *vColumn=m.getColumn(i);
			vector->put(dot(v,vColumn),i);
			delete vColumn;
			}
		return vector;
		}
	}

CVector* reduce(CMatrix& m,CVector& v) {
	int i,ml,mc,vc;
	if((mc=m.getColumns())!=(vc=v.getSize())) return NULL;
	else {
		CVector *vLine;
		ml=m.getLines();
		CVector *vector=new CVector(ml);
		for(i=0;i<ml;i++) {
			vLine=m.getColumn(i);
			vector->put(dot(v,vLine),i);
			delete vLine;
			}
		return vector;
		}
	}

/*CVector* reduce(CMatrix& v,CVector* m) {
	return reduce(v,*m);
	}
*/
/*CVector* reduce(CMatrix* v,CVector& m) {
	return reduce(*v,m);
	}
*/
/*CVector* reduce(CMatrix* v,CVector* m) {
	return reduce(*v,*m);
	}
*/
/*CVector* reduce(CVector& v,CMatrix* m) {
	return reduce(v,*m);
	}
*/
CVector* reduce(CVector* v,CMatrix& m) {
	return reduce(*v,m);
	}

/*CVector* reduce(CVector* v,CMatrix* m) {
	return reduce(*v,*m);
	}
*/
/*

CMatrix *cross(CVector& u,CVector& v) {
	int nu,nv,i,j;
	CMatrix *matrix=new CMatrix(nu=u.getSize(),nv=v.getSize());
	for(i=0;i<nu;i++) for(j=0;j<nv;j++) matrix->put(u.get(i)*v.get(j),i,j);
	return matrix;
	}

CMatrix *cross(CVector* u,CVector& v) {
	return cross(*u,v);
	}

CMatrix *cross(CVector& u,CVector* v) {
	return cross(u,*v);
	}

CMatrix *cross(CVector* u,CVector* v) {
	return cross(*u,*v);
	}
*/

CMatrix* CMatrix::Normalize(double low, double high) {
	int i,j;CMatrix *m=new CMatrix(this);
	for(i=0;i<nLine;i++) for(j=0;j<nColumn;j++) m->put((get(i,j)-low)/(high-low),i,j);
	return m;
	}

void CMatrix::DeNormalize(CMatrix& m,double low, double high) {
	int i,j;
	for(i=0;i<nLine;i++) for(j=0;j<nColumn;j++) m.put((get(i,j)*(high-low)+low),i,j);
	}

void CMatrix::AddToLines(CVector *v) {
	if(nColumn!=v->getSize()) {
		AfxMessageBox("Add Error!");
		return;
		}
	else {
		CVector *vTemp;
		for(int i=0;i<nLine;i++) {
			vTemp=getLine(i);
			(*vTemp)+=v;
			}
		}
	}

void CMatrix::Zeros() {
	CVector* vector;
	for(int i=0;i<nLine;i++) {
		vector=getLine(i);
		vector->Zeros();
		}
	}
