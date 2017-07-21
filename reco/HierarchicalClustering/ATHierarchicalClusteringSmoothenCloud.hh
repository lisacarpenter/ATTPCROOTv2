#ifndef ATHIERARCHICALCLUSTERINGSMOOTHENCLOUD_HH
#define ATHIERARCHICALCLUSTERINGSMOOTHENCLOUD_HH

#include <pcl/io/io.h>

namespace ATHierarchicalClusteringSmoothenCloud
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr smoothenCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, int k, bool useMedian = false);
    pcl::PointCloud<pcl::PointXYZI>::Ptr smoothenCloud(pcl::PointCloud<pcl::PointXYZI>::Ptr cloud, double radius, bool useMedian = false);
}

#endif
