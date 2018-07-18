/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace RakNet {

using System;
using System.Runtime.InteropServices;

public class PluginInterface2 : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal PluginInterface2(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(PluginInterface2 obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~PluginInterface2() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          RakNetPINVOKE.delete_PluginInterface2(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public PluginInterface2() : this(RakNetPINVOKE.new_PluginInterface2(), true) {
  }

  public virtual bool UsesReliabilityLayer() {
    bool ret = RakNetPINVOKE.PluginInterface2_UsesReliabilityLayer(swigCPtr);
    return ret;
  }

  public virtual void OnReliabilityLayerNotification(string errorMessage, uint bitsUsed, SystemAddress remoteSystemAddress, bool isError) {
    RakNetPINVOKE.PluginInterface2_OnReliabilityLayerNotification(swigCPtr, errorMessage, bitsUsed, SystemAddress.getCPtr(remoteSystemAddress), isError);
    if (RakNetPINVOKE.SWIGPendingException.Pending) throw RakNetPINVOKE.SWIGPendingException.Retrieve();
  }

  public RakPeerInterface GetRakPeerInterface() {
    IntPtr cPtr = RakNetPINVOKE.PluginInterface2_GetRakPeerInterface(swigCPtr);
    RakPeerInterface ret = (cPtr == IntPtr.Zero) ? null : new RakPeerInterface(cPtr, false);
    return ret;
  }

  public RakNetGUID GetMyGUIDUnified() {
    RakNetGUID ret = new RakNetGUID(RakNetPINVOKE.PluginInterface2_GetMyGUIDUnified(swigCPtr), true);
    return ret;
  }

}

}
