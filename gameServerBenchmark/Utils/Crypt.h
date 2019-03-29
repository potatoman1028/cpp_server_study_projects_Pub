#pragma once

class CCrypt
{
public:
	static BOOL Encrypt(BYTE *pucSrc, BYTE *pucDest, DWORD dwLength);
	static BOOL Decrypt(BYTE *pucSrc, BYTE *pucDest, DWORD dwLength);
};