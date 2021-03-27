

// PointsCloud.cpp : Defines the entry point for the application.
//
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "PointsCloud.h"
//#include "glog/logging.h"
#include "DataDefine.h"
#include "TaskManager.h"
#include <string.h>

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "IpcCom.h"
#include <pcl/point_types.h>
#include "transmsgstruct.h"
using namespace std;

TaskManager g_TaskManager;


/*
 
void InitGoogleLog()
{
	FLAGS_logtostderr = false; //TRUE:标准输出,FALSE:文件输出
	FLAGS_alsologtostderr = true; //除了日志文件之外是否需要标准输出
	FLAGS_colorlogtostderr = true; //标准输出带颜色
	FLAGS_logbufsecs = 0; //设置可以缓冲日志的最大秒数，0指实时输出
	FLAGS_max_log_size = 10; //日志文件大小(单位：MB)
	FLAGS_stop_logging_if_full_disk = true; //磁盘满时是否记录到磁盘
    google::InitGoogleLogging("PointsCloud");
    google::SetLogDestination(google::GLOG_INFO,"log/");
}

void ShutDownGoogleLog()
{
	 google::ShutdownGoogleLogging();
}

*/
bool InitTask()
{
	//system("mkdir -p ./pcd");
	if(!g_TaskManager.InitilizeCom())
	{
		//LOG(ERROR) << "Initialize point cloud task communication failure";
		return false;
	}
	
	if(!g_TaskManager.Launch())
	{
		//LOG(ERROR) << "Initialize point cloud task failure";
		return false;
	}
	
	return true;
}
 

void StopTask()
{
	g_TaskManager.Terminate(true);
}

/*

//创建守护进程
void CreateDeamon()
{
	umask(0);
	pid_t pid = fork();
	if(pid == -1){
		LOG(ERROR) << "Fork points cloud error";
	}
	else if(pid == 0) { //child
		 
		LOG(INFO) << "Fork points cloud success";
		 
		if(setsid() == -1){
			LOG(ERROR) << "setsid error";
		}
		if(chdir("/") <0){
			LOG(ERROR) << "change work directory error";
		}
		for(int i = 0;i<3;i++){
			close(i);

		}
	}
	else
	{
		exit(1);
	}
	
}
*/
void AppControl()
{
	int select = 0;
	cout<<"Press any key to exit"<<endl;
	cin>>select;
}

 void Test()
 {
	int32 flag = 0x12345678;
	int16 lengh = 48;
	int8 data[54] = {0};
	int8 offset = 0;
	
	memcpy(data+offset,&flag,4);
	offset += 4;

	memcpy(data+offset,&lengh,2);
	offset += 2;

    MFD_Control ctl;
	ctl.validMfd.keyValid = 1;
	ctl.mfd3.page = 4;
	ctl.mfd3.subPage = 1;
	 

	memcpy(data+offset,&ctl,sizeof(MFD_Control));
	offset += sizeof(MFD_Control);



   for(int i = 0;i<10;i++)
   {
	   g_TaskManager.ProcessRecvMessage(data,sizeof(data));
   }

 }

int main()
{


 #if 0
  //	LOG(INFO) << "Enter APP";
	//InitGoogleLog();
	//CreateDeamon();
	InitTask();
	AppControl();
	StopTask();
	//LOG(INFO) << "Exit APP";
#endif
	
/*
    IpcFIFOCom::Instance()->Initialize(O_WRONLY);
	//for(int i = 0;i<120;i++)
	int count = 0;

	while(true)
	{
		IpcMsg m;
		m.msgType =  ++count;
		m.systemControl = 1;
		m.workControl = 2;
		m.laserControl = 3;
		m.renderingControl = 4;
		
		
		IpcFIFOCom::Instance()->Write(m);
		cout<<count<<endl;

		sleep(1);
	}
*/
#if 1
 
 
	//for(int i = 0;i<120;i++)
	int count = 0;

	while(true)
	{
		IpcMsg m;
		m.msgType =  ++count;
		m.systemControl = 1;
		m.workControl = 2;
		m.laserControl = 3;
		m.renderingControl = 4;
		
		if(IpcCom::Instance()->Write(m)>0)
			cout<<count<<endl;

		sleep(1);
	}
	#endif
	//IpcCom::Instance()->Release();;

	
/*
    //共享内存写
	cout<<"this is a test"<<endl;
    


    Point_XYZI buffer[10] = {0};

	int value = 0;
	while(true)
	{
		 
	    buffer[0].x = buffer[0].y = buffer[0].z = value++;
			 
		int length = IpcShareMemory::Instance()->Write(buffer,10*sizeof(Point_XYZI));
		if(length > 0)
		{
			cout<<"count = "<<value<<endl;
			cout<<"Points: x= "<<buffer[0].x<<" y="<<buffer[0].y<<" z="<<buffer[0].z<<endl;
			
		}
		usleep(1000000);
	}
*/
	

   //共享内存读取
   
   /*
    
	vector<PointXYZI> points;
	int count = 0;
	while(true)
	{
		
		int length = IpcShareMemory::Instance()->Read(points);
		if(length > 0)
		{
			cout<<"recv count = "<<++count<<endl;
			cout<<"point count = "<<points.size() <<endl;
		}
		usleep(100000);
	}
	*/
  
	return 0;
}
