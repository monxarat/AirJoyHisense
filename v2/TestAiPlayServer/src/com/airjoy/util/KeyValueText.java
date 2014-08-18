
package com.airjoy.util;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;

public class KeyValueText {

    private Map<String, String> mKeyValues = new HashMap<String, String>();

    public void loadText(String text) {
        this.loadBytes(text.getBytes());
    }

    public void loadBytes(byte[] bytes) {
        if (bytes == null)
            return;

        int p = 0;

        mKeyValues.clear();

        // Headers.
        while ((isChar(bytes[p]) && !isCtl(bytes[p]) && !isTspecial(bytes[p]) && bytes[p] != '\r')
                || (bytes[p] == ' ' || bytes[p] == '\t')) {
            String name = new String();
            String value = new String();

            if (bytes[p] == ' ' || bytes[p] == '\t') {
                return;
            }
            else
            {
                // Start the next header.

                // Header name.
                while (isChar(bytes[p]) && !isCtl(bytes[p]) && !isTspecial(bytes[p])
                        && bytes[p] != ':')
                {
                    name += (char) bytes[p];
                    p++;

                    if (p >= bytes.length)
                        break;
                }

                // Colon and space separates the header name from the header
                // value.
                if (bytes[p++] != ':')
                    return;

                if (p >= bytes.length)
                    return;

                if (bytes[p++] != ' ')
                    return;

                if (p >= bytes.length)
                    break;
            }

            // Header value.
            while (bytes[p] != '\r' && bytes[p] != '\n')
            {
                value += (char) bytes[p];
                p++;

                if (p >= bytes.length)
                    break;
            }

            mKeyValues.put(name.toLowerCase(Locale.getDefault()), value);

            if (p >= bytes.length)
                break;

            // CRLF
            if (bytes[p] == '\r') {
                p++;

                if (p >= bytes.length)
                    break;

                if (bytes[p++] != '\n')
                    break;

                if (p >= bytes.length)
                    break;
            }
            else if (bytes[p] == '\n') {
                p++;

                if (p >= bytes.length)
                    break;
            }
            else {
                break;
            }
        }

        return;
    }

    public String toText() {
        String text = new String();

        Iterator<Entry<String, String>> iter = mKeyValues.entrySet().iterator();
        while (iter.hasNext()) {
            Entry<String, String> entry = iter.next();
            String key = entry.getKey();
            String value = entry.getValue();
            String line = String.format("%s: %s\r\n", key, value);

            if (text == null) {
                text = line;
            }
            else {
                text = String.format("%s%s", text, line);
            }
        }

        return text;
    }

    public byte[] toBytes() {
        return toText().getBytes();
    }

    public String getValue(String name) {
        String lowerName = name.toLowerCase(Locale.getDefault());
        return mKeyValues.get(lowerName);
    }

    public void setValue(String key, String value) {
        mKeyValues.put(key, value);
    }

    // Check if a byte is an HTTP character.
    private static boolean isChar(byte c) {
        return (c >= 0 && c <= 127);
    }

    // Check if a byte is an HTTP control character.
    private static boolean isCtl(byte c) {
        return (c >= 0 && c <= 31) || (c == 127);
    }

    // Check if a byte is defined as an HTTP tspecial character.
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

}
