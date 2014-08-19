
package com.airjoy.util;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;

public class RtspMessage {

    public enum Type {
        Unknown,
        Request,
        Response,
    }

    public enum LoadStatus {
        Unknown,
        Done,
        Error,
        Incomplete,
        HeaderIncomplete,
    }

    private static final String ContentLengthName = "Content-Length";

    private Type mType = Type.Unknown;
    private String mMethod = null;
    private int mResponseCode = 0;
    private String mResponseStatus = null;
    private String mUri = null;
    private int mVersionMajor = 0;
    private int mVersionMinor = 0;
    private byte[] mContent = null;
    private int mContentPosition = 0;
    private int mContentLength = 0;

    // for loadBytes()
    private int mConsumeLength = 0;
    private LoadStatus mLoadStatus = LoadStatus.Unknown;
    private int mContentLengthLoaded = 0;

    // The headers included with the request.
    private Map<String, String> mHeaders = new HashMap<String, String>();

    // load Rtsp data with content
    public LoadStatus loadBytes(byte[] bytes, int len) {
        if (mLoadStatus == LoadStatus.Incomplete)
        {
            int loadLength = loadContent(bytes, len, 0);

            mConsumeLength = loadLength;
            mConsumeLength++;

            mContentLengthLoaded += loadLength;

            mLoadStatus = (mContentLengthLoaded < mContentLength) ? LoadStatus.Incomplete
                    : LoadStatus.Done;
            return mLoadStatus;
        }

        mType = Type.Unknown;
        mResponseCode = 0;
        mResponseStatus = null;
        mMethod = null;
        mUri = null;
        mVersionMajor = 0;
        mVersionMinor = 0;
        mHeaders.clear();
        mContentLength = 0;
        mContent = null;
        mConsumeLength = 0;

        int readableLength = len;
        int requestLinelength = loadStatusLine(bytes, len);
        if (requestLinelength == 0)
        {
            requestLinelength = loadRequestLine(bytes, len);
            if (requestLinelength == 0)
            {
                mLoadStatus = LoadStatus.Error;
                return mLoadStatus;
            }
        }

        mConsumeLength += requestLinelength;

        readableLength = len - requestLinelength;
        int headLength = loadHeads(bytes, len, requestLinelength);
        if (headLength == 0) {
            mLoadStatus = LoadStatus.Incomplete;
            return mLoadStatus;
        }

        mConsumeLength += headLength;

        String contentLength = this.getValueByName(RtspMessage.ContentLengthName);
        if (contentLength == null) {
            mLoadStatus = LoadStatus.Done;
            return mLoadStatus;
        }

        try {
            String value = contentLength.trim();
            mContentLength = Integer.valueOf(value);
        } catch (NumberFormatException e) {
            mContentLength = 0;
        }

        if (mContentLength == 0) {
            mLoadStatus = LoadStatus.Done;
            return mLoadStatus;
        }

        mLoadStatus = LoadStatus.Incomplete;

        mContent = new byte[mContentLength];
        mContentPosition = 0;

        readableLength = len - requestLinelength - headLength;
        if (readableLength > 0)
        {
            mContentLengthLoaded = loadContent(bytes, len, requestLinelength + headLength);

            mConsumeLength += mContentLengthLoaded;

            if (mContentLengthLoaded >= mContentLength)
                mLoadStatus = LoadStatus.Done;
            else
                mLoadStatus = LoadStatus.Incomplete;
        }

        return mLoadStatus;
    }

    // load Rtsp data without content
    public boolean loadBytesWithoutContent(byte[] bytes, int len) {
        mType = Type.Unknown;
        mResponseCode = 0;
        mResponseStatus = null;
        mMethod = null;
        mUri = null;
        mVersionMajor = 0;
        mVersionMinor = 0;
        mHeaders = null;
        mContentLength = 0;
        mContent = null;

        int requestLinelength = loadStatusLine(bytes, len);
        if (requestLinelength == 0)
        {
            requestLinelength = loadRequestLine(bytes, len);
            if (requestLinelength == 0)
                return false;
        }

        int headLength = loadHeads(bytes, len, requestLinelength);
        if (headLength == 0)
            return false;

        String contentLength = this.getValueByName(RtspMessage.ContentLengthName);
        if (contentLength != null)
            mContentLength = Integer.getInteger(contentLength);

        return true;
    }

