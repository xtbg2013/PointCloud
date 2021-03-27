
#include "PointsProcess.h"
#include <string>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
using namespace std;

PointsProcess::PointsProcess(/* args */)
{
}

PointsProcess::~PointsProcess()
{
}

int PointsProcess::SaveToPcd(Point_XYZI *pData,int size)
{
    static long frameId = 0;
    string strFrameId = to_string(frameId++);
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>);//创建自定义点云格式PointXYZI
    cloud->header.frame_id = strFrameId;
    cloud->width = size;
    cloud->height = 1;
    cloud->points.resize(cloud->width * cloud->height);
    for (size_t i = 0; i < cloud->points.size (); ++i) {
        cloud->points[i].x = pData[i].x;
        cloud->points[i].y = pData[i].y;
        cloud->points[i].z = pData[i].z;
        cloud->points[i].intensity = pData[i].intensity;
    }
    string name =  "./pcd/frame"+strFrameId+".pcd"; //
    pcl::io::savePCDFile (name, *cloud);
    return 0;
}