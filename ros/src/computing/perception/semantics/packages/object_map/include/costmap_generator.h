/*
 *  Copyright (c) 2018, Nagoya University
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither the name of Autoware nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ********************/
#ifndef COSTMAP_GENERATOR_H
#define COSTMAP_GENERATOR_H

// headers in ROS
#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <grid_map_ros/grid_map_ros.hpp>

// headers in local directory
#include "vector_map/vector_map.h"
#include "autoware_msgs/DetectedObjectArray.h"
#include "points2costmap.h"

// headers in STL
#include<memory>

class CostmapGenerator
{
  public:
    CostmapGenerator();
    ~CostmapGenerator();

    void init();
    void run();

  private:
    bool use_vectormap_;
    bool use_wayarea_;
    bool use_waypoint_;
    bool use_objects_;
    bool has_subscribed_vectormap_;
    bool has_subscribed_waypoint_;

    std::string sensor_frame_;
    double grid_resolution_;
    double grid_length_x_;
    double grid_length_y_;
    double grid_position_x_;
    double grid_position_y_;

    vector_map::VectorMap vmap_;

    grid_map::GridMap gridmap_;

    ros::NodeHandle nh_;
    ros::NodeHandle private_nh_;

    ros::Publisher pub_costmap_;
    ros::Subscriber sub_waypoint_;
    ros::Subscriber sub_points_;

    std::unique_ptr<message_filters::Subscriber<sensor_msgs::PointCloud2>> sub_sync_points_ptr_;
    std::unique_ptr<message_filters::Subscriber<autoware_msgs::DetectedObjectArray>> sub_sync_objects_ptr_;
    std::unique_ptr<message_filters::TimeSynchronizer<sensor_msgs::PointCloud2,
                                                      autoware_msgs::DetectedObjectArray>> sync_ptr_;

    Points2Costmap points2costmap_;

    void waypointCallback(const autoware_msgs::LaneArray& in_waypoint);
    void sensorPointsCallback(const sensor_msgs::PointCloud2& in_sensor_points);
    void mapPointsCallback(const sensor_msgs::PointCloud2& in_map_points);
    void syncedCallback(const sensor_msgs::PointCloud2::ConstPtr& in_points,
                  const autoware_msgs::DetectedObjectArray::ConstPtr& in_objects);
    void registerVectormapSubscriber();
    void registerSyncedSubscriber();
    void initGridmap();
    grid_map::GridMap generateSensorPointsCostmap(const sensor_msgs::PointCloud2& in_sensor_points);

};

#endif  // COSTMAP_GENERATOR_H
