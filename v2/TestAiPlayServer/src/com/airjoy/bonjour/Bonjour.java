
package com.airjoy.bonjour;

import android.content.Context;
import android.net.wifi.WifiManager;
import android.util.Log;

import com.airjoy.bonjour.serviceinfo.BonjourServiceInfo;
import com.airjoy.util.NetWork;

import java.io.IOException;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.ArrayList;

import javax.jmdns.JmDNS;
import javax.jmdns.ServiceEvent;
import javax.jmdns.ServiceInfo;
import javax.jmdns.ServiceListener;

public class Bonjour implements ServiceListener {

    private static final String TAG = Bonjour.class.getSimpleName();
    private static final Bonjour mSingle = new Bonjour();

    private WifiManager.MulticastLock mWifiLock = null;
    private JmDNS mJmdns = null;
    private BonjourListener mListener = null;
    private Context mContext = null;
    private Status mStatus = Status.Stopped;
    
    private enum Status {
        Stopped,
        Stopping,
        Started,
        Starting,
    }
    
    private Map<String, ServiceInfo> mSvcInfoList = new HashMap<String, ServiceInfo>();
    private ArrayList<String> mSvcType = new ArrayList<String>();

    private Bonjour() {
    }

    public static Bonjour getInstance() {
        return mSingle;
    }

    public void setContent(Context context) {
        mContext = context;
    }

    public void setListener(BonjourListener listener) {
        mListener = listener;
    }

    public boolean isStarted() {
        return (mStatus == Status.Starting || mStatus == Status.Started); 
    }

    public void start() {
        if (mStatus == Status.Stopped) {
            mStatus = Status.Starting;
            new StartTask();    
        }
    }

    public void stop() {
        if (mStatus == Status.Started) {
            mStatus = Status.Stopping;
            new StopTask();    
        } 
    }

    public void publishService(BonjourServiceInfo svcInfo) {
        new PublishTask(svcInfo);
    }

    public boolean unpublishService(String svcType) {
        synchronized (this) {
            if (mJmdns == null) {
                return false;
            }
            
            ServiceInfo serviceInfo = mSvcInfoList.get(svcType);
            if (serviceInfo != null) {
                Log.v(TAG, String.format("unregisterService: %s", svcType));
                mJmdns.unregisterService(serviceInfo);
                mSvcInfoList.remove(svcType);
            }
            
            return true;
        }
    }

    public boolean discoveryService(String serviceType) {
        synchronized (this) {
            if (mJmdns == null) {
                return false;
            }
            
            mSvcType.add(serviceType);

            if (mStatus == Status.Started) {
                Log.d(TAG, String.format("discoveryService: %s", serviceType));
                mJmdns.addServiceListener(serviceType, this);
            }
            
            return true;
        }
    }

    public boolean undiscoveryService(String serviceType) {
        synchronized (this) {
            if (mJmdns == null) {
                return false;
            }

            mSvcType.remove(serviceType);

            if (mStatus == Status.Started) {
                Log.d(TAG, String.format("discoveryService: %s", serviceType));
                mJmdns.removeServiceListener(serviceType, this);
            }
            
            return true;
        }
    }


    private class StartTask implements Runnable {
        private Thread mThread = null;

        public StartTask() {
            mThread = new Thread(this);
            mThread.start();
        }

        @Override
        public void run() {
            synchronized (this) {
                Log.v(TAG, "start");

                WifiManager wifi = (WifiManager) mContext
                        .getSystemService(android.content.Context.WIFI_SERVICE);

                mWifiLock = wifi.createMulticastLock("bonjourlock");
                mWifiLock.setReferenceCounted(true);
                mWifiLock.acquire();

                try {
                    byte[] ip = NetWork.getLocalIpInt(mContext);
                    if (ip == null) {
                    	Log.e(TAG, "JmDNS.create() failed! ip is null");
                    	
                        mStatus = Status.Stopped;
                    	mListener.onStartFailed();
                        return;
                    }

                    InetAddress addr = InetAddress.getByAddress(ip);

                    mJmdns = JmDNS.create(addr);
                    Log.d(TAG, String.format("JmDNS version: %s (%s)", JmDNS.VERSION,
                            addr.getHostAddress()));

                    mStatus = Status.Started;
                    mListener.onStarted();
                } catch (IOException e) {
                    Log.e(TAG, "JmDNS.create() failed!");
                    e.printStackTrace();
                    
                    mStatus = Status.Stopped;
                    mListener.onStartFailed();
                }
            }
        }
    }

