/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.9
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class dspJNI {
  public final static native void g_current_volume_set(int jarg1);
  public final static native int g_current_volume_get();
  public final static native boolean initDSP();
  public final static native boolean dspEnd();
  public final static native boolean setVolume(int jarg1);
  public final static native boolean getVolume(long jarg1);
  public final static native boolean increase_Volume();
  public final static native boolean decrease_Volume();
  public final static native boolean doMute();
  public final static native boolean doDeMute();
  public final static native boolean SetBalance(int jarg1);
  public final static native boolean SetFader(int jarg1);
  public final static native boolean SetLoudness(int jarg1);
  public final static native boolean setSource(int jarg1);
  public final static native boolean getSource(long jarg1);
  public final static native boolean playSin(int jarg1, int jarg2, boolean jarg3);
  public final static native boolean change_band(int jarg1);
  public final static native void get_band(int jarg1, int jarg2);
  public final static native boolean set_frequency(int jarg1, int jarg2, int jarg3);
  public final static native int get_frequency();
}
