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

bool svcam_discover(vector<SV_DEVICE_INFO *>& devInfoList, vector<SVCamSystem *>& svCamSysList, vector<char *>& tlIDList);
bool svcam_connect(vector<SV_DEVICE_INFO *> devInfoList, vector<SVCamSystem *> svCamSysList, vector<char *> tlIDList, Camera*& cam);
bool svcam_set_parameter(Camera* cam, int expTime_ns);
bool svcam_aquire_image(Camera* cam, int expTime_ns);
bool svcam_save_image(Camera* cam, const char* img_name);
bool svcam_disconnect(vector<SV_DEVICE_INFO *> devInfoList, vector<SVCamSystem *> svCamSysList, vector<char *> tlIDList);