    private class StopTask implements Runnable {
        private Thread mThread = null;

        public StopTask() {
            mThread = new Thread(this);
            mThread.start();
        }

        @Override
        public void run() {
            synchronized (Bonjour.this) {
                Log.v(TAG, "stop");
                if (mJmdns != null) {
                	mJmdns.unregisterAllServices();
                    mSvcInfoList.clear();

                	try {
                        mJmdns.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    
                    mJmdns = null;
                }

                if (mWifiLock != null) {
                    mWifiLock.setReferenceCounted(false);
                    mWifiLock.release();
                    mWifiLock = null;
                }

                mStatus = Status.Stopped;
                mListener.onStopped();
            }
        }
    }

    private class PublishTask implements Runnable {
        private Thread mThread = null;
        private BonjourServiceInfo mSvcInfo = null;

        public PublishTask(BonjourServiceInfo svcInfo) {
            mSvcInfo = svcInfo;
            mThread = new Thread(this);
            mThread.start();
        }

        @Override
        public void run() {
            synchronized (Bonjour.this) {
                if (mJmdns == null) {
                    return;
                }

                ServiceInfo serviceInfo = ServiceInfo.create(mSvcInfo.getServiceType(),
                        mSvcInfo.getServiceName(),
                        mSvcInfo.getServicePort(),
                        0,
                        0,
                        mSvcInfo.getProperties());

                if (!mSvcInfoList.containsKey(mSvcInfo.getServiceType())) {
                    mSvcInfoList.put(mSvcInfo.getServiceType(), serviceInfo);

                    try {
                        Log.v(TAG, String.format("registerService: %s (%s)",
                                mSvcInfo.getServiceType(),
                                mSvcInfo.getServiceName()));
                        mJmdns.registerService(serviceInfo);
                    } catch (IOException e) {
                        e.printStackTrace();
                    } catch (IllegalStateException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    @Override
    public void serviceAdded(ServiceEvent event) {
        Log.d(TAG,
                String.format("serviceAdded: %s.%s", event.getName(),
                        event.getType()));
        mJmdns.requestServiceInfo(event.getType(), event.getName());
    }

    @Override
    public void serviceRemoved(ServiceEvent event) {
        String name = event.getName();
        String type = event.getType();
        String ip = null;

        Inet4Address[] addresses = event.getInfo().getInet4Addresses();
        for (int i = 0; i < addresses.length; ++i) {
            ip = addresses[i].getHostAddress();
        }

        Log.d(TAG, String.format("serviceRemoved: %s.%s %s", event.getName(),
                event.getType(), ip));
        mListener.onServiceLost(name, type, ip);
    }

    @Override
    public void serviceResolved(ServiceEvent event) {
        String name = event.getName();
        String type = event.getType();
        int port = event.getInfo().getPort();
        String ip = null;

        Inet4Address[] addresses = event.getInfo().getInet4Addresses();
        for (int i = 0; i < addresses.length; ++i) {
            ip = addresses[i].getHostAddress();

            Log.d(TAG,
                    String.format("serviceResolved: %s.%s %s:%d",
                            event.getName(), event.getType(), ip, port));
        }

        if (ip == null)
            return;

        Map<String, String> properties = new HashMap<String, String>();

        Enumeration<String> propertyNames = event.getInfo().getPropertyNames();
        while (propertyNames.hasMoreElements()) {
            String key = propertyNames.nextElement();
            String value = event.getInfo().getPropertyString(key);
            properties.put(key, value);
        }

        mListener.onServiceFound(name, type, ip, port, properties);
    }
}
