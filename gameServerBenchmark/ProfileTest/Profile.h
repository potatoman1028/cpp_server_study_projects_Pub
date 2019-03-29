#pragma once

#define MAX_PROFILE_SAMPLES		50

typedef struct _PROFILE_SAMPLE
{
	BOOL bValid;
	UINT uiProfileInstance;
	INT32 iOpenProfiles;
	WCHAR wszName[MAX_PATH];
	double dStartTime;
	double dAccumulator;
	double dChildrenSampleTime;
	UINT uiNumberOfParents;
} PROFILE_SAMPLE;

typedef struct _PROFILE_SAMPLE_HISTORY
{
	BOOL bValid;
	WCHAR wszName[MAX_PATH];
	double dAverage;
	double dMinimum;
	double dMaximum;
} PROFILE_SAMPLE_HISTORY;

class CProfile
{
public:
	CProfile(VOID);
	virtual ~CProfile(VOID);

public:
	VOID Begin(LPWSTR lpwName);
	VOID End(LPWSTR lpdwName);

	VOID DumpProfileOutput(VOID);

private:
	double getExactTime(VOID);

	VOID initialize(VOID);
	VOID storeProfileInHistory(LPWSTR lpwName, double dPercent);
	VOID getProfileFromHistory(LPWSTR lpwName, double &rdAverage, double &rdMinimum, double &rdMaximum);

private:
	PROFILE_SAMPLE m_oSamples[MAX_PROFILE_SAMPLES];
	PROFILE_SAMPLE_HISTORY m_oHistories[MAX_PROFILE_SAMPLES];

	double m_dBeginProfile;
	double m_dEndProfile;
	double m_dFrequency;
};