    // return the length of loadBytes()
    public int getConsumeLength() {
        if (mLoadStatus == LoadStatus.Error)
            return 0;

        if (mLoadStatus == LoadStatus.Incomplete)
            return mConsumeLength;

        if (mLoadStatus == LoadStatus.Done)
            return mConsumeLength;

        return 0;
    }

    // convert RtspMessage to bytes stream
    public byte[] toBytes() {

        if (mType == Type.Unknown)
            return null;

        String header = null;
        if (mType == Type.Request) {
            header = String.format("%s %s Rtsp/%d.%d\r\n", mMethod, mUri, mVersionMajor,
                    mVersionMinor);
        }
        else {
            header = String.format("Rtsp/%d.%d %d %s\r\n", mVersionMajor, mVersionMinor,
                    mResponseCode, mResponseStatus);
        }

        Iterator<Entry<String, String>> iter = mHeaders.entrySet().iterator();
        while (iter.hasNext()) {
            Entry<String, String> entry = iter.next();
            String key = entry.getKey();
            String value = entry.getValue();
            String line = String.format("%s: %s\r\n", key, value);

            header = String.format("%s%s", header, line);
        }

        header = String.format("%s\r\n", header);

        if (mContent == null)
            return header.getBytes();

        return Bytes.cat(header.getBytes(), mContent);
    }

    // The Rtsp type, e.g. "Rtsp-REQUEST" or "Rtsp-RESPONSE"
    public Type getType() {
        return mType;
    }

    public void setType(Type type) {
        mType = type;
    }

    // The request method, e.g. "GET", "POST", "PUT".
    public String getMethod() {
        return mMethod;
    }

    public void setMethod(String method) {
        mMethod = method;
    }

    // / Rtsp response code, e.g. 200
    public int getStatusCode() {
        return mResponseCode;
    }

    public void setResponseCode(int code) {
        mResponseCode = code;
    }

    public void setResponse(int code, String status) {
        mResponseCode = code;
        mResponseStatus = status;
    }

    // The Rtsp response status
    public String getStatus() {
        return mResponseStatus;
    }

    public void setResponseStatus(String status) {
        mResponseStatus = status;
    }

    // The requested URI, such as a path to a file.
    public String getUri() {
        return mUri;
    }

    public void setUri(String uri) {
        mUri = uri;
    }

    public void setVersion(int major, int minor) {
        mVersionMajor = major;
        mVersionMinor = minor;
    }

    // / Major version number, usually 1.
    public int getMajorVersion() {
        return mVersionMajor;
    }

    public void setVersionMajor(int m) {
        mVersionMinor = m;
    }

    // / Minor version number, usually 0 or 1.
    public int getMinorVersion() {
        return mVersionMinor;
    }

    public void setVersionMinor(int m) {
        mVersionMinor = m;
    }

    // / The optional content sent with the request.
    public byte[] getContent() {
        return mContent;
    }

    public void setContent(byte[] bytes) {
        mContent = bytes;
    }

    public int getContentLength() {
        return mContentLength;
    }

    public void addHeaders(String name, String value) {
        mHeaders.put(name, value);
    }

    public void addHeaders(String name, int value) {
        mHeaders.put(name, String.format("%d", value));
    }

    public String getValueByName(String name) {
        String lowerName = name.toLowerCase(Locale.getDefault());
        return mHeaders.get(lowerName);
    }

    // value in URI
    public String getValueInUri(String key) {
        int position = mUri.indexOf(key);
        if (position == -1)
            return null;

        String value = mUri.substring(position + key.length());
        return value;
    }

    /*
     * Pivate API
     */

