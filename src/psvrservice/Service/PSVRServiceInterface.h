#ifndef PSVR_SERVICE_INTERFACE_H
#define PSVR_SERVICE_INTERFACE_H

//-- includes -----
#include "PSVRClient_CAPI.h"

//-- definitions -----
enum DeviceCategory
{
	DeviceCategory_TRACKER= 0,
	DeviceCategory_HMD= 1
}

struct TrackerDataPacket
{
	PSVRTrackerID tracker_id; 
	PSVRTrackerType tracker_type;
    int sequence_num;
	bool is_connected;
};
	
struct HMDDataPacket
{
    PSVRHmdID hmd_id;
    PSVRHmdType hmd_type;
    union
    {
        PSVRMorpheus  morpheus_state;
        PSVRVirtualHMD virtual_hmd_state;
    }               hmd_state;
    bool            is_valid;
    int             output_sequence_num;
    bool            is_connected;
};	
	
struct DeviceOutputDataFrame
{
	union{
		TrackerDataPacket tracker_data_packet;
		HMDDataPacket hmd_data_packet;
	} device;
	DeviceCategory device_category;
};

class SharedVideoFrameBuffer
{
public:
    SharedVideoFrameBuffer();
    ~SharedVideoFrameBuffer();

    bool initialize(const char *buffer_name, int width, int height, int stride);
    void dispose();
    void writeVideoFrame(const unsigned char *buffer);
    const unsigned char *getBuffer() const;
    unsigned char *getBufferMutable();
    static size_t computeVideoBufferSize(int stride, int height);

private:
    std::string m_shared_memory_name;
	int m_width;
    int m_height;
    int m_stride;
	unsigned char *m_buffer;
	int m_frame_index;	
};

//-- interface -----
class INotificationListener
{
public:
    virtual void handle_notification(const PSVREventMessage &response) = 0;
};

class IDataFrameListener
{
public:
    virtual void handle_data_frame(const DeviceOutputDataFrame &data_frame) = 0;
};

#endif  // PSVRPROTOCOL_INTERFACE_H