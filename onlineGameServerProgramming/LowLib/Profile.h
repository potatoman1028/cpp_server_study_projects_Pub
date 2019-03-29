#pragma once

#define MAX_PROFILE_SAMPLES		50

typedef struct _PROFILE_SAMPLE
{
	BOOL		Valid;
	UINT		ProfileInstance;
	INT32		OpenProfiles;
	TCHAR		Name[MAX_PATH];
	double		StartTime;
	double		Accumulator;
	double		ChildrenSampleTime;
	UINT		NumberOfParents;
} PROFILE_SAMPLE;

typedef struct _PROFILE_SAMPLE_HISTORY
{
	BOOL		Valid;
	TCHAR		Name[MAX_PATH];
	double		Average;
	double		Minimum;
	double		Maximum;
} PROFILE_SAMPLE_HISTORY;

class CProfile
{
public:
	CProfile(VOID);
	virtual ~CProfile(VOID);

public:
	VOID					Begin(LPTSTR name);
	VOID					End(LPTSTR name);

	VOID					DumpProfileOutput(VOID);

private:
	double					getExactTime(VOID);

	VOID					initialize(VOID);
	VOID					storeProfileInHistory(LPTSTR name, double percent);
	VOID					getProfileFromHistory(LPTSTR name, double &average, double &minimum, double &maximum);

private:
	PROFILE_SAMPLE			mSamples[MAX_PROFILE_SAMPLES];
	PROFILE_SAMPLE_HISTORY	mHistories[MAX_PROFILE_SAMPLES];

	double					mBeginProfile;
	double					mEndProfile;
	double					mFrequency;
};
