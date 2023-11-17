#include "SVCamMiniSample.h"

#ifdef SVGENSDK_DYNAMIC_LOAD
//#define SVS_SDK_DLL „Path to SVGenSDK32.dll/ SVGenSDK64.dll“// use this define to load specific SDK Dll path otherwise it will load Dll on SVS_SDK_BIN_32/ SVS_SDK_BIN_64 environmental variable
#include "sv_gen_sdk_dynamic.h"
#endif

unsigned long __stdcall AcquisitionThread(void *context)
{
    SVCamAcquisition * svacq = (SVCamAcquisition*)context;
    if(NULL == svacq)
        return -1;

    printf("%s AcquisitionThread Running \n", __FUNCTION__ );
    while(!svacq->acqTerminated)
    {
        if (svacq->imageBufferInfo.size() !=0)
        {
            // Obtain the image data pointer and characteristics
            SV_BUFFER_INFO  *NewImageInfo = svacq->imageBufferInfo.front();
            printf("new image:  %p\n", NewImageInfo->pImagePtr);
            svacq->imageBufferInfo.pop_front();
            delete NewImageInfo;
        }
        else
        {
            WaitForSingleObject(svacq->m_newImage, 1000);
            ResetEvent(svacq->m_newImage);
        }
    }
    printf("%s AcquisitionThread Exiting \n", __FUNCTION__ );
    return 0;
}

void PrintDevInfo(const SV_DEVICE_INFO &info)
{
    printf("******************Device Info*****************\n");
    printf("uid:%s\n", info.uid);
    printf("Vendor:%s\n", info.vendor);
    printf("Model:%s\n", info.model);
    printf("DisplayName:%s\n", info.displayName);
    printf("TLType:%s\n", info.tlType);
    printf("Access Status:0x%x\n", info.accessStatus);
    printf("User Defined Name:%s\n", info.userDefinedName);
    printf("Serial Number:%s\n", info.serialNumber);
    printf("Version:%s\n", info.version);
}

void PrintFeatureInfo( vector<SVCamFeaturInf*> sv_camfeatureinf)
{
    printf("--------------------------------------------------------------------\n");
    printf("******************Camera Feature Info*****************\n");
    printf( " ");
    for(std::vector<SVCamFeaturInf*>::iterator i = sv_camfeatureinf.begin()+1; i != sv_camfeatureinf.end(); i++)
    {
        printf( "|");
        for (int j =0; j <(*i)->SVFeaturInf.level; j++)
            printf( "--");

        printf( (*i)->SVFeaturInf.displayName);
        printf( ": "); printf( (*i)->strValue);   printf( "\n ");
    }
    printf("--------------------------------------------------------------------\n");
}


HANDLE g_hThreadStreamEvents = NULL;
HANDLE g_hThreadDeviceEvents = NULL;
HANDLE g_hThreadRemoteDeviceEvents = NULL;

typedef struct
{
    void *hModule;
    SV_EVENT_HANDLE hEvent;
}SV_EVENTS_CONTEXT;

typedef struct
{
    uint16_t reserved;
    uint16_t event_id;
    uint16_t stream_channel;
    uint16_t block_id;
    uint32_t timestamp_high;
    uint32_t timestamp_low;
}gv_event;


// demonstration  for event  
unsigned long __stdcall RemoteDeviceEventsThread(void *context)
{
    SV_EVENTS_CONTEXT *pData = (SV_EVENTS_CONTEXT*)context;
    if (NULL == pData)
        return -1;

    printf("RemoteDeviceEventsThread Running...\n", __FUNCTION__);
    while (true)
    {
        SV_EVENT_REMOTE_DEVICE_DATA eventData;
        size_t buffSize = sizeof(eventData);

        SV_RETURN ret = SVEventWait(pData->hModule, pData->hEvent, &eventData, &buffSize, INFINITE);
        if (SV_ERROR_ABORT == ret)
        {
            printf("SVEventWait SV_ERROR_ABORT\n");
            break;
        }
        else if (SV_ERROR_TIMEOUT == ret)
            printf("SVEventWait (TIMEOUT)!\n");
        else if (SV_ERROR_SUCCESS == ret)
        {
            gv_event *pGevEvent = (gv_event*)(eventData.data);
            if (pGevEvent)
                printf("SVEventWait Successful! Signal:0x%x BlockID:%d StreamChannel:%d\n", eventData.eventID , _byteswap_ushort(pGevEvent->block_id), _byteswap_ushort(pGevEvent->stream_channel)); //  _byteswap_ushort(pGevEvent->event_id)
        }
        else
            printf("SVEventWait Failed! %d\n", ret);
    }
    printf("RemoteDeviceEventsThread Exiting...\n", __FUNCTION__);
    return 0;
}

