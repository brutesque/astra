#ifndef HANDTRACKER_H
#define HANDTRACKER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <SenseKitUL.h>

#include "depthutility.h"
#include "trackedpoint.h"
#include "pointprocessor.h"
#include "../../SenseKitUL/SenseKitUL_internal.h"
#include "coordinateconverter.h"
#include <SenseKit/Plugins/plugin_api.h>
#include <SenseKit/Plugins/PluginStream.h>

namespace sensekit
{
    namespace plugins
    {
        namespace hands
        {
            class HandTracker : public FrameReadyListener
            {
            public:
                HandTracker(PluginServiceProxy& pluginService,
                            Sensor& streamset,
                            StreamDescription& depthDescription);
                virtual ~HandTracker();
                virtual void on_frame_ready(StreamReader& reader, Frame& frame) override;
            private:
                void create_hand_streams(PluginServiceProxy& pluginService, Sensor streamset);
                void subscribe_to_depth_stream(Sensor& streamset, StreamDescription& depthDescription);
                void reset();
                static void copy_position(cv::Point3f& source, sensekit_vector3f_t& target);
                static sensekit_handstatus_t convert_hand_status(TrackingStatus status);
                static void reset_hand_point(sensekit_handpoint_t& point);

                void update_tracking(DepthFrame& depthFrame);
                void update_hand_frame(std::vector<TrackedPoint>& internalTrackedPoints, _sensekit_handframe& frame);

                void track_points(cv::Mat& matDepth, cv::Mat& matForeground);

                //fields
                PluginServiceProxy& m_pluginService;
                
                using ColorStream = PluginStream < sensekit_colorframe_wrapper_t > ;
                using ColorStreamPtr = std::unique_ptr < ColorStream > ;
                ColorStreamPtr m_handDebugImageStream;
                
                using HandStream = PluginStream<sensekit_handframe_wrapper_t>;
                using HandStreamPtr = std::unique_ptr<HandStream>;
                HandStreamPtr m_handStream;

                StreamReader m_reader;

                DepthUtility m_depthUtility;
                CoordinateConverter m_converter;
                PointProcessor m_pointProcessor;

                int m_width;
                int m_height;

                float m_resizeFactor;

                cv::Mat m_matDepth;
                cv::Mat m_matForeground;

            };
        }
    }
}

#endif // HANDTRACKER_H
