# include <stdio.h>
# include "Fuzzy.h"
void FuzzyField::LoadFromFile(FILE *fp) {
	fscanf(fp,"%d%d%f%f%f",&FieldID,&type,&LeftZero,&CenterOne,&RightZero);
	}

void FuzzyField::printout() {
	printf("Id=%d type=%d left=%5.3f iddle=%5.3f right=%5.3f eta=%5.3f\n",FieldID,type,LeftZero,CenterOne,RightZero,eta);
	}
void FuzzyField::Fuzzy(float f) {
	switch(type) {
		case MostLeft:
			if(f<=CenterOne) eta=1.0;
			else if(f>=RightZero) eta=0.0;
			else eta=(RightZero-f)/(RightZero-CenterOne);
			break;
		case Middle:
			if(f<LeftZero||f>RightZero) eta=0.0;
			else if(f<=CenterOne) eta=(f-LeftZero)/(CenterOne-LeftZero);
			else eta=(RightZero-f)/(RightZero-CenterOne);
			break;
		case MostRight:
			if(f>=CenterOne) eta=1.0;
			else if(f<=LeftZero) eta=0.0;
			else eta=(f-LeftZero)/(CenterOne-LeftZero);
			break;
		}
	}

float FuzzyField::GetEtaofX(float x) {
	if(eta>0&&x>LeftZero&&x<RightZero) {
		float etaSave=eta,retValue;Fuzzy(x);
		if(eta<etaSave) retValue=eta;
		else retValue=etaSave;
		eta=etaSave;
		return retValue;
		}
	else return 0.0;
	}

void FuzzyFields::LoadFromFile(FILE *fp) {
	if(pFields!=NULL) delete[] pFields;
	fscanf(fp,"%d",&nFields);
	pFields=new FuzzyField[nFields];
	for(int i=0;i<nFields;i++) pFields[i].LoadFromFile(fp);
	Left=pFields[0].LeftZero;
	Right=pFields[nFields-1].RightZero;
	}

void FuzzyFields::Fuzzy(float f) {
	for(int i=0;i<nFields;i++) pFields[i].Fuzzy(f);
	}

void FuzzyFields::Zeros() {
	for(int i=0;i<nFields;i++) pFields[i].eta=0.0;
	}

void FuzzyFields::printout() {
	for(int i=0;i<nFields;i++) pFields[i].printout();
	}

float FuzzyFields::GetEtaOfID(int id) {
	for(int i=0;i<nFields;i++) {
		if(pFields[i].FieldID==id) return pFields[i].eta;
		}
	return 0;
	}

float FuzzyFields::GetMaxEta(float x) {
	float max=0,thisEta;
	for(int i=0;i<nFields;i++) {
		thisEta=pFields[i].GetEtaofX(x);
		if(thisEta>max) max=thisEta;
		}
	return max;
	}

float FuzzyFields::DeFuzzy() {
	float IntgralStep=(Right-Left)*0.01,x,sumArea=0.0,sumL=0,maxEta;
	for(x=Left;x<Right;x+=IntgralStep) {
		maxEta=GetMaxEta(x);
		sumArea+=maxEta*IntgralStep;
		sumL+=x*maxEta*IntgralStep;
		}
	return sumL/sumArea;
	}

void FuzzyFields::SetEtaOfID(int id,float eta){
	for(int i=0;i<nFields;i++) {
		if(pFields[i].FieldID==id) {
			if(eta>pFields[i].eta) pFields[i].eta=eta;
			}
		}
	}

void FuzzyRules::LoadFromFile(FILE *fp) {
	fscanf(fp,"%d",&nRules);
	pFuzzyRules=new FuzzyRule[nRules];
	for(int i=0;i<nRules;i++) {
		fscanf(fp,"%d%d%d",&pFuzzyRules[i].ErrorID,&pFuzzyRules[i].VelocityID,&pFuzzyRules[i].AdjustID);
		}
	}

void FuzzyLogic::LoadFromFile(char *fname) {
	FILE *fp=fopen(fname,"rt");
	if(fp) {
		ErrorFields.LoadFromFile(fp);
		VelocityFields.LoadFromFile(fp);
		AdjustFields.LoadFromFile(fp);
		Rules.LoadFromFile(fp);
		fclose(fp);
		}
	}

float FuzzyLogic::Adjust(float err,float vel)
{
	ErrorFields.Fuzzy(err);
	VelocityFields.Fuzzy(vel);
	AdjustFields.Zeros();
	int i,rules=Rules.nRules;
	int errID,velID,adjID;
	float etaErr,etaVel,etaAdj;
	for(i=0;i<rules;i++)
	{
		errID=Rules.pFuzzyRules[i].ErrorID;
		etaErr=ErrorFields.GetEtaOfID(errID);

		velID=Rules.pFuzzyRules[i].VelocityID;
		etaVel=VelocityFields.GetEtaOfID(velID);

		adjID=Rules.pFuzzyRules[i].AdjustID;
		etaAdj= (etaErr<etaVel)?etaErr:etaVel;
		AdjustFields.SetEtaOfID(adjID,etaAdj);
	}
	return AdjustFields.DeFuzzy();
}

void FuzzyLogic::printout() {
	printf("Error Fields = \n");
	ErrorFields.printout();
	printf("Velocity Fields = \n");
	VelocityFields.printout();
	printf("Adjust Fields = \n");
	AdjustFields.printout();
	}