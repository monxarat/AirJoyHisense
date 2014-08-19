
package com.airjoy.util;

public class Bytes {

    public static byte[] cat(byte[]... args) {
        int count = args.length;

        int totalLength = 0;
        for (int i = 0; i < count; ++i) {
            totalLength += args[i].length;
        }

        byte[] data = new byte[totalLength];

        int pos = 0;
        for (int i = 0; i < count; ++i) {
            System.arraycopy(args[i], 0, data, pos, args[i].length);
            pos += args[i].length;
        }

        return data;
    }
}
