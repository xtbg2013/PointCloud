

// PointsCloud.cpp : Defines the entry point for the application.
//
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "PointsCloud.h"
#include "glog/logging.h"
#include "DataDefine.h"
#include "TaskManager.h"
#include <string.h>

using namespace std;

TaskManager g_TaskManager;


 
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


bool InitTask()
{
	system("mkdir -p ./pcd");
	if(!g_TaskManager.InitilizeCom())
	{
		LOG(ERROR) << "Initialize point cloud task communication failure";
		return false;
	}
	
	if(!g_TaskManager.Launch())
	{
		LOG(ERROR) << "Initialize point cloud task failure";
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

struct ZPoints
{
    float x;
    float y;
    float z;
    float intensity;
};

 

 
int main()
{
	
	LOG(INFO) << "Enter APP";
	InitGoogleLog();
	//CreateDeamon();
	InitTask();
	AppControl();
	StopTask();
	LOG(INFO) << "Exit APP";
	
	return 0;
}
