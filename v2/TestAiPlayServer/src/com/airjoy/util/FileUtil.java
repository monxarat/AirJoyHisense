
package com.airjoy.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class FileUtil {

    private static FileUtil mInstance;
    private FileInputStream f;

    private FileUtil() {
    }

    public static FileUtil getInstance() {
        if (mInstance == null) {
            mInstance = new FileUtil();
        }

        return mInstance;
    }

    public long getFileSize(String uri) {
        long size = 0;

        try {
            File file = new File(uri);
            if (file.exists()) {
                    f = new FileInputStream(file);
                size = f.available();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return size;
    }
}
