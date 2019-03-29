#include "stdafx.h"
#include "Profile.h"

CProfile::CProfile(VOID)
{
	initialize();
}

CProfile::~CProfile(VOID)
{
}

VOID CProfile::Begin(LPTSTR name)
{
	INT i = 0;

	while (i < MAX_PROFILE_SAMPLES && mSamples[i].Valid == TRUE)
	{
		if (_tcscmp(mSamples[i].Name, name) == 0)
		{
			mSamples[i].OpenProfiles++;
			mSamples[i].ProfileInstance++;
			
			mSamples[i].StartTime = getExactTime();
			
			return;
		}

		i++;
	}
	if (i >= MAX_PROFILE_SAMPLES)
	{
		return;
	}

	_tcscpy(mSamples[i].Name, name);

	mSamples[i].Valid				= TRUE;
	mSamples[i].OpenProfiles		= 1;
	mSamples[i].ProfileInstance		= 1;
	mSamples[i].Accumulator			= 0.0;
	mSamples[i].StartTime			= getExactTime();
	mSamples[i].ChildrenSampleTime	= 0.0;
}

VOID CProfile::End(LPTSTR name)
{
	INT i = 0, NumOfParents = 0;

	while (i < MAX_PROFILE_SAMPLES && mSamples[i].Valid == TRUE)
	{
		if (_tcscmp(mSamples[i].Name, name) == 0)
		{
			INT Inner	= 0;
			INT Parent	= -1;

			double EndTime = getExactTime();

			mSamples[i].OpenProfiles--;

			while (mSamples[Inner].Valid == TRUE)
			{
				if (mSamples[Inner].OpenProfiles > 0)
				{
					NumOfParents++;

					if (Parent < 0)
						Parent = Inner;
					else if (mSamples[Inner].StartTime >= mSamples[Parent].StartTime)
						Parent = Inner;
				}

				Inner++;
			}

			mSamples[i].NumberOfParents = NumOfParents;

			if (Parent >= 0)
				mSamples[Parent].ChildrenSampleTime += (EndTime - mSamples[i].StartTime);

			mSamples[i].Accumulator += (EndTime - mSamples[i].StartTime);

			return;
		}
		i++;
	}
}

VOID CProfile::DumpProfileOutput(VOID)
{
	INT i = 0;

	mEndProfile = getExactTime();

	printf("   Ave  :   Min   :   Max   :       # : Profile Name\n");
	printf("---------------------------------------------------------------------------\n");

	while (i < MAX_PROFILE_SAMPLES && mSamples[i].Valid == TRUE)
	{
		UINT	Indent				= 0;
		
		double	SampleTime			= 0.0F;
		double  PercentTime			= 0.0F;
		double  AveageTime			= 0.0F;
		double  MinimumTime			= 0.0F;
		double	MaximumTime			= 0.0F;

		TCHAR	Line[256]			= {0,};
		TCHAR   Name[256]			= {0,};
		TCHAR	IndentedName[256]	= {0,};
		
		TCHAR	Aveage[16]			= {0,};
		TCHAR	Minimum[16]			= {0,};
		TCHAR   Maximum[16]			= {0,};
		TCHAR   Number[16]			= {0,};

		SampleTime	= mSamples[i].Accumulator - mSamples[i].ChildrenSampleTime;
		PercentTime = (SampleTime / (mEndProfile - mBeginProfile)) * 100.0;

		AveageTime = MinimumTime = MaximumTime = PercentTime;

		storeProfileInHistory(mSamples[i].Name, PercentTime);
		getProfileFromHistory(mSamples[i].Name, AveageTime, MinimumTime, MaximumTime);

		_sntprintf(Aveage,	16, _T("%4.2f"),	AveageTime);
		_sntprintf(Minimum,	16, _T("%4.2f"),	MinimumTime);
		_sntprintf(Maximum,	16, _T("%4.2f"),	MaximumTime);
		_sntprintf(Number,	16, _T("%7d"),		mSamples[i].ProfileInstance);

		_tcsncpy(IndentedName, mSamples[i].Name, 256);

		for (Indent=0;Indent<mSamples[i].NumberOfParents;Indent++)
		{
			_sntprintf(Name, 256, _T("  %s"), IndentedName);
			_tcsncpy(IndentedName, Name, 256);
		}

		_sntprintf(Line, 256, _T("%7s : %7s : %7s : %7s : %s\n"), Aveage, Minimum, Maximum, Number, IndentedName);
		_tprintf(Line);
		i++;
	}

	for(i=0;i<MAX_PROFILE_SAMPLES;i++)
		mSamples[i].Valid = FALSE;

	mBeginProfile = getExactTime();
}

VOID CProfile::initialize(VOID)
{
	for (INT i=0;i<MAX_PROFILE_SAMPLES;i++)
	{
		mSamples[i].Valid	= FALSE;
		mHistories[i].Valid	= FALSE;
	}

	LARGE_INTEGER CurrentFrequency;

	QueryPerformanceFrequency(&CurrentFrequency);

	mFrequency		= (double) CurrentFrequency.QuadPart;
	mBeginProfile	= getExactTime();
}

double CProfile::getExactTime(VOID)
{
	LARGE_INTEGER CurrentTime;

	QueryPerformanceCounter(&CurrentTime);

	return ((double) CurrentTime.QuadPart) / mFrequency;
}

VOID CProfile::storeProfileInHistory(LPTSTR name, double percent)
{
	INT			i			= 0;
	double		OldRatio	= 0.0F;
	double		NewRatio	= 0.8F * 0.001F;

	if (NewRatio > 1.0F)
		NewRatio = 1.0F;

	OldRatio = 1.0F - NewRatio;

	while (i < MAX_PROFILE_SAMPLES && mHistories[i].Valid == TRUE)
	{
		if (_tcscmp(mHistories[i].Name, name) == 0)
		{
			mHistories[i].Average = (mHistories[i].Average * OldRatio) + (percent * NewRatio);

			if (percent < mHistories[i].Minimum)
				mHistories[i].Minimum	= percent;
			else
				mHistories[i].Minimum	= (mHistories[i].Minimum * OldRatio) + (percent * NewRatio);

			if (mHistories[i].Minimum < 0.0F)
				mHistories[i].Minimum	= 0.0F;

			if (percent > mHistories[i].Maximum)
				mHistories[i].Maximum	= percent;
			else
				mHistories[i].Maximum	= (mHistories[i].Maximum * OldRatio) + (percent * NewRatio);

			return;
		}

		i++;
	}

	if (i < MAX_PROFILE_SAMPLES)
	{
		_tcsncpy(mHistories[i].Name, name, MAX_PATH);
		
		mHistories[i].Valid		= TRUE;
		mHistories[i].Average	= mHistories[i].Minimum = mHistories[i].Maximum = percent;
	}
}

VOID CProfile::getProfileFromHistory(LPTSTR name, double &average, double &minimum, double &maximum)
{
	INT i = 0;

	while (i < MAX_PROFILE_SAMPLES && mHistories[i].Valid == TRUE)
	{
		if (_tcscmp(mHistories[i].Name, name) == 0)
		{
			average = mHistories[i].Average;
			minimum	= mHistories[i].Minimum;
			maximum = mHistories[i].Maximum;

			return;
		}

		i++;
	}

	average = minimum = maximum = 0.0F;
}