
#include "..\include\xiApi.h"

#define CAM_SELECT       "-select_cam_sn"     // camera with SN will be used
#define BW_SELECT        "-set_bandwidth"     // bandwidth in MegaBytesPerSecond (same unit as xiCOP shows)
#define AUTO_BW_DISABLED "-disable_auto_bw"   // auto bandwidth measurement will not be performed on open

class CommandLineParser : public CCommandLineInfo   //Parser ÆÊÎöÆ÷£¬½âÎöÆ÷
{
	bool isRunWithSN;
	char camSerial[MAX_PATH];
	enum context_e
	{
		WAIT_PARAM_NONE,
		WAIT_PARAM_SELECT_CAM_SN,
		WAIT_PARAM_BANDWIDTH_MBPS,
	};
	context_e parse_context;
	bool auto_bw_enabled;
	bool bandwidth_limit_enabled;
	int  bandwidth_limit_MBPS;

public:
	CommandLineParser()
	{
		isRunWithSN = false;
		memset(camSerial, 0, sizeof(camSerial));
		parse_context = WAIT_PARAM_NONE;
		auto_bw_enabled = true;
		bandwidth_limit_enabled = false;
		bandwidth_limit_MBPS = 0;
	}


 
  //public methods for checking these.
public:

	virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast)
	{
		if(bFlag) // getting parameter types
		{ 
			if(!strcmp(pszParam, CAM_SELECT)) 
				parse_context=WAIT_PARAM_SELECT_CAM_SN;
			else if (!strcmp(pszParam, BW_SELECT))
				parse_context=WAIT_PARAM_BANDWIDTH_MBPS;
			else if (!strcmp(pszParam, AUTO_BW_DISABLED))
				auto_bw_enabled = false;
			else
			{
				printf("Error: unknown command line parameter %s\n",pszParam);
			}
		} 
		else // getting parameter values
		{
			switch (parse_context)
			{
			case WAIT_PARAM_SELECT_CAM_SN:
				isRunWithSN = true;
				strcpy(camSerial, pszParam);
				break;
			case WAIT_PARAM_BANDWIDTH_MBPS:
				bandwidth_limit_MBPS = atoi(pszParam);
				if (bandwidth_limit_MBPS)
					bandwidth_limit_enabled=true;
				break;
			default:
				printf("Error: unknown command line context %d\n",parse_context);
				break;
			}
		}
	}

	void pushParams(XiPlayPane* dlg)
	{
		dlg->isRunWithSN = isRunWithSN;
		strcpy(dlg->runSN, camSerial);
		dlg->auto_bandwidth_measure_enabled = auto_bw_enabled;
		dlg->bandwidth_limit_MBPS = bandwidth_limit_MBPS;
		dlg->bandwidth_limit_enabled = bandwidth_limit_enabled;
	}
};