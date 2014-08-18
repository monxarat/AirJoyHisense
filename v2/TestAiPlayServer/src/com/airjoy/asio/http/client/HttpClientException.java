package com.airjoy.asio.http.client;

public class HttpClientException extends Exception {

    private static final long serialVersionUID = 4771667622064144051L;

    public HttpClientException(String message) {
        super(message);
    }

    public HttpClientException(Exception e) {
        super(e);
    }
}