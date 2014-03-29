# include <stdlib.h>
# include <stdio.h>

enum FieldType {MostLeft,Middle,MostRight};
class FuzzyField {
public:
	int FieldID;
	FieldType type;
	float LeftZero;
	float CenterOne;
	float RightZero;
	float eta;
public:
	FuzzyField() {
		FieldID=0;
		type=Middle;
		LeftZero=0.0;
		CenterOne=0.0;
		RightZero=0.0;
		eta=0.0;
		}
	void Fuzzy(float f);
	float GetEtaofX(float x);
	void LoadFromFile(FILE *fp);
	void printout();
	};

class FuzzyFields {
public:
	int nFields;
	float Left,Right;
	FuzzyField* pFields;
	FuzzyFields() {
		nFields=0;
		pFields=NULL;
		}
	~FuzzyFields() {
		if(pFields!=NULL) delete[] pFields;
		}
	void Fuzzy(float f);
	void LoadFromFile(FILE *fp);
	float GetEtaOfID(int id);
	float DeFuzzy();
	void SetEtaOfID(int id,float eta);
	float GetMaxEta(float x);
	void Zeros();
	void printout();
	};

struct FuzzyRules {
	int nRules;
	struct FuzzyRule {
		int ErrorID,VelocityID,AdjustID;
		}* pFuzzyRules;
	void LoadFromFile(FILE *fp);
	};

class FuzzyLogic {
public:
	FuzzyFields ErrorFields;
	FuzzyFields VelocityFields;
	FuzzyFields AdjustFields;
	FuzzyRules Rules;
	float Adjust(float err,float vel);
	void LoadFromFile(char *fname);
    void printout();
	};

