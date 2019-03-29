#include "stdafx.h"
#include "Profile.h"

CProfile::CProfile(VOID)
{
	initialize();
}

CProfile::~CProfile(VOID)
{
}

VOID CProfile::Begin(LPWSTR lpwName)
{
	INT i = 0;

	while (i < MAX_PROFILE_SAMPLES && m_oSamples[i].bValid == TRUE)
	{
		if (wcscmp(m_oSamples[i].wszName, lpwName) == 0)
		{
			m_oSamples[i].iOpenProfiles++;
			m_oSamples[i].uiProfileInstance++;

			m_oSamples[i].dStartTime = getExactTime();

			return;
		}

		i++;
	}
	if (i >= MAX_PROFILE_SAMPLES)
	{
		return;
	}

	wcscpy(m_oSamples[i].wszName, lpwName);

	m_oSamples[i].bValid = TRUE;
	m_oSamples[i].iOpenProfiles = 1;
	m_oSamples[i].uiProfileInstance	= 1;
	m_oSamples[i].dAccumulator = 0.0F;
	m_oSamples[i].dStartTime = getExactTime();
	m_oSamples[i].dChildrenSampleTime = 0.0F;
}

VOID CProfile::End(LPWSTR lpwName)
{
	INT i = 0, NumOfParents = 0;

	while (i < MAX_PROFILE_SAMPLES && m_oSamples[i].bValid == TRUE)
	{
		if (wcscmp(m_oSamples[i].wszName, lpwName) == 0)
		{
			INT Inner	= 0;
			INT Parent	= -1;

			double EndTime = getExactTime();

			m_oSamples[i].iOpenProfiles--;

			while (m_oSamples[Inner].bValid == TRUE)
			{
				if (m_oSamples[Inner].iOpenProfiles > 0)
				{
					NumOfParents++;

					if (Parent < 0)
						Parent = Inner;
					else if (m_oSamples[Inner].dStartTime >= m_oSamples[Parent].dStartTime)
						Parent = Inner;
				}

				Inner++;
			}

			m_oSamples[i].uiNumberOfParents = NumOfParents;

			if (Parent >= 0)
				m_oSamples[Parent].dChildrenSampleTime += (EndTime - m_oSamples[i].dStartTime);

			m_oSamples[i].dAccumulator += (EndTime - m_oSamples[i].dStartTime);

			return;
		}
		i++;
	}
}

VOID CProfile::DumpProfileOutput(VOID)
{
	INT i = 0;

	m_dEndProfile = getExactTime();

	wprintf(L"   Ave  :   Min   :   Max   :       # : Profile Name\n");
	wprintf(L"---------------------------------------------------------------------------\n");

	while (i < MAX_PROFILE_SAMPLES && m_oSamples[i].bValid == TRUE)
	{
		UINT	Indent				= 0;

		double	SampleTime			= 0.0F;
		double  PercentTime			= 0.0F;
		double  AveageTime			= 0.0F;
		double  MinimumTime			= 0.0F;
		double	MaximumTime			= 0.0F;

		WCHAR	Line[256]			= {0,};
		WCHAR   Name[256]			= {0,};
		WCHAR	IndentedName[256]	= {0,};

		WCHAR	Aveage[16]			= {0,};
		WCHAR	Minimum[16]			= {0,};
		WCHAR   Maximum[16]			= {0,};
		WCHAR   Number[16]			= {0,};

		SampleTime	= m_oSamples[i].dAccumulator - m_oSamples[i].dChildrenSampleTime;
		PercentTime = (SampleTime / (m_dEndProfile - m_dBeginProfile)) * 100.0F;
		AveageTime = MinimumTime = MaximumTime = PercentTime;

		storeProfileInHistory(m_oSamples[i].wszName, PercentTime);
		getProfileFromHistory(m_oSamples[i].wszName, AveageTime, MinimumTime, MaximumTime);

		_snwprintf(Aveage,	16, L"%4.2f",	AveageTime);
		_snwprintf(Minimum,	16, L"%4.2f",	MinimumTime);
		_snwprintf(Maximum,	16, L"%4.2f",	MaximumTime);
		_snwprintf(Number,	16, L"%7d",		m_oSamples[i].uiProfileInstance);

		wcsncpy(IndentedName, m_oSamples[i].wszName, 256);

		for (Indent=0;Indent<m_oSamples[i].uiNumberOfParents;Indent++)
		{
			_snwprintf(Name, 256, L"  %s", IndentedName);
			wcsncpy(IndentedName, Name, 256);
		}

		_snwprintf(Line, 256, L"%7s : %7s : %7s : %7s : %s\n", Aveage, Minimum, Maximum, Number, IndentedName);
		wprintf(Line);
		i++;
	}

	for(i=0;i<MAX_PROFILE_SAMPLES;i++)
		m_oSamples[i].bValid = FALSE;

	m_dBeginProfile = getExactTime();
}

