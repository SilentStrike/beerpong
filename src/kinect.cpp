#include "kinect.h"

void Kinect::VideoCallback(void* _rgb, uint32_t timestamp)
{
    m_rgb_mutex.lock();
    uint8_t* rgb = static_cast<uint8_t*>(_rgb);
    rgbMat.data = rgb;
    m_new_rgb_frame = true;
    m_rgb_mutex.unlock();
}

void Kinect::DepthCallback(void* _depth, uint32_t timestamp)
{
    m_depth_mutex.lock();
    uint16_t* depth = static_cast<uint16_t*>(_depth);
    depthMat.data = (uchar*) depth;
    m_new_depth_frame = true;
    m_depth_mutex.unlock();
}

bool Kinect::GetVideo(Mat& output)
{
    m_rgb_mutex.lock();
    if(m_new_rgb_frame) {
        cv::cvtColor(rgbMat, output, CV_RGB2BGR);
        m_new_rgb_frame = false;
        m_rgb_mutex.unlock();
        return true;
    } else {
        m_rgb_mutex.unlock();
        return false;
    }
}

bool Kinect::GetDepth(Mat& output)
{
    m_depth_mutex.lock();
    if(m_new_depth_frame) {
        depthMat.copyTo(output);
        m_new_depth_frame = false;
        m_depth_mutex.unlock();
        return true;
    } else {
        m_depth_mutex.unlock();
        return false;
    }
}