    // return length of status line
    private int loadStatusLine(byte[] bytes, int len) {
        int p = 0;

        if (bytes == null)
            return 0;

        if (len == 0)
            return 0;

        // Rtsp protocol identifier.
        if (bytes[p++] != 'R')
            return 0;

        if (p >= len)
            return 0;

        if (bytes[p++] != 'T')
            return 0;

        if (p >= len)
            return 0;

        if (bytes[p++] != 'S')
            return 0;

        if (p >= len)
            return 0;

        if (bytes[p++] != 'P')
            return 0;

        if (p >= len)
            return 0;

        // Slash.
        if (bytes[p++] != '/')
            return 0;

        if (p >= len)
            return 0;

        // Major version number.
        if (!isDigit(bytes[p]))
            return 0;

        mVersionMajor = 0;
        while (isDigit(bytes[p]))
        {
            mVersionMajor = mVersionMajor * 10 + bytes[p] - '0';
            p++;

            if (p >= len)
                return 0;
        }

        // Dot.
        if (bytes[p++] != '.')
            return 0;

        if (p >= len)
            return 0;

        // Minor version number.
        if (!isDigit(bytes[p]))
            return 0;

        mVersionMinor = 0;
        while (isDigit(bytes[p]))
        {
            mVersionMinor = mVersionMinor * 10 + bytes[p] - '0';
            p++;

            if (p >= len)
                return 0;
        }

        // Space.
        if (bytes[p++] != ' ')
            return 0;

        if (p >= len)
            return 0;

        // code
        mResponseCode = 0;
        while (isDigit(bytes[p]))
        {
            mResponseCode = mResponseCode * 10 + (bytes[p] - '0');
            p++;

            if (p >= len)
                return 0;
        }

        // status
        int start = p;
        while (!isCtl(bytes[p]))
        {
            p++;

            if (p >= len)
                return 0;
        }
        byte status[] = new byte[p - start];
        System.arraycopy(bytes, start, status, 0, p - start);
        mResponseStatus = new String(status);

        // CRLF.
        if (bytes[p++] != '\r')
            return 0;

        if (p >= len)
            return 0;

        if (bytes[p++] != '\n')
            return 0;

        if (p >= len)
            return 0;

        mType = Type.Response;

        // length of status line
        return p;
    }

    // return length of status line
    private int loadRequestLine(byte[] bytes, int len) {
        if (bytes == null)
            return 0;

        if (len == 0)
            return 0;

        int p = 0;

        // Request method.
        int start = p;
        while (isChar(bytes[p]) && !isCtl(bytes[p]) && !isTspecial(bytes[p]) && bytes[p] != ' ')
        {
            p++;

            if (p >= len)
                return 0;
        }
        byte method[] = new byte[p - start];
        System.arraycopy(bytes, start, method, 0, p - start);
        mMethod = new String(method);

        if (mMethod.length() == 0)
            return 0;

        // Space.
        if (bytes[p++] != ' ')
            return 0;

        if (p >= len)
            return 0;

        // URI.
        start = p;
        while (!isCtl(bytes[p]) && bytes[p] != ' ')
        {
            p++;

            if (p >= len)
                return 0;
        }
        byte uri[] = new byte[p - start];
        System.arraycopy(bytes, start, uri, 0, p - start);
        mUri = new String(uri);

        if (mUri.length() == 0)
            return 0;

        // Space.
        if (bytes[p++] != ' ')
            return 0;

        if (p >= len)
            return 0;

        // Rtsp protocol identifier.
        if (bytes[p++] != 'R')
            return 0;

        if (p >= len)
            return 0;

        if (bytes[p++] != 'T')
            return 0;

        if (p >= len)
            return 0;

        if (bytes[p++] != 'S')
            return 0;

        if (p >= len)
            return 0;

        if (bytes[p++] != 'P')
            return 0;

        if (p >= len)
            return 0;

        // Slash.
        if (bytes[p++] != '/')
            return 0;

        if (p >= len)
            return 0;

        // Major version number.
        if (!isDigit(bytes[p]))
            return 0;

        mVersionMajor = 0;
        while (isDigit(bytes[p]))
        {
            mVersionMajor = mVersionMajor * 10 + bytes[p] - '0';
            p++;

            if (p >= bytes.length)
                return 0;
        }

        // Dot.
        if (bytes[p++] != '.')
            return 0;

        if (p >= len)
            return 0;

        // Minor version number.
        if (!isDigit(bytes[p]))
            return 0;

        mVersionMinor = 0;
        while (isDigit(bytes[p]))
        {
            mVersionMinor = mVersionMinor * 10 + (char) bytes[p] - '0';
            p++;

            if (p >= len)
                return 0;
        }

        // CRLF.
        if (bytes[p++] != '\r')
            return 0;

        if (p >= len)
            return 0;

        if (bytes[p++] != '\n')
            return 0;

        mType = Type.Request;

        // length of status line
        return p;
    }