unsigned long __stdcall StreamEventsThread(void *context)
{
    SV_EVENTS_CONTEXT *pData = (SV_EVENTS_CONTEXT*)context;
    if (NULL == pData)
        return -1;

    printf("StreamEventsThread Running...\n", __FUNCTION__);
    while (true)
    {
        SV_EVENT_DS_ACQUISITION_DATA eventData = {0};
        size_t buffSize = sizeof(eventData);

        SV_RETURN ret = SVEventWait(pData->hModule, pData->hEvent, &eventData, &buffSize, 1000);
        if (SV_ERROR_ABORT == ret)
        {
            printf("SVEventWait SV_ERROR_ABORT\n");
            break;
        }
        else if (SV_ERROR_TIMEOUT == ret)
            printf("SVEventWait (TIMEOUT)!\n");
        else if (SV_ERROR_SUCCESS == ret)
            printf("SVEventWait Successful! Signal:%d ImageId:%d TimeStamp:%d\n", eventData.signalType, eventData.data.imageID, eventData.data.timeStamp);
        else
            printf("SVEventWait Failed! %d\n", ret);
    }
    printf("StreamEventsThread Exiting...\n", __FUNCTION__);
    return 0;
}

unsigned long __stdcall DeviceConnectionLostEventThread(void *context)
{
    SV_EVENTS_CONTEXT *pData = (SV_EVENTS_CONTEXT*)context;
    if (NULL == pData)
        return -1;

    printf("DeviceConnectionLostEventThread Running...\n", __FUNCTION__);
    while (true)
    {
        SV_EVENT_DEVICE_CONNECTION_DATA eventInfo;
        size_t buffSize = sizeof(eventInfo);

        SV_RETURN ret = SVEventWait(pData->hModule, pData->hEvent, &eventInfo, &buffSize, INFINITE);
        if (SV_ERROR_ABORT == ret)
        {
            printf("SVEventWait SV_ERROR_ABORT\n");
            break;
        }
        else if (SV_ERROR_TIMEOUT == ret)
            printf("SVEventWait (TIMEOUT)!\n");
        else if (SV_ERROR_SUCCESS == ret)
        {
            printf("SVEventWait Successful! Status:%d DeviceID:%s\n", eventInfo.status, eventInfo.deviceID);
        }
        else
            printf("SVEventWait Failed! %d\n", ret);
    }
    printf("DeviceConnectionLostEventThread Exiting...\n", __FUNCTION__);
    return 0;
}

SV_EVENT_HANDLE DeviceEventRegister(SV_DEVICE_HANDLE hDev, int EventID)
{
    SV_EVENT_HANDLE hEvent = NULL;
    SV_RETURN ret = SVEventRegister(hDev, EventID, &hEvent);
    if (SV_ERROR_SUCCESS != ret)
    {
        printf("SVEventRegister (SV_EVENT_STREAM_ACQUISITION_EVENTS) Failed!\n", __FUNCTION__);
        return NULL;
    }
    SV_EVENTS_CONTEXT *pData = new SV_EVENTS_CONTEXT;
    pData->hModule = hDev;
    pData->hEvent = hEvent;

    if (SV_EVENT_REMOTE_DEVICE == EventID)
        g_hThreadRemoteDeviceEvents = CreateThread(NULL, 0, RemoteDeviceEventsThread, pData, 0, NULL);
    else if (SV_EVENT_DEVICE_CONNECTION == EventID)
        g_hThreadDeviceEvents = CreateThread(NULL, 0, DeviceConnectionLostEventThread, pData, 0, NULL);

    return hEvent;
}

void DeviceEventUnregister(SV_DEVICE_HANDLE hDev, SV_EVENT_HANDLE hEvent)
{
    SV_RETURN ret = SVEventFlush(hDev, hEvent);
    if (SV_ERROR_SUCCESS != ret)
        printf("SVEventRegister SVEventFlush Failed! %d\n", ret);

    ret = SVEventKill(hDev, hEvent);
    if (SV_ERROR_SUCCESS != ret)
        printf("SVEventRegister SVEventKill Failed! %d\n", ret);

    ret = SVEventUnRegister(hDev, SV_EVENT_DEVICE_CONNECTION);
    if (SV_ERROR_SUCCESS != ret)
        printf("SVEventRegister SVEventKill Failed! %d\n", ret);
}