VOID CProfile::initialize(VOID)
{
	for (INT i=0;i<MAX_PROFILE_SAMPLES;i++)
	{
		m_oSamples[i].bValid	= FALSE;
		m_oHistories[i].bValid	= FALSE;
	}

	LARGE_INTEGER CurrentFrequency;

	QueryPerformanceFrequency(&CurrentFrequency);

	m_dFrequency = (double) CurrentFrequency.QuadPart;
	m_dBeginProfile	= getExactTime();
}

double CProfile::getExactTime(VOID)
{
	LARGE_INTEGER CurrentTime;

	QueryPerformanceCounter(&CurrentTime);

	return ((double) CurrentTime.QuadPart) / m_dFrequency;
}

VOID CProfile::storeProfileInHistory(LPWSTR lpwName, double dPercent)
{
	INT i = 0;
	double dOldRatio = 0.0F;
	double dNewRatio = 0.8F * 0.001F;

	if (dNewRatio > 1.0F)
		dNewRatio = 1.0F;

	dOldRatio = 1.0F - dNewRatio;

	while (i < MAX_PROFILE_SAMPLES && m_oHistories[i].bValid == TRUE)
	{
		if (wcscmp(m_oHistories[i].wszName, lpwName) == 0)
		{
			m_oHistories[i].dAverage = (m_oHistories[i].dAverage * dOldRatio) + (dPercent * dNewRatio);

			if (dPercent < m_oHistories[i].dMinimum)
				m_oHistories[i].dMinimum	= dPercent;
			else
				m_oHistories[i].dMinimum	= (m_oHistories[i].dMinimum * dOldRatio) + (dPercent * dNewRatio);

			if (m_oHistories[i].dMinimum < 0.0F)
				m_oHistories[i].dMinimum	= 0.0F;

			if (dPercent > m_oHistories[i].dMaximum)
				m_oHistories[i].dMaximum	= dPercent;
			else
				m_oHistories[i].dMaximum	= (m_oHistories[i].dMaximum * dOldRatio) + (dPercent * dNewRatio);

			return;
		}

		i++;
	}

	if (i < MAX_PROFILE_SAMPLES)
	{
		wcsncpy(m_oHistories[i].wszName, lpwName, MAX_PATH);

		m_oHistories[i].bValid		= TRUE;
		m_oHistories[i].dAverage	= m_oHistories[i].dMinimum = m_oHistories[i].dMaximum = dPercent;
	}
}

VOID CProfile::getProfileFromHistory(LPWSTR lpwName, double &rdAverage, double &rdMinimum, double &rdMaximum)
{
	INT i = 0;

	while (i < MAX_PROFILE_SAMPLES && m_oHistories[i].bValid == TRUE)
	{
		if (wcscmp(m_oHistories[i].wszName, lpwName) == 0)
		{
			rdAverage = m_oHistories[i].dAverage;
			rdMinimum	= m_oHistories[i].dMinimum;
			rdMaximum = m_oHistories[i].dMaximum;

			return;
		}

		i++;
	}

	rdAverage = rdMinimum = rdMaximum = 0.0F;
}