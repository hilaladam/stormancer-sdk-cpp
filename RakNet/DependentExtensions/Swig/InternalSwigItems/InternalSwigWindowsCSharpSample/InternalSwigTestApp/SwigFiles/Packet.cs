/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace RakNet {

using System;
using System.Runtime.InteropServices;

public class Packet : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Packet(IntPtr cPtr, bool cMemoryOwn) 
  {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(Packet obj)
  {
     if (obj != null)
     {
        if (obj.dataIsCached)
        {
           obj.SetPacketData(obj.data, obj.data.Length); //If an individual index was modified we need to copy the data before passing to C++
        }
	obj.dataIsCached=false;
     }
     return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~Packet() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          RakNetPINVOKE.delete_Packet(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

    private bool dataIsCached = false;
    private byte[] dataCache;

  public SystemAddress systemAddress {
    set {
      RakNetPINVOKE.Packet_systemAddress_set(swigCPtr, SystemAddress.getCPtr(value));
    } 
    get {
      IntPtr cPtr = RakNetPINVOKE.Packet_systemAddress_get(swigCPtr);
      SystemAddress ret = (cPtr == IntPtr.Zero) ? null : new SystemAddress(cPtr, false);
      return ret;
    } 
  }

  public RakNetGUID guid {
    set {
      RakNetPINVOKE.Packet_guid_set(swigCPtr, RakNetGUID.getCPtr(value));
    } 
    get {
      IntPtr cPtr = RakNetPINVOKE.Packet_guid_get(swigCPtr);
      RakNetGUID ret = (cPtr == IntPtr.Zero) ? null : new RakNetGUID(cPtr, false);
      return ret;
    } 
  }

  public uint length {
    set {
      RakNetPINVOKE.Packet_length_set(swigCPtr, value);
    } 
    get {
      uint ret = RakNetPINVOKE.Packet_length_get(swigCPtr);
      return ret;
    } 
  }

  public uint bitSize {
    set {
      RakNetPINVOKE.Packet_bitSize_set(swigCPtr, value);
    } 
    get {
      uint ret = RakNetPINVOKE.Packet_bitSize_get(swigCPtr);
      return ret;
    } 
  }

  public byte[] data {
	set 
	{
	    	dataCache=value;
		dataIsCached = true;
		SetPacketData (value, value.Length);
	    

	}

        get
        {
            byte[] returnBytes;
            if (!dataIsCached)
            {
                IntPtr cPtr = RakNetPINVOKE.Packet_data_get (swigCPtr);
                int len = (int)((Packet)swigCPtr.Wrapper).length;
		if (len<=0)
		{
			return null;
		}
                returnBytes = new byte[len];
                Marshal.Copy(cPtr, returnBytes, 0, len);
                dataCache = returnBytes;
                dataIsCached = true;
            }
            else
            {
                returnBytes = dataCache;
            }
            return returnBytes;
        }
 
  }

  public bool deleteData {
    set {
      RakNetPINVOKE.Packet_deleteData_set(swigCPtr, value);
    } 
    get {
      bool ret = RakNetPINVOKE.Packet_deleteData_get(swigCPtr);
      return ret;
    } 
  }

  public bool wasGeneratedLocally {
    set {
      RakNetPINVOKE.Packet_wasGeneratedLocally_set(swigCPtr, value);
    } 
    get {
      bool ret = RakNetPINVOKE.Packet_wasGeneratedLocally_get(swigCPtr);
      return ret;
    } 
  }

  public Packet() : this(RakNetPINVOKE.new_Packet(), true) {
  }

  public void SetPacketData(byte[] inByteArray, int numBytes) {
    RakNetPINVOKE.Packet_SetPacketData(swigCPtr, inByteArray, numBytes);
  }

}

}
