package com.airjoy.asio.rtsp.client;

public class RtspClientException extends Exception {

    private static final long serialVersionUID = 4771667622064144051L;

    public RtspClientException(String message) {
        super(message);
    }

    public RtspClientException(Exception e) {
        super(e);
    }
}