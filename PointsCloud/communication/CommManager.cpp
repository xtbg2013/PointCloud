#include "CommManager.h"
CommManager::CommManager(ICom *pMCComHighSpeed,ICom *pMCComLowSpeed,ICom *pLaserCom,ICom *pSPCom)
{
    m_pMCComHighSpeed = pMCComHighSpeed;
    m_pMCComLowSpeed = pMCComLowSpeed;   
    m_pLaserCom = pLaserCom;
    m_pSPCom = pSPCom;   
}

CommManager::~CommManager()
{
    /*
	FILE *fp = nullptr;
	fp = fopen("./Points.dat","w");
	if(fp != nullptr)
	{
		ZPoints points;
		points.x = 12.80;
		points.y = 17.0;
		points.z = 19.0;
		points.intensity = 0.30;
		char strData[100] = {0};
		sprintf(strData,"%.2f %.2f %.2f %.2f",points.x,points.y,points.z,points.intensity);
		//string str = to_string(points.x);
		//string str(strData);

		fwrite(strData,strlen(strData),1,fp);
		fclose(fp);
	}
    */
}