    // return length of heads
    private int loadHeads(byte[] bytes, int len, int startPosition) {

        int p = startPosition;

        if (p >= len)
            return 0;

        mHeaders.clear();

        // Headers.
        while ((isChar(bytes[p]) && !isCtl(bytes[p]) && !isTspecial(bytes[p]) && bytes[p] != '\r')
                || (bytes[p] == ' ' || bytes[p] == '\t')) {

            String name = null;
            String value = null;

            if (bytes[p] == ' ' || bytes[p] == '\t') {
                return 0;
            }
            else
            {
                // Start the next header.
                if (p >= len)
                    break;

                // Header name.
                int start = p;
                while (isChar(bytes[p]) && !isCtl(bytes[p]) && !isTspecial(bytes[p])
                        && bytes[p] != ':')
                {
                    p++;

                    if (p >= len)
                        break;
                }
                byte k[] = new byte[p - start];
                System.arraycopy(bytes, start, k, 0, p - start);
                name = new String(k);

                // Colon and space separates the header name from the header
                // value.
                if (bytes[p++] != ':')
                    return 0;

                if (p >= len)
                    return 0;

                // skip SPACE if necessary
                if (bytes[p] == ' ') {
                    p++;

                    if (p >= len)
                        break;
                }
            }

            // Header value.
            int start = p;
            while (bytes[p] != '\r')
            {
                p++;

                if (p >= len)
                    break;
            }
            byte k[] = new byte[p - start];
            System.arraycopy(bytes, start, k, 0, p - start);
            value = new String(k);

            mHeaders.put(name.toLowerCase(Locale.getDefault()), value);

            if (p >= len)
                break;

            // CRLF.
            if (bytes[p++] != '\r')
                return 0;

            if (p >= len)
                return 0;

            if (bytes[p++] != '\n')
                return 0;

            if (p >= len)
                break;
        }

        if (p >= len)
            return (p - startPosition);

        // CRLF.
        if (bytes[p++] != '\r')
            return 0;

        if (p >= len)
            return 0;

        if (bytes[p] != '\n')
            return 0;

        p++;

        // length of heads
        return (p - startPosition);
    }

    // return length of content
    private int loadContent(byte[] bytes, int len, int startPosition) {
        if (bytes == null)
            return 0;

        if (mContent == null)
            return 0;

        int length = len - startPosition;
        if (length > (mContent.length - mContentPosition))
            length = mContent.length - mContentPosition;

        System.arraycopy(bytes, startPosition, mContent, mContentPosition, length);
        mContentPosition += length;

        return length;
    }

    // Check if a byte is an Rtsp character.
    private static boolean isChar(byte c) {
        return (c >= 0 && c <= 127);
    }

    // Check if a byte is an Rtsp control character.
    private static boolean isCtl(byte c) {
        return (c >= 0 && c <= 31) || (c == 127);
    }

    // Check if a byte is defined as an Rtsp tspecial character.
    private static boolean isTspecial(byte c) {
        boolean ret = false;
        switch (c)
        {
            case '(':
            case ')':
            case '<':
            case '>':
            case '@':
            case ',':
            case ';':
            case ':':
            case '\\':
            case '"':
            case '/':
            case '[':
            case ']':
            case '?':
            case '=':
            case '{':
            case '}':
            case ' ':
            case '\t':
                ret = true;
                break;

            default:
                break;
        }

        return ret;
    }

    // Check if a byte is a digit.
    private static boolean isDigit(byte c) {
        return (c >= '0' && c <= '9');
    }
}