SV_EVENT_HANDLE StreamEventsRegister(SV_STREAM_HANDLE hDS)
{
    SV_EVENT_HANDLE hEvent = NULL;
    SV_RETURN ret = SVEventRegister(hDS, SV_EVENT_DS_ACQUISITION, &hEvent);
    if (SV_ERROR_SUCCESS != ret)
    {
        printf("SVEventRegister (SV_EVENT_STREAM_ACQUISITION_EVENTS) Failed!\n", __FUNCTION__);
        return NULL;
    }

    SV_EVENTS_CONTEXT *pData = new SV_EVENTS_CONTEXT;
    pData->hModule = hDS;
    pData->hEvent = hEvent;

    g_hThreadStreamEvents = CreateThread(NULL, 0, StreamEventsThread, pData, 0, NULL);

    return hEvent;
}

void StreamEventsUnregister(SV_STREAM_HANDLE hDS, SV_EVENT_HANDLE hEvent)
{
    SV_RETURN ret = SVEventFlush(hDS, hEvent);
    if (SV_ERROR_SUCCESS != ret)
        printf("SVEventRegister SVEventFlush Failed! %d\n", ret);

    ret = SVEventKill(hDS, hEvent);
    if (SV_ERROR_SUCCESS != ret)
        printf("SVEventRegister SVEventKill Failed! %d\n", ret);

    ret = SVEventUnRegister(hDS, SV_EVENT_DS_ACQUISITION);
    if (SV_ERROR_SUCCESS != ret)
        printf("SVEventRegister SVEventKill Failed! %d\n", ret);
}

bool InitSDK()
{
    string ctiPath;
    string genicamPath;
    string genicamCachePath;
    string clProtocolPath;
    char buffer[1024] = { 0 };
    
#ifdef _WIN64
    int res = GetEnvironmentVariableA("GENICAM_GENTL64_PATH", buffer, sizeof(buffer));
    if (0 == res)
        return false;
#else
    int res = GetEnvironmentVariableA("GENICAM_GENTL32_PATH", buffer, sizeof(buffer));
    if (0 == res)
        return false;
#endif

    ctiPath = string(buffer);
    memset(buffer, 0, sizeof(buffer));
    res = GetEnvironmentVariableA("SVS_GENICAM_ROOT", buffer, sizeof(buffer));
    if (0 == res)
        return false;

    genicamPath = string(buffer);
	memset(buffer, 0, sizeof(buffer));
    res = GetEnvironmentVariableA("SVS_GENICAM_CACHE", buffer, sizeof(buffer));
    if (0 == res)
        return false;

    genicamCachePath = string(buffer);
	memset(buffer, 0, sizeof(buffer));
    res = GetEnvironmentVariableA("SVS_GENICAM_CLPROTOCOL", buffer, sizeof(buffer));
    if (0 == res)
        return false;

    clProtocolPath = string(buffer);
	SV_RETURN ret = SVLibInit(ctiPath.c_str(), genicamPath.c_str(), genicamCachePath.c_str(), clProtocolPath.c_str());
    if (SV_ERROR_SUCCESS != ret)
    {
        printf("SVLibInit Failed! :%d", ret);
        return false;
    }

    return true;
}


SVCamSystem * GetSVCamSystem(vector<SVCamSystem *> sv_cam_sys_list, char *tlid)
{
    // Get the SVCamSyste with the selected TL id
    for (std::vector<SVCamSystem*>::iterator currentSystem = sv_cam_sys_list.begin(); currentSystem != sv_cam_sys_list.end(); currentSystem++)
    {
        std::string str((*currentSystem)->sv_tl_inf.id);
        if (str.compare(tlid) == 0)
        {
            return  (*currentSystem);
        }
    }
    return NULL;
}


Camera * GetCamera(SVCamSystem* svcam, char* id)
{
    // Get the Camera with the selected device id
    for (std::vector<Camera*>::iterator currentcam = svcam->sv_cam_list.begin(); currentcam != svcam->sv_cam_list.end(); currentcam++)
    {
        std::string str((*currentcam)->devInfo.uid);
        if (str.compare(id) == 0)
        {
            return  (*currentcam);
        }
    }
    return NULL;
}

