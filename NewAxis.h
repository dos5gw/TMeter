#pragma once

class CNewAxis
{
public:
	double minRange; 
	double maxRange; 
	double extendFactor; 
	double mainStep; 
	int nSteps; 
	int nJep; 

	CString axisTitle;
	CString scaleFormat;

	void SetTitle(CString title);
	void SetExtendFactor(double ext){ extendFactor=ext; };
	void SetScale(double m,double M);
	BOOL ScaleAxis();
	CNewAxis(void);
	CNewAxis(double extend);
	~CNewAxis(void);

private:
	double base;
};
