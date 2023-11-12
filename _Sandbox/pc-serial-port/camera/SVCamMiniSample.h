#include "SVCamSystem.h"

unsigned long __stdcall AcquisitionThread(void *context);
void PrintDevInfo(const SV_DEVICE_INFO &info);
void PrintFeatureInfo( vector<SVCamFeaturInf*> sv_camfeatureinf);
unsigned long __stdcall RemoteDeviceEventsThread(void *context);
unsigned long __stdcall StreamEventsThread(void *context);
unsigned long __stdcall DeviceConnectionLostEventThread(void *context);
SV_EVENT_HANDLE DeviceEventRegister(SV_DEVICE_HANDLE hDev, int EventID);
void DeviceEventUnregister(SV_DEVICE_HANDLE hDev, SV_EVENT_HANDLE hEvent);
SV_EVENT_HANDLE StreamEventsRegister(SV_STREAM_HANDLE hDS);
void StreamEventsUnregister(SV_STREAM_HANDLE hDS, SV_EVENT_HANDLE hEvent);

bool InitSDK();
SVCamSystem * GetSVCamSystem(vector<SVCamSystem *> sv_cam_sys_list, char *tlid);
Camera * GetCamera(SVCamSystem* svcam, char* id);

void acquire_image(double expTime_ns);