bool svcam_discover(vector<SV_DEVICE_INFO *>& devInfoList, vector<char *>& tlIDList, vector<SVCamSystem *>& svCamSysList){
    SV_RETURN ret = SV_ERROR_SUCCESS;
    uint32_t tlCount = 0;
    SVLibSystemGetCount(&tlCount);

    int index = 0;
    for (uint32_t i = 0; i < tlCount; i++)
    {
		SV_TL_INFO tlInfo = {0};
        ret = SVLibSystemGetInfo(i, &tlInfo);
        if (SV_ERROR_SUCCESS != ret)
            continue;

        SVCamSystem *new_svcam = new SVCamSystem(tlInfo);
        if (new_svcam->SVCamSystemInit(i))
        {
			svCamSysList.push_back(new_svcam);
            new_svcam->deviceDiscovery(5000);
            for (int j = 0; j < new_svcam->devInfoList.size(); j++)
            {
				SV_DEVICE_INFO * devInfo = new_svcam->devInfoList.at(j);
				tlIDList.push_back(new_svcam->sv_tl_inf.id);
				devInfoList.push_back(devInfo);
				index++;
            }
        }
        else
        {
            delete new_svcam;
            continue;
        }
    }
    if (tlCount >= 1) {
        return true;
    } else {
        return false;
    }
}

bool svcam_connect(vector<SV_DEVICE_INFO *> devInfoList, vector<SVCamSystem *> svCamSysList, vector<char *> tlIDList, Camera* cam){
    SVCamSystem *sv_cam = NULL;
	SV_DEVICE_INFO* devinf = NULL;
	sv_cam = GetSVCamSystem(svCamSysList, tlIDList.at(0));
    devinf = devInfoList.at(0);
    
    sv_cam->openDevice(*devinf);
    cam = GetCamera(sv_cam, devinf->uid);
    
    if (cam == NULL){
        return false;
    } else {
        return true;
    }
}

bool svcam_set_parameter(Camera* cam, int expTime_ns){
    printf("set measurement parameter...\n");
    SV_FEATURE_HANDLE hFeature = NULL;
    SVFeatureGetByName(cam->sv_cam_acq->hRemoteDev, "TriggerMode", &hFeature);
    SVFeatureSetValueInt64Enum(cam->sv_cam_acq->hRemoteDev, hFeature, 1);

    hFeature = NULL;
    SVFeatureGetByName(cam->sv_cam_acq->hRemoteDev, "ExposureTime", &hFeature);
    SV_RETURN ret = SVFeatureSetValueFloat(cam->sv_cam_acq->hRemoteDev, hFeature, expTime_ns);

    if (SV_ERROR_SUCCESS != ret){
        return false;
    } else {
        return true;
    }
}

bool svcam_aquire_image(Camera* cam, int expTime_ns){
    printf("open streaming channel and start acquisition...\n");
    unsigned int bufcount = 1;
    cam->sv_cam_acq->AcquisitionStart(bufcount);

    SV_FEATURE_HANDLE hFeature = NULL;
    UINT32 timeOut = 1000;
    SVFeatureGetByName(cam->sv_cam_acq->hRemoteDev, "TriggerSoftware", &hFeature);

    SV_RETURN ret = SVFeatureCommandExecute(cam->sv_cam_acq->hRemoteDev, hFeature, timeOut);
    Sleep(int(expTime_ns/1000 + 100));
    if (SV_ERROR_SUCCESS != ret){
        return false;
    } else {
        return true;
    }
}

bool svcam_save_image(Camera* cam, const char* img_name){
    SV_RETURN ret = SVUtilSaveImageToFile(*(cam->sv_cam_acq->imageBufferInfo.front()), img_name, SV_IMAGE_FILE_PNG);
    if (SV_ERROR_SUCCESS != ret){
        return false;
    } else {
        return true;
    }
}

bool svcam_disconnect(vector<SV_DEVICE_INFO *> devInfoList, vector<SVCamSystem *> svCamSysList, vector<char *> tlIDList){
    if (!devInfoList.empty())
        devInfoList.clear();
	if (!tlIDList.empty())
		tlIDList.clear();
	if (!svCamSysList.empty())
		DSDeleteContainer(svCamSysList);

    printf("Close the library and free all the allocated resources.\n");
    SV_RETURN ret = SVLibClose();
    if (SV_ERROR_SUCCESS != ret){
        return false;
    } else {
        return true;
    }
}
