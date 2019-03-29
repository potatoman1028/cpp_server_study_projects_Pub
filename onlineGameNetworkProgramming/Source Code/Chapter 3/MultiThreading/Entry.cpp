#include <Windows.h>
#include <stdio.h>

DWORD __stdcall ThreadRunner( LPVOID parameter )
{
	int* argument;
	int count;

	//  ������ 3�� �����Ѵ�.
	count = 3;

	//  ���ڸ� �޾ƿ´�.
	argument = ( int* )parameter;

	while( count-- > 0 )
		printf( "I'm %d Thread !!\n", *argument );

	//  �� �Լ��� �Բ� ������� ����ȴ�.
	return 0;
}

void main()
{
	HANDLE handleThread[5];
	int array[5] = { 0, };
	int i;

	for( i = 0 ; i < 5 ; i++ )
	{
		array[i] = i;
		handleThread[i] = ::CreateThread( 0, 0, ThreadRunner, &array[i], 0, 0 );
	}

	for( i = 0 ; i < 5 ; i++ )
		printf( "I'm Main thread !!\n" );

	::WaitForMultipleObjects( 5, handleThread, TRUE, INFINITE );
}