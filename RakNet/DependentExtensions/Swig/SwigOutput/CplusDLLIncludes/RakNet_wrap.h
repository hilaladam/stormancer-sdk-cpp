/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.12
 *
 * This file is not intended to be easily readable and contains a number of
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG
 * interface file instead.
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_RakNet_WRAP_H_
#define SWIG_RakNet_WRAP_H_

class SwigDirector_NetworkIDObject : public RakNet::NetworkIDObject, public Swig::Director {

public:
    SwigDirector_NetworkIDObject();
    virtual ~SwigDirector_NetworkIDObject();
    virtual void SetNetworkIDManager(RakNet::NetworkIDManager *manager);
    virtual RakNet::NetworkIDManager *GetNetworkIDManager() const;
    virtual RakNet::NetworkID GetNetworkID();
    virtual void SetNetworkID(RakNet::NetworkID id);
    virtual void SetParent(void *_parent);
    virtual void *GetParent() const;

    typedef void (SWIGSTDCALL* SWIG_Callback0_t)(void *);
    typedef void * (SWIGSTDCALL* SWIG_Callback1_t)();
    typedef unsigned long long (SWIGSTDCALL* SWIG_Callback2_t)();
    typedef void (SWIGSTDCALL* SWIG_Callback3_t)(unsigned long long);
    void swig_connect_director(SWIG_Callback0_t callbackSetNetworkIDManagerOrig, SWIG_Callback1_t callbackGetNetworkIDManager, SWIG_Callback2_t callbackGetNetworkID, SWIG_Callback3_t callbackSetNetworkID);

private:
    SWIG_Callback0_t swig_callbackSetNetworkIDManagerOrig;
    SWIG_Callback1_t swig_callbackGetNetworkIDManager;
    SWIG_Callback2_t swig_callbackGetNetworkID;
    SWIG_Callback3_t swig_callbackSetNetworkID;
    void swig_init_callbacks();
};

class SwigDirector_FileListTransferCBInterface : public RakNet::FileListTransferCBInterface, public Swig::Director {

public:
    SwigDirector_FileListTransferCBInterface();
    virtual ~SwigDirector_FileListTransferCBInterface();
    virtual bool OnFile(OnFileStruct *onFileStruct);
    virtual void OnFileProgress(FileProgressStruct *fps);
    virtual bool Update();
    virtual bool OnDownloadComplete(DownloadCompleteStruct *dcs);
    virtual void OnDereference();

    typedef unsigned int (SWIGSTDCALL* SWIG_Callback0_t)(void *);
    typedef void (SWIGSTDCALL* SWIG_Callback1_t)(void *);
    typedef unsigned int (SWIGSTDCALL* SWIG_Callback2_t)();
    typedef unsigned int (SWIGSTDCALL* SWIG_Callback3_t)(void *);
    typedef void (SWIGSTDCALL* SWIG_Callback4_t)();
    void swig_connect_director(SWIG_Callback0_t callbackOnFile, SWIG_Callback1_t callbackOnFileProgress, SWIG_Callback2_t callbackUpdate, SWIG_Callback3_t callbackOnDownloadComplete, SWIG_Callback4_t callbackOnDereference);

private:
    SWIG_Callback0_t swig_callbackOnFile;
    SWIG_Callback1_t swig_callbackOnFileProgress;
    SWIG_Callback2_t swig_callbackUpdate;
    SWIG_Callback3_t swig_callbackOnDownloadComplete;
    SWIG_Callback4_t swig_callbackOnDereference;
    void swig_init_callbacks();
};


#endif
