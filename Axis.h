# ifndef __AXIS_H__
# define __AXIS_H__
class CAxis : public CObject{
	BOOL path0;
	double min,max;
public:
	CString axisTitle,scaleFormat;
	double minScale,maxScale;
	double mainStep;
	int mainSteps,subSteps;
public:
	void ForcePathZero(BOOL p0);
	void Initialize() {
		path0=FALSE;
		min=1.0e6,max=0;
		}
	void Update(double v);
	double GetWidth();
	CAxis() {
		path0=FALSE;
 		min=1.0e6,max=0;
		}
	void SetTitle(CString s) {
		axisTitle=s;
		}
	BOOL ScaleAxis(double base=10.0);
	void SetRange(double m,double M,double base=10.0) {
		min=m;max=M;
		ScaleAxis(base);
		}
	};
# endif