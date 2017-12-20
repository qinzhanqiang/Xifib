#include "stdafx.h"
#include "..\include\xiApi.h"

#define MAX_NAME_LEN 300

int saveImageBMP(char* filename, XI_IMG* image)
{
	unsigned char* BitmapData=(unsigned char*)image->bp;
	DWORD dwWidth = image->width;
	DWORD dwHeight=image->height;
	UINT line_width_bytes= 0;
	int bits_per_pixel = 8;
	int bytes_per_pixel = 0;
	bool input_mono8=false;

	switch(image->frm)
	{
	case XI_MONO8 :
	case XI_RAW8  :
		input_mono8=true;
		line_width_bytes = image->width + image->padding_x;
		bytes_per_pixel = 1;
		break;
	case XI_RGB32 :
		bits_per_pixel = 24;
		line_width_bytes = (image->width + image->padding_x/4)*4;
		bytes_per_pixel = 4;
		break;
	default:
		// format not supported
		return FALSE;
	}
	
	#define Fs sizeof(BITMAPFILEHEADER)
    #define Is sizeof(BITMAPINFOHEADER)
    DWORD bmP[256];
    struct  LOGPAL{ WORD palVersion; WORD palNumEntries; DWORD      palRGBF[256];} LogPal={0x300,256};
    BITMAPFILEHEADER bmF = {0x4D42, (dwWidth&(~3))*dwHeight+Fs+Is+1024, 0, 0, Fs+Is+1024 }; // +1024 in mono8
	BITMAPINFOHEADER bmI = {40, (dwWidth&(~3)), dwHeight, 1, bits_per_pixel, BI_RGB, 0, 0x0ec4, 0x0ec4, 0, 0};
    
	// generate grayscale pallete
    for(int j=0; j<256; j++ ) LogPal.palRGBF[j] = RGB(j, j, j);
    for(int j=0; j<256; j++ ) 
            bmP[j] = (LogPal.palRGBF[j]&0xFF)*0x10000 +
            (LogPal.palRGBF[j]&0xFF00) +
            (LogPal.palRGBF[j]&0xFF0000) / 0x10000; 
	
	// path
	//char	modePath[MAX_PATH];
	//GetModuleFileName(NULL, modePath, MAX_NAME_LEN);
	//*strrchr(modePath,'\\') = 0;
	
	FILE * fp;
	fp = fopen(filename, "wb");
	if(fp != NULL)
	{
		fwrite(&bmF, Fs, 1, fp);
        fwrite(&bmI, Is, 1, fp);
		//if (input_mono8)
		fwrite(&bmP, 1024, 1, fp); // save pallete
        unsigned char* p = BitmapData;
        p += (dwHeight - 1) * line_width_bytes;
		// bitmap data stored upside down
        for(DWORD y=0; y < dwHeight; y++, p-= line_width_bytes)
        {
            for(DWORD x = 0; x < ((dwWidth&(~3))*bytes_per_pixel); x++) 
			{			
				fwrite(&p[x], 1, bits_per_pixel/8, fp);
				if(image->frm == XI_RGB32)
				{
					x += bits_per_pixel/8;
				}			
			}
        }
		fclose(fp);
		fp = NULL;
	}
	else
	{
		char msg[100]="";
		sprintf(msg,"Error opening x%x\n",GetLastError());
		return FALSE;
	}
	return TRUE;
}
